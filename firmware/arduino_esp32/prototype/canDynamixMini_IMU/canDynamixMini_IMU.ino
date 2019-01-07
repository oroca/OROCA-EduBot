#include "canDynamixMini.h"


ImuSensor imu;

StepMotor step_l(0);
StepMotor step_r(1);
 

void setup() 
{
  Serial.begin(115200);

  if (imu.begin() == true)
  {
    Serial.println("IMU Init : OK");
  }
  else
  {
    Serial.println("IMU Init : Fail");
  }
  step_l.begin(12, 14, 32, -1);  // enable_pin, step_pin, dir_pin, dir
  step_l.setAcc(1);
  step_l.setSpeed(0);

  step_r.begin(27, 15, 33, 1);   // enable_pin, step_pin, dir_pin, dir
  step_r.setAcc(1);
  step_r.setSpeed(0);
}


void loop()
{
  static uint8_t state = 0;
  static uint8_t next_state = 0;
  static uint32_t pre_time[2];
  static uint32_t timeout;
  static uint8_t acc = 1;
  static int32_t speed;

  switch(state)
  {
    case 0:
      speed = 300;
      step_l.setSpeed(speed);
      step_r.setSpeed(speed);
      state = 99;
      next_state = 1;
      timeout = 2000;
      pre_time[0] = millis();
      break;

    case 1:
      step_l.setSpeed(-speed);
      step_r.setSpeed(-speed);
      state = 99;
      next_state = 0;
      timeout = 2000;
      pre_time[0] = millis();
      step_l.setAcc(acc);
      step_r.setAcc(acc);
      acc++;

      if (acc > 3)
      {
        acc = 1;
      }
      break;

    case 99:
      if (millis()-pre_time[0] >= timeout)
      {
        state = next_state;
      }
      break;
  }

  imu.update();
  
#if 0
  if (millis()-pre_time[1] >= 100)
  {
    pre_time[1] = millis();

    Serial.print("L cnt : ");
    Serial.print(step_l.step_count);
    Serial.print("\t L cur_spd : ");
    Serial.print(step_l.cur_speed);

    Serial.print("\tR cnt : ");
    Serial.print(step_r.step_count);
    Serial.print("\t R cur_spd : ");
    Serial.println(step_r.cur_speed);    
  }
#else  
  if (millis()-pre_time[1] >= 10)
  {
    pre_time[1] = millis();

    Serial.print("R : ");
    Serial.print(imu.getRoll());
    Serial.print("\tP : ");
    Serial.print(imu.getPitch());

    Serial.print("\tY : ");
    Serial.print(imu.getYaw());
    Serial.println();    
  }
#endif
}



