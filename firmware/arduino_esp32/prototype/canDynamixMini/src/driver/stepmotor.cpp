#include "stepmotor.h"
#include "steptbl.h"


#define MOTOR_TIMER_HZ				1000000
#define MOTOR_START_SPEED   	(MOTOR_TIMER_HZ/(10))
#define MOTOR_STEP_IDLE				0
#define MOTOR_STEP_WORK				1
#define MOTOR_STEP_TABLE_MAX	300 


void onTimerISR(uint8_t num);


void IRAM_ATTR onTimer1() { onTimerISR(0); }
void IRAM_ATTR onTimer2() { onTimerISR(1); }
void IRAM_ATTR onTimer3() { onTimerISR(2); }
void IRAM_ATTR onTimer4() { onTimerISR(3); }


static class StepMotor *p_stepmotor[4];
void (*timer_func_ptr[4])() =
{
	onTimer1,
	onTimer2,
	onTimer3,
	onTimer4
};


void onTimerISR(uint8_t num)
{
	class StepMotor *p_class = p_stepmotor[num];	

	p_class->onTimer();
}

void StepMotor::onTimer(void)
{	
	step_edge ^= 1;


	if (step_edge == 1)
	{
		if (cur_speed == 0)
		{
			digitalWrite(pin_step, LOW);
		}
		else
		{
			digitalWrite(pin_step, HIGH);
		}

  	timerAlarmWrite(timer, m_TimeTable[abs(cur_speed)], true);


		if (cur_speed < target_speed)
		{
			cur_speed += target_acc;
			if (cur_speed > target_speed)
			{
				cur_speed = target_speed;
			}
		}
		else
		{
			cur_speed -= target_acc;
			if (cur_speed < target_speed)
			{
				cur_speed = target_speed;
			}
		}
	}
	else
	{
		digitalWrite(pin_step, LOW);

		if (cur_speed == 0 && target_speed == 0)
		{
			digitalWrite(pin_enable, HIGH);
		}
		else
		{
			digitalWrite(pin_enable, LOW);
		}

		if (cur_speed > 0)
		{
			cur_dir = 1;			
			step_count++;
		}
		else if (cur_speed < 0)
		{
			cur_dir = -1;			
			step_count--;
		}

		cur_dir = cur_dir * cfg_dir;
		if (cur_dir > 0)
		{
			digitalWrite(pin_dir, HIGH);			
		}
		else
		{
			digitalWrite(pin_dir, LOW);			
		}		

		pre_speed = cur_speed;
	}
		
}

StepMotor::StepMotor(uint8_t timer_num)
{
  cur_speed = 0;
  target_speed = 0;

  pin_enable = -1;
  pin_step = -1;
  pin_dir = -1;

  cfg_dir = 1;	
	timer_number = timer_num;
	p_stepmotor[timer_num] = this;

	step_res = 2000;
	step_count = 0;
	target_acc = 1;
	step_edge = 0;
}

StepMotor::~StepMotor()
{
	
}

void StepMotor::begin(int8_t enable_pin, int8_t step_pin, int8_t dir_pin, int8_t dir)
{		
	pin_enable = enable_pin;
  pin_step = step_pin;
  pin_dir = dir_pin;
	cfg_dir = dir;

	pinMode(pin_enable, OUTPUT);
	pinMode(pin_step, OUTPUT);
	pinMode(pin_dir, OUTPUT);

	digitalWrite(pin_enable, HIGH);
	digitalWrite(pin_step, LOW);
	digitalWrite(pin_dir, LOW);


	step_state = MOTOR_STEP_IDLE;

  timer = timerBegin(timer_number, 80, true);
  timerAttachInterrupt(timer, timer_func_ptr[timer_number], true);
	timerAlarmWrite(timer, m_TimeTable[0], true);
	timerAlarmEnable(timer);     
}

void StepMotor::setAcc(int32_t value)
{
	target_acc = value;
}

void StepMotor::setSpeed(int32_t value)
{
	value = constrain(value, -MOTOR_STEP_TABLE_MAX, MOTOR_STEP_TABLE_MAX);

	target_speed = value;
}

void StepMotor::stop(void)
{	
	digitalWrite(pin_enable, HIGH);
	step_edge = 0;	
}

void StepMotor::start(void)
{
	digitalWrite(pin_enable, LOW);
	step_edge = 0;
}