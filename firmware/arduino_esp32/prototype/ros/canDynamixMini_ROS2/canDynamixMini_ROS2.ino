#include "canDynamixMini.h"

#include <ros2arduino.h>
#include <std_msgs/String.hpp>
#include <geometry_msgs/Twist.hpp>

#include <WiFi.h>
#define SSID       "ssid"
#define SSID_PW    "ssid_pw"
#define AGENT_IP   "ip"
#define AGENT_PORT 2018 //AGENT port number


void subscribeCmdVel(geometry_msgs::Twist* msg, void* arg);
void subscribeTimeSync(builtin_interfaces::Time* msg, void* arg);



ImuSensor imu;
Audio     audio;
StepMotor step_l(0);
StepMotor step_r(1);


void motorInit(void);
void motorSetSpeed(int32_t left, int32_t right);
void motorSetStep(int32_t left, int32_t right, bool wait = false);


class EduBot : public ros2::Node
{
public:
  EduBot()
  : Node()
  {
    /*******************************************************************************
    * Publisher
    *******************************************************************************/
  

    /*******************************************************************************
    * Subscriber
    *******************************************************************************/
    cmd_vel_sub_       = this->createSubscriber<geometry_msgs::Twist>("cmd_vel", (ros2::CallbackFunc)subscribeCmdVel, NULL);
    time_sync_sub_     = this->createSubscriber<builtin_interfaces::Time>("time_sync", (ros2::CallbackFunc)subscribeTimeSync, NULL);
  }


private:

  /* Publisher Pointer */
  
  /* Subscriber Pointer */
  ros2::Subscriber<geometry_msgs::Twist>*         cmd_vel_sub_;
  ros2::Subscriber<builtin_interfaces::Time>*     time_sync_sub_;
};



void setup() 
{
  Serial.begin(115200);
  

  WiFi.begin(SSID, SSID_PW);
  while(WiFi.status() != WL_CONNECTED);             

  ros2::init(AGENT_IP, AGENT_PORT, false);

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
  static EduBot EduBotNode;

  
  imu.update();  

  if (millis()-pre_time[0] >= 100)
  {
    pre_time[0] = millis();

    digitalWrite(13, !digitalRead(13));
  }

  ros2::spin(&EduBotNode);
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


void subscribeCmdVel(geometry_msgs::Twist* msg, void* arg)
{
  (void)(arg);
  //goal_velocity_from_cmd[LINEAR]  = constrain(msg->linear.x,  MIN_LINEAR_VELOCITY, MAX_LINEAR_VELOCITY);
  //goal_velocity_from_cmd[ANGULAR] = constrain(msg->angular.z, MIN_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);

  double wheel_speed_cmd[2];
  double lin_vel_left;
  double lin_vel_right;

  wheel_speed_cmd[0] = msg->linear.x - (msg->angular.z / 2 );
  wheel_speed_cmd[1] = msg->linear.x + (msg->angular.z / 2 );

  lin_vel_left  = constrain(wheel_speed_cmd[0] * 300, -300, 300);
  lin_vel_right = constrain(wheel_speed_cmd[1] * 300, -300, 300);

  motorSetSpeed((int32_t)lin_vel_left, (int32_t)lin_vel_right);
  
  Serial.print(msg->linear.x);
  Serial.print(" ");
  Serial.println(msg->angular.z);  
}


void subscribeTimeSync(builtin_interfaces::Time* msg, void* arg)
{
  (void)(arg);
  
  ros2::syncTimeFromRemote(msg);
}
