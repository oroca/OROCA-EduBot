#include "audio.h"




static pcm_format_t bt_buffer_fmt = {
    .sample_rate   = 8000, // 44100
    .bit_depth     = I2S_BITS_PER_SAMPLE_16BIT,
    .num_channels  = 2,
    .buffer_format = PCM_INTERLEAVED
};


typedef struct
{
  bool is_init;  
  bool is_busy;
  bool is_stop;
  bool is_opened;

  char file_name[128];
  pcm_format_t buffer_fmt;
  uint8_t spk_buf[2][4096];  
  
} audio_node_t;


audio_node_t audio_play;
wav_header_t wav_header;
MAX98357 speaker;


volatile SemaphoreHandle_t play_semaphore = NULL;
volatile TaskHandle_t _play_task_handle = NULL;


bool wavplayStartFile(char *p_name);


void audioThread(void *arg)
{
  while(1)
  {
    if (xSemaphoreTake(play_semaphore, 0) == pdTRUE)
    {
      audio_play.is_busy = true;
      wavplayStartFile(audio_play.file_name);
      audio_play.is_busy = false;
    }      
    delay(1);
  }
}


Audio::Audio(void)
{
  audio_play.is_init = false;
  audio_play.is_opened = false;
  audio_play.is_stop = false;

  audio_play.buffer_fmt.sample_rate = 8000;
  audio_play.buffer_fmt.bit_depth = I2S_BITS_PER_SAMPLE_16BIT;
  audio_play.buffer_fmt.num_channels = 2;
  audio_play.buffer_fmt.buffer_format = PCM_INTERLEAVED;
}

Audio::~Audio()
{
	
}

bool Audio::isInit(void)
{
  return audio_play.is_init;
}

bool Audio::begin(void)
{  
  if (audio_play.is_init == false)
  {
    speaker.begin();

    if (play_semaphore == NULL)
    {
      play_semaphore = xSemaphoreCreateBinary();
    }
    xTaskCreate(audioThread, "audioThread", 2048, NULL, 3, (TaskHandle_t*)&_play_task_handle);    
    

    if (!_play_task_handle)
    {
      audio_play.is_init = false;
    }
    else
    {
      audio_play.is_init = true;
    }    
    
    if(!SPIFFS.begin(true))
    {
      audio_play.is_init = false;    
      return false;
    }  
  }
  

  return audio_play.is_init;
}

bool Audio::playFile(char *p_name, bool wait)
{
  if (audio_play.is_init == false)
  {
    return false;
  }


  audio_play.is_stop = true;
  while(audio_play.is_busy)
  {
    delay(1);
  }
  audio_play.is_stop = false;


  strcpy(audio_play.file_name, p_name);
  
  xSemaphoreGive(play_semaphore);

  if (wait == true)
  {
    audio_play.is_busy = true;
    while(audio_play.is_busy)
    {
      delay(1);
    }
  }
  else
  {
    audio_play.is_busy = true;
  }
  return true;
}

void Audio::playStop(void)
{
  audio_play.is_stop = true;
  while(audio_play.is_busy)
  {
    delay(1);
  }
  audio_play.is_stop = false;
}

void Audio::waitStop(void)
{
  while(audio_play.is_busy)
  {
    delay(1);
  }
}

bool Audio::isBusy(void)
{
  return audio_play.is_busy;  
}

bool wavplayStartFile(char *p_name)
{  
  bool started;
  uint8_t buf_i;

  File file = SPIFFS.open(p_name);
  if(!file)
  {
    Serial.println("Failed to open file for reading");
    return false;
  }


  file.read((uint8_t *)&wav_header, 44);


  audio_play.buffer_fmt.num_channels = wav_header.Fmt.NumChannels;
  audio_play.buffer_fmt.sample_rate = wav_header.Fmt.SampleRate;

  /*
  Serial.print("AudioFormat : ");          
  Serial.println(wav_header.Fmt.AudioFormat);          

  Serial.print("NumChannels : ");          
  Serial.println(wav_header.Fmt.NumChannels);          

  Serial.print("SampleRate : ");          
  Serial.println(wav_header.Fmt.SampleRate);          

  Serial.print("AvgByteRate : ");          
  Serial.println(wav_header.Fmt.AvgByteRate);          

  Serial.print("BlockAlign : ");          
  Serial.println(wav_header.Fmt.BlockAlign);          

  Serial.print("BitPerSample : ");          
  Serial.println(wav_header.Fmt.BitPerSample); 
  */

  file.read(audio_play.spk_buf[1], 512);

  memset(audio_play.spk_buf[1], 0 ,4096);  
  speaker.start();
  speaker.write((char *)audio_play.spk_buf[1], 4096, &audio_play.buffer_fmt);     

  buf_i = 0;
  started = false;  
  while(1)
  {
    int length = file.available();

    if (length > 4096*1) length = 4096*1;
    if (length <= 0) break;

    file.read(audio_play.spk_buf[buf_i], length);

    if (started == false)
    {

      started = true;
    }
    speaker.write((char *)audio_play.spk_buf[buf_i], length, &bt_buffer_fmt);    
    delay(1);

    buf_i ^= 1;
    if (audio_play.is_stop == true)
    {
      speaker.stop();
      break;
    }
  }

  delay(100);
  speaker.stop();  

  file.close();  
}
