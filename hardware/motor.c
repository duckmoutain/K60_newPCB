#include "motor.h"
#include "gpio.h"
#include "ftm.h"
#include "control.h"

void motion_init(void)
{
	FTM_PWM_QuickInit(FTM1_CH0_PB00,kPWM_EdgeAligned,330);
	motor_set_pwm(0);
	
	/* for servo*/
	FTM_PWM_QuickInit(FTM0_CH0_PC01,kPWM_EdgeAligned,50);
	servo_set_pwm(SERVO_PWM_MID);
}


void motor_set_pwm(uint16_t pwm)
{
	
	if(pwm > MOTOR_PWM_MAX)
		pwm = MOTOR_PWM_MAX;
	if(pwm < MOTOR_PWM_MIN)
		pwm = MOTOR_PWM_MIN;
	
	if(motor_run_flag)
	FTM_PWM_ChangeDuty(HW_FTM1,HW_FTM_CH0,pwm); 
	else
	FTM_PWM_ChangeDuty(HW_FTM1,HW_FTM_CH0,MOTOR_PWM_MID); 
}


void servo_set_pwm(uint16_t pwm)
{
	if(pwm > SERVO_PWM_MAX)
		pwm = SERVO_PWM_MAX;
	if(pwm < SERVO_PWM_MIN)
		pwm = SERVO_PWM_MIN;
	
  if(motor_run_flag)
	FTM_PWM_ChangeDuty(HW_FTM0,HW_FTM_CH0,pwm);
	else
	FTM_PWM_ChangeDuty(HW_FTM0,HW_FTM_CH0,SERVO_PWM_MID);
}


