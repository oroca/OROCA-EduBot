#include "config.h"
#include "logo.h"


// 0xBF40FF00 전진
// 0xE619FF00 후진 
// 0xF807FF00 좌회전 
// 0xF609FF00 후회전 


IrRemote  ir_remote;
ImuSensor imu;
Audio     audio;
StepMotor step_l(0);
StepMotor step_r(1);


void motorInit(void);
void motorSetSpeed(int32_t left, int32_t right);
void motorSetStep(int32_t left, int32_t right, bool wait = false);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  ir_remote.begin();
  audio.begin();
  
  if (imu.begin() == true)
  {
    Serial.println("IMU Init : OK");
  }
  else
  {
    Serial.println("IMU Init : Fail");
  }

  motorInit();    
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint32_t pre_time;
  int32_t speed = 100;
  

  if (ir_remote.available() > 0)
  {
    uint32_t key_code;

    key_code = ir_remote.read();

    Serial.print("Key : 0x");
    Serial.print(key_code, HEX);
    Serial.print(" ");
    Serial.println(millis()-pre_time);


    if (key_code == 0xBF40FF00)
    {
      motorSetSpeed(speed, speed);
    }
    if (key_code == 0xE619FF00)
    {
      motorSetSpeed(-speed, -speed);
    }
    if (key_code == 0xF807FF00)
    {
      motorSetSpeed(-speed, speed);
    }
    if (key_code == 0xF609FF00)
    {
      motorSetSpeed(speed, -speed);
    }    
   

    pre_time = millis();
  }

  if (millis()-pre_time >= 150)
  {
      motorSetSpeed(0, 0);
  }  
}


void motorInit(void)
{
  step_l.begin(27, 14, 32, -1);  // enable_pin, step_pin, dir_pin, dir
  step_l.setAcc(1);
  step_l.setSpeed(0);

  step_r.begin(12, 15, 33, 1);   // enable_pin, step_pin, dir_pin, dir
  step_r.setAcc(1);
  step_r.setSpeed(0);  
}

void motorSetSpeed(int32_t left, int32_t right)
{
  step_l.setSpeed(left);
  step_r.setSpeed(right);  
}

void motorSetStep(int32_t left, int32_t right, bool wait)
{
  step_l.setStep(left);
  step_r.setStep(right);  

  if (wait == true)
  {
    while(step_l.isBusy());
    while(step_r.isBusy());
  }
}




