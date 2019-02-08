#ifndef _IMU_H_
#define _IMU_H_

#include <Arduino.h>
#include "SparkFunMPU9250-DMP.h"





class ImuSensor 
{
  public:
    ImuSensor();
    ~ImuSensor();
    
    bool begin(void);
    bool isInit(void);
    bool update(void);


    float getRoll(void) { return m_imu.roll; }
    float getPitch(void) { return m_imu.pitch; }
    float getYaw(void) { return m_imu.yaw; }    

  private:
    bool is_init;
    MPU9250_DMP m_imu;
};



#endif