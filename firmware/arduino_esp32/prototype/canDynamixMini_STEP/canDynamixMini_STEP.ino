#include "canDynamixMini.h"
#include "BluetoothSerial.h"


ImuSensor imu;
Audio     audio;
StepMotor step_l(0);
StepMotor step_r(1);
BluetoothSerial SerialBT;

void motorInit(void);
void motorSetSpeed(int32_t left, int32_t right);
void motorSetStep(int32_t left, int32_t right, bool wait = false);




void setup() 
{
  Serial.begin(115200);
  
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

  SerialBT.begin("canDynamix");
  audio.playFile("/intro.wav", false);  
}


void loop()
{
  static uint32_t pre_time[2];
  int32_t speed = 100;
  static uint32_t cnt = 0;
  uint32_t timeout;
  int32_t turn_step = 940;
  
  imu.update();  

  if (millis()-pre_time[0] >= 100)
  {
    pre_time[0] = millis();
    //Serial.println(hallRead());
    //SerialBT.println(hallRead());
    Serial.print(step_l.step_count);
    Serial.print(" ");
    Serial.print(step_l.cur_speed);
    Serial.print(" ");
    Serial.print(step_l.index);
    Serial.print(" ");
    Serial.println(step_l.target_speed);
    SerialBT.println(step_l.step_count);
  }

  if (SerialBT.available() > 0)
  {
    pre_time[1] = millis();

    uint8_t ch;

    ch = SerialBT.read();

    if (ch == 'i')
    {
      motorSetStep(1000, 1000);
    }
    if (ch == 'k')
    {
      motorSetStep(-1000, -1000);
    }
    if (ch == 'j')
    {
      motorSetStep(-turn_step, turn_step);       
    }
    if (ch == 'l')
    {
      motorSetStep(turn_step, -turn_step);       
    }    
    if (ch == '1')
    {
      motorSetStep(1000, 1000, true);
      motorSetStep(-turn_step, turn_step, true);       
      motorSetStep(1000, 1000, true);
      motorSetStep(-turn_step, turn_step, true);       
      motorSetStep(1000, 1000, true);
      motorSetStep(-turn_step, turn_step, true);       
      motorSetStep(1000, 1000, true);
      motorSetStep(-turn_step, turn_step, true);       
    }
    if (ch == '2')
    {
      motorSetStep(1000, 1000, true);
      motorSetStep( turn_step,-turn_step, true);       
      motorSetStep(1000, 1000, true);
      motorSetStep( turn_step,-turn_step, true);       
      motorSetStep(1000, 1000, true);
      motorSetStep( turn_step,-turn_step, true);       
      motorSetStep(1000, 1000, true);
      motorSetStep( turn_step,-turn_step, true);       
    }
    if (ch == 'r')
    {
      
    }
    if (ch == '8')
    {
      step_l.setAcc(1);
      step_r.setAcc(1);
    }
    if (ch == '9')
    {
      step_l.setAcc(2);
      step_r.setAcc(2);
    }
    if (ch == '0')
    {
      step_l.setAcc(3);
      step_r.setAcc(3);
    }

    cnt++;
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
