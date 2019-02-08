#include "canDynamixMini.h"

#include "sys/time.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEBeacon.h>
#include <BLE2902.h>

#define ENABLE_DEBUG_PRINT
#ifdef ENABLE_DEBUG_PRINT
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINTLN(x) (x)
#endif

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define CONTROL_SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define INFO_SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914c"
#define MOTOR_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a7"
#define AUDIO_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define IMU_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a9"

typedef struct BLEServiceCharConfig{
  const char* uuid;
  uint32_t properties;
  BLECharacteristicCallbacks *callback;
}BLEServiceCharConfig;

BLEServer *pServer = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

ImuSensor imu;
Audio audio;
StepMotor step_l(0);
StepMotor step_r(1);
uint32_t pre_time_last_cmd_for_motor;

void motorInit(void);
void motorSetSpeed(int32_t left, int32_t right);
void bleInit(const char *device_name);
void bleSetService(const char *service_uuid, BLEServiceCharConfig *char_list);
void bleMaintain();
BLECharacteristic* bleGetServiceCharacteristic(const char* service_uuid, const char* char_uuid);

/*
  Server callback
*/
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

/**
 * Characteristic event callback
 */
class MotorCharEventCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pChar)
  {
    DEBUG_PRINTLN("MotorCharEventCallback - onWrite");
    uint32_t speed = 100;
    const char *p_string = pChar->getValue().c_str();
    
    pre_time_last_cmd_for_motor = millis();

    if (strcmp(p_string,"i") == 0)
    {
      motorSetSpeed(speed, speed);
    }
    else if (strcmp(p_string,"k") == 0)
    {
      motorSetSpeed(-speed, -speed);
    }
    else if (strcmp(p_string,"j") == 0)
    {
      motorSetSpeed(-speed, speed);
    }
    else if (strcmp(p_string,"l") == 0)
    {
      motorSetSpeed(speed, -speed);
    }
  }
};

class AudioCharEventCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pChar)
  {
    DEBUG_PRINTLN("AudioCharEventCallback - onWrite");
    const char *p_string = pChar->getValue().c_str();
    
//    if (strcmp(p_string,"a") == 0)
//    {
//      audio.playFile("/a.wav", false);
//    }
//    else if (strcmp(p_string,"r") == 0)
//    {
//      audio.playFile("/r.wav", false);
//    }
  }
};

class ImuCharEventCallback : public BLECharacteristicCallbacks
{
  void onRead(BLECharacteristic *pChar)
  {
    DEBUG_PRINTLN("ImuCharEventCallback - onRead");
    static uint32_t cnt = 0;
    char value[20];
    sprintf(value, "IMU_value! %d\r\n", cnt++);
    pChar->setValue(value);
  }
};



BLEServiceCharConfig control_service_char_list[] = 
{
  {MOTOR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE, new MotorCharEventCallback()},
  {AUDIO_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE, new AudioCharEventCallback()},
  {NULL, 0, NULL}
};

BLEServiceCharConfig info_service_char_list[] = 
{
  {IMU_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_NOTIFY, new ImuCharEventCallback()},
  {NULL, 0, NULL}
};



void setup()
{
  Serial.begin(115200);

  audio.begin();

  if (imu.begin() == true)
  {
    DEBUG_PRINTLN("IMU Init : OK");
  }
  else
  {
    DEBUG_PRINTLN("IMU Init : Fail");
  }

  motorInit();

  bleInit("canDynamix");
  bleSetService(CONTROL_SERVICE_UUID, control_service_char_list);
  bleSetService(INFO_SERVICE_UUID, info_service_char_list);

  pServer->startAdvertising();

//  audio.playFile("/intro.wav", false);
}

void loop()
{
  static uint32_t pre_time[1];

  if(millis() - pre_time[0] > 100)
  {
    pre_time[0] = millis();
    imu.update();

    BLECharacteristic *pIMUChar = bleGetServiceCharacteristic(INFO_SERVICE_UUID, IMU_CHARACTERISTIC_UUID);
    uint8_t notify_data[20] = {0, };
    float imu_data[3] = {imu.getRoll(), imu.getPitch(), imu.getYaw()};
    notify_data[0] = 0xEF;
    memcpy(&notify_data[1], &imu_data[0], sizeof(float));
    memcpy(&notify_data[5], &imu_data[1], sizeof(float));
    memcpy(&notify_data[9], &imu_data[2], sizeof(float));
    notify_data[19] = 0xFE;
    pIMUChar->setValue(notify_data, sizeof(notify_data));
    pIMUChar->notify();
  }

  if(millis() - pre_time_last_cmd_for_motor > 500)
  {
    pre_time_last_cmd_for_motor = millis();
    motorSetSpeed(0, 0);
  }

  bleMaintain();
}





void motorInit(void)
{
  step_l.begin(12, 14, 32, -1); // enable_pin, step_pin, dir_pin, dir
  step_l.setAcc(1);
  step_l.setSpeed(0);

  step_r.begin(27, 15, 33, 1); // enable_pin, step_pin, dir_pin, dir
  step_r.setAcc(1);
  step_r.setSpeed(0);
}

void motorSetSpeed(int32_t left, int32_t right)
{
  step_l.setSpeed(left);
  step_r.setSpeed(right);
}

void bleInit(const char *device_name)
{
  // Create the BLE Device
  BLEDevice::init(device_name);
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
}

void bleSetService(const char *service_uuid, BLEServiceCharConfig *char_list)
{
  // Create the BLE Service
  BLEService *pService = pServer->createService(service_uuid);
  
  // Create a BLE Characteristic
  uint32_t cnt = 0;
  BLECharacteristic *pCharacteristic;
  BLEServiceCharConfig *pCharConfig = &char_list[cnt];

  while(pCharConfig->uuid != NULL)
  {
    pCharacteristic = pService->createCharacteristic(
        pCharConfig->uuid,
        pCharConfig->properties);
  
    pCharacteristic->setCallbacks(pCharConfig->callback);      
    pCharacteristic->addDescriptor(new BLE2902());

    pCharConfig = &char_list[++cnt];
  }

  if(cnt > 0)
  {
    // Start the service
    pService->start();
  
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(service_uuid);    
  }
}

void bleMaintain(void)
{
  static uint32_t disconnected_time = millis();

  if (deviceConnected)
  {
    disconnected_time = millis();
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    if (millis() - disconnected_time > 500) // give the bluetooth stack the chance to get things ready
    {
      disconnected_time = millis();
      DEBUG_PRINTLN("restart advertising");
      pServer->startAdvertising(); // restart advertising
      oldDeviceConnected = deviceConnected;
    }
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}

BLECharacteristic* bleGetServiceCharacteristic(const char* service_uuid, const char* char_uuid)
{
  return pServer->getServiceByUUID(service_uuid)->getCharacteristic(char_uuid);
}
