#include "stepmotor.h"
#include "steptbl.h"
#include "steptimer.h"

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

void IRAM_ATTR onTimerISR(uint8_t num)
{
	class StepMotor *p_class = p_stepmotor[num];	

	p_class->onTimer();
}

void IRAM_ATTR StepMotor::onTimer(void)
{	
	uint64_t alarm_value;

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

  	steptimerAlarmWrite(timer, m_TimeTable[abs(cur_speed)], true);
		

		if (cur_speed < target_speed)
		{
			cur_speed += target_acc;
			if (cur_speed > target_speed)
			{
				cur_speed = target_speed;
			}
		}
		else if (cur_speed > target_speed)
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


		if (total_step > 0)
		{									
			if (profile.count_index < profile.count_length)
			{
				if (profile.step_index == profile.target_index[profile.count_index])
				{
					uint8_t index;

					index  = profile.count_index + 1;
					index %= profile.count_length;

					target_speed = profile.target_speed[index];
					profile.count_index++;
				}				
			}

			if (profile.count_index >= profile.count_length)
			{
				cur_speed    = 0;
				target_speed = 0;
				total_step   = 0;
			}		

			if (cur_speed != 0)
			{
				profile.step_index++;				
			}
		}		
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

	total_step = 0;
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

	if (total_step == 0)
	{
		target_speed = value;
	}
}

void StepMotor::stop(void)
{	
	digitalWrite(pin_enable, HIGH);
	step_edge = 0;	
	target_speed = 0;
	cur_speed = 0;
	total_step = 0;
}

void StepMotor::start(void)
{
	digitalWrite(pin_enable, LOW);
	step_edge = 0;
}

void StepMotor::setStep(int32_t value, int32_t max_speed, bool accel)
{
	int32_t goal_step;
	int32_t acc_step;
	int32_t mov_step;
	int32_t dec_step;
	int32_t acc_speed;


	if (cur_speed != 0 || target_speed != 0)
	{
		return;
	}

	if (total_step == 0)
	{
		max_speed = constrain(abs(max_speed), 0, MOTOR_STEP_TABLE_MAX);				
		if (max_speed == 0)
		{
			max_speed = MOTOR_STEP_TABLE_MAX;
		}

		goal_step = abs(value);

		profile.count_index = 0;

		if(max_speed * 2 <= goal_step)
		{
			if (goal_step >= 2 )
			{
				acc_step = max_speed / target_acc;
				dec_step = max_speed / target_acc;
				mov_step = goal_step - (acc_step + dec_step);
				acc_speed = max_speed;
			}
			else
			{
				acc_step = 0;
				dec_step = 0;
				mov_step = goal_step;
				acc_speed = max_speed;
			}
		}
		else
		{
			acc_step = goal_step / 2 / target_acc;
			dec_step = goal_step / 2 / target_acc;
			mov_step = goal_step - (acc_step + dec_step);

			acc_speed = goal_step / 2;
		}

		profile.count_length = 0;

		if (acc_step > 0)
		{
			profile.target_speed[profile.count_length] = constrain(value, -acc_speed, acc_speed);
			profile.target_index[profile.count_length] = acc_step;
			profile.count_length++;
		}
		if (mov_step > 0)
		{
			profile.target_speed[profile.count_length] = constrain(value, -acc_speed, acc_speed);
			profile.target_index[profile.count_length] = acc_step + mov_step;
			profile.count_length++;
		}
		if (dec_step > 0)
		{
			profile.target_speed[profile.count_length] = 0;
			profile.target_index[profile.count_length] = goal_step;
			profile.count_length++;
		}
		

		if (accel == true)
		{
			profile.step_index = 0;
			total_step = goal_step;
			target_speed = profile.target_speed[0];
		}
		else
		{
			profile.target_speed[0] = constrain(value, -max_speed, max_speed);
			profile.target_index[0] = goal_step;
			profile.count_length = 1;			
			profile.step_index = 0;
			total_step = goal_step;
			cur_speed = profile.target_speed[0];
			target_speed = profile.target_speed[0];
		}
	/*
		Serial.println(total_step);
		Serial.println(target_speed);
		Serial.println(acc_step);
		Serial.println(mov_step);
		Serial.println(dec_step);		
		Serial.println(profile.target_index[0]);
		Serial.println(profile.target_index[1]);
		Serial.println(profile.target_index[2]);
	*/		
	}
	else
	{
		return;
	}
}

bool StepMotor::isBusy(void)
{
	if (total_step > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}



