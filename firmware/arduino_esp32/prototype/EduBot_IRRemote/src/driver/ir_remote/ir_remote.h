#ifndef _IR_REMOTE_H_
#define _IR_REMOTE_H_

#include <Arduino.h>




class IrRemote
{
  public:
    IrRemote();
    ~IrRemote();
    
    bool begin(uint8_t _rx_pin = 5);
    bool isInit(void);


    uint32_t available(void);
    uint32_t read(void);
    
  private:
    bool is_init;
    uint8_t rx_pin;    
};



#endif