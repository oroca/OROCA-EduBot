#include "imu.h"




ImuSensor::ImuSensor(void)
{
  is_init = false;
}

ImuSensor::~ImuSensor()
{
	
}

bool ImuSensor::isInit(void)
{
  return is_init;
}

bool ImuSensor::begin(void)
{
  inv_error_t inv_error;


  // Call imu.begin() to verify communication and initialize
  if (m_imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
    return false;
  }
  
  inv_error = m_imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
                           DMP_FEATURE_GYRO_CAL,    // Use gyro calibration
                           200);                    // Set DMP FIFO rate to 10 Hz

  if (inv_error != INV_SUCCESS)
  {
    Serial.print("dmpBegin err : ");
    Serial.println(inv_error);
    return false;
  }

  return true;
}

bool ImuSensor::update(void)
{
  bool ret = false;


  // Check for new data in the FIFO
  if ( m_imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( m_imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      m_imu.computeEulerAngles();
      ret = true;
    }
  }  

  return ret;
}