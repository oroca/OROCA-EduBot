#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_

#include <Arduino.h>






class StepMotor 
{
  public:
    StepMotor(uint8_t timer_num);
    ~StepMotor();
    
    void begin(int8_t enable_pin, int8_t step_pin, int8_t dir_pin, int8_t dir);
    void onTimer(void);  
    void stop(void);
    void start(void);
    void setAcc(int32_t value);
    void setSpeed(int32_t value);
    
  
    int32_t  step_count;
    

  //private:

    int8_t pin_enable;
    int8_t pin_step;
    int8_t pin_dir;

    int32_t cfg_dir;
    int32_t cur_dir;
    uint8_t timer_number;    
    uint32_t step_res;
    
    uint8_t  step_edge;
    uint8_t  step_state;

    int32_t pre_speed;
    int32_t cur_speed;
    int32_t target_speed;
    int32_t target_acc;

    hw_timer_t * timer;
};



#endif