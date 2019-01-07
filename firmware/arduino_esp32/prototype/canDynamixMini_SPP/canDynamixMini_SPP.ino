#include "canDynamixMini.h"
#include "BluetoothSerial.h"


ImuSensor imu;
Audio     audio;
StepMotor step_l(0);
StepMotor step_r(1);
BluetoothSerial SerialBT;

void motorInit(void);
void motorSetSpeed(int32_t left, int32_t right);





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

  imu.update();  

  if (millis()-pre_time[0] >= 100)
  {
    pre_time[0] = millis();
    //Serial.println(hallRead());
    //SerialBT.println(hallRead());
  }

  if (SerialBT.available() > 0)
  {
    pre_time[1] = millis();

    uint8_t ch;

    ch = SerialBT.read();

    if (ch == 'i')
    {
      motorSetSpeed(speed, speed);
    }
    if (ch == 'k')
    {
      motorSetSpeed(-speed, -speed);
    }
    if (ch == 'j')
    {
      motorSetSpeed(-speed, speed);
    }
    if (ch == 'l')
    {
      motorSetSpeed(speed, -speed);
    }    
    if (ch == 'a')
    {
      audio.playFile("/a.wav", false);
    }
    if (ch == 'r')
    {
      audio.playFile("/r.wav", false);
    }
    cnt++;
  }
  else
  {
    if (cnt == 1)
    {
      timeout = 550;
    }
    else
    {
      timeout = 100;
    }
    if (millis()-pre_time[1] >= timeout)
    {
      cnt = 0;
      motorSetSpeed(0, 0);
    }
  }
}

void motorInit(void)
{
  step_l.begin(12, 14, 32, -1);  // enable_pin, step_pin, dir_pin, dir
  step_l.setAcc(1);
  step_l.setSpeed(0);

  step_r.begin(27, 15, 33, 1);   // enable_pin, step_pin, dir_pin, dir
  step_r.setAcc(1);
  step_r.setSpeed(0);  
}

void motorSetSpeed(int32_t left, int32_t right)
{
  step_l.setSpeed(left);
  step_r.setSpeed(right);  
}