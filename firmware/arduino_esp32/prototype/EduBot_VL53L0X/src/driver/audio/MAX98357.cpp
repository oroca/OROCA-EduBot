#include "MAX98357.h"




MAX98357::MAX98357(i2s_port_t i2s_num, uint32_t pin_lrc, uint32_t pin_bck, uint32_t pin_din)
{
  is_init = false;

  renderer_config.bit_depth = I2S_BITS_PER_SAMPLE_16BIT;
  renderer_config.i2s_num = i2s_num;
  renderer_config.sample_rate = 44100;
  renderer_config.sample_rate_modifier = 1.0;
  renderer_config.output_mode = I2S;

  this->pin_lrc = pin_lrc;
  this->pin_bck = pin_bck;
  this->pin_din = pin_din;

  renderer_instance = &renderer_config;
  renderer_status = UNINITIALIZED;
}

MAX98357::~MAX98357()
{
  
}

void MAX98357::begin(void)
{
  renderer_config_t *config = &renderer_config;
  i2s_mode_t mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  i2s_comm_format_t comm_fmt = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);


  /* don't use audio pll on buggy rev0 chips */
  int use_apll = 0;
  esp_chip_info_t out_info;
  esp_chip_info(&out_info);
  if(out_info.revision > 0 && (config->output_mode == I2S || config->output_mode == I2S_MERUS)) {
      use_apll = 1;
      ESP_LOGI(TAG, "chip revision %d, enabling APLL for I2S", out_info.revision);
  }

  /*
   * Allocate just enough to decode AAC+, which has huge frame sizes.
   *
   * Memory consumption formula:
   * (bits_per_sample / 8) * num_chan * dma_buf_count * dma_buf_len
   *
   * 16 bit: 32 * 256 = 8192 bytes
   * 32 bit: 32 * 256 = 16384 bytes
   */
  i2s_config_t i2s_config;
    
  i2s_config.mode                 = mode;  
  i2s_config.sample_rate          = config->sample_rate;
  i2s_config.bits_per_sample      = config->bit_depth;
  i2s_config.channel_format       = I2S_CHANNEL_FMT_RIGHT_LEFT;  // 2-channels
  i2s_config.communication_format = comm_fmt;
  i2s_config.dma_buf_count        = 32;                          // number of buffers, 128 max.
  i2s_config.dma_buf_len          = 64;                          // size of each buffer
  i2s_config.intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1;        // Interrupt level 1
  i2s_config.use_apll             = use_apll;


  i2s_pin_config_t pin_config;
    
  pin_config.bck_io_num   = pin_bck;
  pin_config.ws_io_num    = pin_lrc;
  pin_config.data_out_num = pin_din;
  pin_config.data_in_num  = I2S_PIN_NO_CHANGE;

  i2s_driver_install(config->i2s_num, &i2s_config, 1, &i2s_event_queue);
  i2s_set_pin(config->i2s_num, &pin_config);
  i2s_stop(config->i2s_num);

  is_init = true;
  renderer_status = INITIALIZED;
}

void MAX98357::write(char *buf, uint32_t buf_len, pcm_format_t *buf_desc)
{
    //ESP_LOGI(TAG, "buf_desc: bit_depth %d format %d num_chan %d sample_rate %d", buf_desc->bit_depth, buf_desc->buffer_format, buf_desc->num_channels, buf_desc->sample_rate);
    //ESP_LOGI(TAG, "renderer_instance: bit_depth %d, output_mode %d", renderer_instance->bit_depth, renderer_instance->output_mode);

    // handle changed sample rate
    if(renderer_instance->sample_rate != buf_desc->sample_rate)
    {
        ESP_LOGI(TAG, "changing sample rate from %d to %d", renderer_instance->sample_rate, buf_desc->sample_rate);
        uint32_t rate = buf_desc->sample_rate * renderer_instance->sample_rate_modifier;
        i2s_set_sample_rates(renderer_instance->i2s_num, rate);
        renderer_instance->sample_rate = buf_desc->sample_rate;
    }

    uint8_t buf_bytes_per_sample = (buf_desc->bit_depth / 8);
    uint32_t num_samples = buf_len / buf_bytes_per_sample / buf_desc->num_channels;

    // formats match, we can write the whole block
    if (buf_desc->bit_depth == renderer_instance->bit_depth
            && buf_desc->buffer_format == PCM_INTERLEAVED
            && buf_desc->num_channels == 2
            && renderer_instance->output_mode != DAC_BUILT_IN) {

        // do not wait longer than the duration of the buffer
        TickType_t max_wait = buf_desc->sample_rate / num_samples / 2;

        // don't block, rather retry
        int bytes_left = buf_len;
        int bytes_written = 0;
        while(bytes_left > 0 && renderer_status != STOPPED) {
            bytes_written = i2s_write_bytes(renderer_instance->i2s_num, buf, bytes_left, 0);
            bytes_left -= bytes_written;
            buf += bytes_written;
            delay(1);
        }

        return;
    }

    // support only 16 bit buffers for now
    if(buf_desc->bit_depth != I2S_BITS_PER_SAMPLE_16BIT) {
        ESP_LOGE(TAG, "unsupported decoder bit depth: %d", buf_desc->bit_depth);
        return;
    }

    // pointer to left / right sample position
    char *ptr_l = buf;
    char *ptr_r = buf + buf_bytes_per_sample;
    uint8_t stride = buf_bytes_per_sample * 2;

    // right half of the buffer contains all the right channel samples
    if(buf_desc->buffer_format == PCM_LEFT_RIGHT)
    {
        ptr_r = buf + buf_len / 2;
        stride = buf_bytes_per_sample;
    }

    if (buf_desc->num_channels == 1) {
        ptr_r = ptr_l;
    }

    int bytes_pushed = 0;
    TickType_t max_wait = 20 / portTICK_PERIOD_MS; // portMAX_DELAY = bad idea
    for (int i = 0; i < num_samples; i++) {
        if (renderer_status == STOPPED) break;

        if(renderer_instance->output_mode == DAC_BUILT_IN)
        {
            // assume 16 bit src bit_depth
            short left = *(short *) ptr_l;
            short right = *(short *) ptr_r;

            // The built-in DAC wants unsigned samples, so we shift the range
            // from -32768-32767 to 0-65535.
            left = left + 0x8000;
            right = right + 0x8000;

            uint32_t sample = (uint16_t) left;
            sample = (sample << 16 & 0xffff0000) | ((uint16_t) right);

            bytes_pushed = i2s_push_sample(renderer_instance->i2s_num, (const char*) &sample, max_wait);
        }
        else {

            if (renderer_instance->bit_depth == I2S_BITS_PER_SAMPLE_16BIT)
            {
              /* low - high / low - high */
              const char samp32[4] = {ptr_l[0], ptr_l[1], ptr_r[0], ptr_r[1]};

              bytes_pushed = i2s_push_sample(renderer_instance->i2s_num, (const char*) &samp32, max_wait);
            }
            else if (renderer_instance->bit_depth == I2S_BITS_PER_SAMPLE_32BIT)
            {
              const char samp64[8] = {0, 0, ptr_l[0], ptr_l[1], 0, 0, ptr_r[0], ptr_r[1]};
              bytes_pushed = i2s_push_sample(renderer_instance->i2s_num, (const char*) &samp64, max_wait);
            }
            else
            { 
              ESP_LOGE(TAG, "bit depth unsupported: %d", renderer_instance->bit_depth);
            }
        }

        // DMA buffer full - retry
        if (bytes_pushed == 0) {
            i--;
        } else {
            ptr_r += stride;
            ptr_l += stride;
        }
    }
}

void MAX98357::clearBuffer(void)
{
  i2s_zero_dma_buffer(renderer_instance->i2s_num);  
}

void MAX98357::start(void)
{
  if(renderer_status == RUNNING)
  {
    return;
  }

  renderer_status = RUNNING;
  i2s_start(renderer_instance->i2s_num);

  // buffer might contain noise
  i2s_zero_dma_buffer(renderer_instance->i2s_num);
}

void MAX98357::stop(void)
{
  if(renderer_status == STOPPED)
  {
    return;
  }

  renderer_status = STOPPED;
  i2s_stop(renderer_instance->i2s_num);  
}