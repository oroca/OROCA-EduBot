# Sketch_ble_app

OROCA_EduBot과 Bluetooth LE로 연결하기 위한 Sketch 프로젝트임. 개발환경 및 도구의 사용법은 OROCA 카페를 참고.

- [Arduino 개발환경 구축](https://cafe.naver.com/openrt/19738)
- [Arduino 라이브러리 설치](https://cafe.naver.com/openrt/19739)
- [Arduino 라이브러리 설치 #2](https://cafe.naver.com/openrt/19772)


# 프로젝트 실행 

위에서 설치한 Arduino IDE를 이용하여 프로젝트를 열고, 빌드 & 업로드하여 실행한다.


# Services/Characteristics


## Motor Service (uuid: 34443c32-3356-11e9-b210-d663bd873d93)

- MOTOR_CHARACTERISTIC_SET_STEP_UUID

Property: Write

Value: Hex Array (10 bytes)

| l_step (4 bytes) - int32_t | r_step (4 bytes) - int32_t | max_vel (2 bytes) - int16_t |

Range: 

    l_step: (-2,147,483,648 ~ 2,147,483,647)
    r_step: (-2,147,483,648 ~ 2,147,483,647)
    max_vel: (-300 ~ 300)

- MOTOR_CHARACTERISTIC_SET_SPEED_UUID

Property: Write

Value: Hex Array (6 bytes)

| l_vel (2 bytes) - int16_t | r_vel (2 bytes) - int16_t | delay_ms (2 bytes) - int16_t |

Range:

    l_vel: (-32,768 ~ 32,767)
    r_vel: (-32,768 ~ 32,767)
    delay_ms: 0 ~ 65,535


- MOTOR_CHARACTERISTIC_SET_DISTANCE_UUID

Property: Write

Value: Hex Array (10 bytes)

| l_distance (4 bytes) - int32_t | r_distance (4 bytes) - int32_t | max_vel (2 bytes) - int16_t |

Scale: (mm)

Range:

    l_step: (-2,147,483,648 ~ 2,147,483,647)
    r_step: (-2,147,483,648 ~ 2,147,483,647)
    max_vel: (-300 ~ 300)

- MOTOR_CHARACTERISTIC_SET_ACCEL_UUID

Property: Read/Write

Value: Hex Array (4 bytes)

| l_acc (2 bytes) - uint16_t | r_acc (2 bytes) - uint16_t |

Range:

    l_acc: (1 ~ 4)
    r_acc: (1 ~ 4)

- MOTOR_CHARACTERISTIC_WAIT_RESULT_UUID

Property: Notify

Value: 1 byte

Deescription: setStep, setDistance의 작업이 완료되면 Notify.


## Misc. Service (uuid: 34443c38-3356-11e9-b210-d663bd873d93)

- MISC_CHARACTERISTIC_COLOR_LED_UUID

Property: Write

Value: Hex array (6 bytes)

| left_r (1 byte) | left_g (1 byte) | left_b (1 byte) | right_r (1 byte) | right_g (1 byte) | right_b (1 byte) |


- MISC_CHARACTERISTIC_PLAY_SOUND_UUID (미구현)

Property: Write


- MISC_CHARACTERISTIC_BUTTON_UUID

Property: Read/Notify

Value: 1 byte

Description: 유저 버튼 상태, 누를 경우 Notify.


- MISC_CHARACTERISTIC_SET_TEXT_OLED_UUID

Property: Write

Value: String


- MISC_CHARACTERISTIC_SET_IMAGE_OLED_UUID

Property: Write

Value: 1 byte

Description: 로봇 내부에 저장된 이미지 인덱스


## Sensor Service (uuid: 34443c44-3356-11e9-b210-d663bd873d93)

- SENSOR_CHARACTERISTIC_FLOOR_SENSORS_UUID

Property: Read/Notify

Value: 4 bytes 

| left_out | left_in | right_in | right_out |


- SENSOR_CHARACTERISTIC_DISTANCE_SENSOR_UUID

Property: Read/Notify

Value: 4 bytes

| left_dist - 2 byte | right_dist - 2 byte |


- SENSOR_CHARACTERISTIC_IMU_SENSOR_UUID

Property: Read/Notify

Value: 18 bytes

| roll | pitch | yaw | acc_x | acc_y | acc_z | gyro_x | gyro_y | gyro_z |

각각의 값은 int16_t 이며, 실제값에 100을 곱한 값. 따라서 수신된 값을 받아 100으로 나누어 사용함.


- SENSOR_CHARACTERISTIC_BATTERY_LEVEL_UUID

Property: Read/Notify

Value: 1 byte

배터리 전압, 실제 전압값에 10을 곱한 값. 배터리 전압이 3.2V 이하면 Notify
