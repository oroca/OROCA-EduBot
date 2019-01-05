#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_

#include <Arduino.h>



typedef struct
{
	int32_t step_index;
	uint8_t count_index;
	uint8_t count_length;
	
	int32_t target_speed[3];
	int32_t target_index[3];

} step_profile_t;



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
    
    void setStep(int32_t value, int32_t max_speed = 0, bool accel = true);
    bool isBusy(void);

    int32_t  step_count;
    int32_t  index;
    step_profile_t profile;

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

    int32_t  total_step;

    hw_timer_t * timer;
};



#endif