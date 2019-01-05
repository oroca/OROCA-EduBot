#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <Arduino.h>
#include "MAX98357.h"
#include "SPIFFS.h"  



#define AUDIO_SAMPLE_8BIT     I2S_BITS_PER_SAMPLE_8BIT
#define AUDIO_SAMPLE_16BIT    I2S_BITS_PER_SAMPLE_16BIT
#define AUDIO_1CH             1
#define AUDIO_2CH             2


typedef struct { 
  uint8_t  ChunkID[4]; 
  uint32_t ChunkSize; 
  uint8_t  Format[4]; 
} RIFF;

//------------------------------------------- 
// [Channel] 
// - streo : [left][right] 
// - 3 channel : [left][right][center] 
// - quad : [front left][front right][rear left][reat right] 
// - 4 channel : [left][center][right][surround] 
// - 6 channel : [left center][left][center][right center][right][surround] 
//------------------------------------------- 
typedef struct { 
  uint8_t  ChunkID[4];   // Contains the letters "fmt " in ASCII form 
  uint32_t ChunkSize;    // 16 for PCM. This is the size of the rest of the Subchunk which follows this number. 
  uint16_t AudioFormat;  // PCM = 1 
  uint16_t NumChannels;  // Mono = 1, Stereo = 2, etc. 
  uint32_t SampleRate;   // 8000, 44100, etc. 
  uint32_t AvgByteRate;  // SampleRate * NumChannels * BitsPerSample/8 
  uint16_t BlockAlign;   // NumChannels * BitsPerSample/8 
  uint16_t BitPerSample; // 8 bits = 8, 16 bits = 16, etc 
} FMT;

typedef struct { 
  char     ChunkID[4]; // Contains the letters "data" in ASCII form 
  uint32_t ChunkSize;  // NumSamples * NumChannels * BitsPerSample/8 
} DATA;


typedef struct
{
  RIFF Riff; 
  FMT	 Fmt; 
  DATA Data;
} wav_header_t;


class Audio
{
  public:
    Audio();
    ~Audio();
    
    bool begin(void);
    bool isInit(void);

    bool playFile(char *p_name, bool wait);
    void playStop(void);
    void waitStop(void);
    bool isBusy(void);

  private:
    
};



#endif