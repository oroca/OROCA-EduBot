#include "ir_remote.h"


typedef struct
{
  uint8_t  bit_count;
  uint8_t  bit_start;
  uint8_t  bit_end;
  uint16_t bit_high_min;
  uint16_t bit_high_max;
} ir_remote_bit_format_t;



static ir_remote_bit_format_t bit_format;



static rmt_obj_t* rmt_recv = NULL;
static volatile uint32_t ir_recevied_data = 0;
static volatile bool     is_recevied = false;
static volatile bool     print_log = false;


extern "C" void IRAM_ATTR ir_remote_isr(uint32_t *data, size_t len)
{
  rmt_data_t *p_data = (rmt_data_t*) data;

  
  if (print_log == true)
  {
    Serial.print("Len : ");  
    Serial.println(len);

    for(size_t i = 0; i<len; i++)
    {
      Serial.printf("%d : %d, %d, %d, %d\n", i,
                                         p_data[i].duration0,   
                                         p_data[i].level0,
                                         p_data[i].duration1,
                                         p_data[i].level1);
    }
  }

  if (len == bit_format.bit_count)
  {
    int index = 0;
    uint32_t ir_data = 0;
    for (int i=bit_format.bit_start; i<=bit_format.bit_end; i++)
    {
      if (p_data[i].duration1 >= bit_format.bit_high_min && p_data[i].duration1 <= bit_format.bit_high_max)
      {
        ir_data |= (1<<index);
      }
      index++;
    }
    ir_recevied_data = ir_data;
  }

  is_recevied = true;
}



IrRemote::IrRemote(void)
{
  is_init = false;

  bit_format.bit_count = 34;
  bit_format.bit_start = 1;
  bit_format.bit_end   = 32;

  bit_format.bit_high_min = 7;
  bit_format.bit_high_max = 10;
}

IrRemote::~IrRemote()
{
	
}

bool IrRemote::isInit(void)
{
  return is_init;
}

bool IrRemote::begin(uint8_t _rx_pin)
{
  rx_pin = _rx_pin;


  if ((rmt_recv = rmtInit(rx_pin, false, RMT_MEM_64)) == NULL)
  {
    Serial.println("ir_remote : failed\n");
    return false;
  }

  
  rmtSetTick(rmt_recv, 200000);
  
  rmtRead(rmt_recv, ir_remote_isr);    
  is_init = true;
}

uint32_t IrRemote::available(void)
{
  return is_recevied;
}

uint32_t IrRemote::read(void)
{
  is_recevied = false;
  return ir_recevied_data;
}