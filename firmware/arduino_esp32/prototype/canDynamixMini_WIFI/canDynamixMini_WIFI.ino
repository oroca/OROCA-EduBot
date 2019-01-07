#include "canDynamixMini.h"

#include <WiFi.h>
#include <WiFiClient.h>


#define WIFI_CLIENT_MAX     4

const char *ssid = "canDynamix"; 
const char *pw   = "canDynamix"; 
IPAddress ip(192, 168, 1, 100);   
IPAddress netmask(255, 255, 255, 0);
WiFiServer wifi_server(8881);
WiFiClient wifi_client;


ImuSensor imu;
Audio     audio;
StepMotor step_l(0);
StepMotor step_r(1);


void motorInit(void);
void motorSetSpeed(int32_t left, int32_t right);
void motorSetStep(int32_t left, int32_t right, bool wait = false);




void setup() 
{
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask); 
  WiFi.softAP(ssid, pw);              

  wifi_server.begin();
  wifi_server.setNoDelay(true);


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

  audio.playFile("/intro.wav", false);  

  pinMode(13, OUTPUT);

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

    digitalWrite(13, !digitalRead(13));
  }


  if (wifi_server.hasClient())
  {
    if (wifi_client && !wifi_client.connected())
    {
        wifi_client.stop();

        wifi_client = wifi_server.available();
        wifi_client.setNoDelay(true);
        Serial.println("client start"); 
    }
    if (!wifi_client)
    {
        wifi_client = wifi_server.available();
        wifi_client.setNoDelay(true);
        Serial.println("client start"); 
    }
  }


  if(wifi_client && wifi_client.connected()) 
  {
    while(wifi_client.available())
    {
      uint8_t ch = wifi_client.read();
      Serial.write(ch);

      pre_time[1] = millis();

      if (ch == 'U')
      {
        motorSetSpeed(speed, speed);
      }
      if (ch == 'D')
      {
        motorSetSpeed(-speed, -speed);
      }
      if (ch == 'L')
      {
        motorSetSpeed(-speed, speed);
      }
      if (ch == 'R')
      {
        motorSetSpeed(speed, -speed);
      }    
      if (ch == 'A')
      {
        audio.playFile("/a.wav", false);
      }
      if (ch == 'B')
      {
        audio.playFile("/r.wav", false);
      }      
    } 

    if (millis()-pre_time[1] >= 100)
    {
      motorSetSpeed(0, 0);
    }
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

