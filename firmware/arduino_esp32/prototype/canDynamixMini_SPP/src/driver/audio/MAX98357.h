#ifndef _MAX98357_H_
#define _MAX98357_H_


#include "Arduino.h"


#include "driver/gpio.h"
#include "driver/i2s.h"



typedef enum 
{
  I2S, I2S_MERUS, DAC_BUILT_IN, PDM
} output_mode_t;


typedef struct
{
  output_mode_t output_mode;
  int sample_rate;
  float sample_rate_modifier;
  i2s_bits_per_sample_t bit_depth;
  i2s_port_t i2s_num;
} renderer_config_t;


/* ESP32 is Little Endian, I2S is Big Endian.
 *
 * Samples produced by a decoder will probably be LE,
 * and I2S recordings BE.
 */
typedef enum
{
    PCM_INTERLEAVED, PCM_LEFT_RIGHT
} pcm_buffer_layout_t;

typedef enum
{
    PCM_BIG_ENDIAN, PCM_LITTLE_ENDIAN
} pcm_endianness_t;

typedef struct
{
    uint32_t sample_rate;
    i2s_bits_per_sample_t bit_depth;
    uint8_t num_channels;
    pcm_buffer_layout_t buffer_format;
    pcm_endianness_t endianness; // currently unused
} pcm_format_t;


typedef enum {
    UNINITIALIZED, INITIALIZED, RUNNING, STOPPED
} component_status_t;


class MAX98357 
{
  public:
    MAX98357(i2s_port_t i2s_num=I2S_NUM_0, uint32_t pin_lrc=GPIO_NUM_25, uint32_t pin_bck=GPIO_NUM_26, uint32_t pin_din=GPIO_NUM_21);
    ~MAX98357();    

    void begin(void);
    void write(char *buf, uint32_t buf_len, pcm_format_t *buf_desc);
    void clearBuffer(void);
    void start(void);
    void stop(void);

    bool is_init;
    
  private:    

    uint32_t pin_lrc;
    uint32_t pin_bck;
    uint32_t pin_din;

    component_status_t renderer_status;
    QueueHandle_t i2s_event_queue;
    renderer_config_t renderer_config;
    renderer_config_t *renderer_instance;
};


#endif

