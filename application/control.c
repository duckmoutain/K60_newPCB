#include "control.h"
#include "pit.h"
#include "motor.h"
#include "communicate.h"
#include "ftm.h"
pid_struct pidv;
pid_struct pidd;


int servo_out = 0;
int motor_out = 0;
int e_speed = 0;
uint8_t e_direct = 0;
uint8_t last_edirect = 0;
int isr_num = 0;
uint8_t motor_run_flag = 0;

uint8_t bluetooth_control_flag = 1;
uint8_t top_computer_con = 0;


void bluetooth_control(void);


void dir_control()
{
	pidd.err_1 = pidd.err;
	pidd.err   = servo_recv_pwm - 1500;
	pidd.out   = pidd.kp/10*pidd.err + pidd.kd/10*(pidd.err - pidd.err_1);
	if(top_computer_con)
	servo_out  = pidd.out;
}


void v_control()
{
	pidv.err_1 = pidv.err;
	pidv.err   = motor_recv_pwm - 1500; 
	
	pidv.out   = pidv.kp/10.0*pidv.err + pidv.kd/10.0*(pidv.err - pidv.err_1);
	if(top_computer_con)
	motor_out = pidv.out;
}


void protect_num()
{
	if(servo_out>(SERVO_PWM_MAX-SERVO_PWM_MID))
		servo_out=(SERVO_PWM_MAX-SERVO_PWM_MID);
	if(servo_out<(SERVO_PWM_MIN-SERVO_PWM_MID))
		servo_out=(SERVO_PWM_MIN-SERVO_PWM_MID);
	
	if(motor_out>(MOTOR_PWM_MAX-MOTOR_PWM_MID))
		motor_out=(MOTOR_PWM_MAX-MOTOR_PWM_MID);
	if(motor_out<(MOTOR_PWM_MIN-MOTOR_PWM_MID))
		motor_out=(MOTOR_PWM_MIN-MOTOR_PWM_MID);
}


void pit_control_isr(void)
{
	if(bluetooth_control_flag)
	{
		bluetooth_control();
	}
	
	dir_control();
	v_control();
	protect_num();
	isr_num ++;
	if(isr_num == 20)
	{
		FTM_QD_GetData(HW_FTM2, &e_speed, &e_direct);
		if(!e_direct && e_speed != 0 && e_direct + last_edirect != 1)e_speed = -(65535-e_speed);
		e_speed = e_speed *(500.0/240.0)/0.1;
		FTM_QD_ClearCount(HW_FTM2);
		isr_num = 0;
	}
	
	servo_set_pwm(SERVO_PWM_MID + servo_out);
	motor_set_pwm(MOTOR_PWM_MID + motor_out);
}


void bluetooth_control(void)
{
	switch(bluetooth_rx_dat)
	{
		case 2:
			bluetooth_rx_dat = -1;
			motor_out -= 25;
			bluetooth_rx_dat = -1;
			break;
		case 4:
			servo_out -= 75;
			bluetooth_rx_dat = -1;
			break;
		case 5:
			bluetooth_rx_dat = -1;
			motor_run_flag = !motor_run_flag;
			motor_out = 0;
			servo_out = 0;
			bluetooth_rx_dat = -1;
			break;
		case 6:
			servo_out += 75;
			bluetooth_rx_dat = -1;
			break;
		case 8:
			motor_out += 25;
			bluetooth_rx_dat = -1;
			break;
		case 9:
			bluetooth_rx_dat = -1;
			top_computer_con = !top_computer_con;
			bluetooth_rx_dat = -1;
		break;

		default: 
			bluetooth_rx_dat = -1;
			break;
	}
}


void control_init(void)
{
	/* pid control timer 5ms */
	PIT_QuickInit(HW_PIT_CH2, 5 * 1000);		
	PIT_CallbackInstall(HW_PIT_CH2,pit_control_isr);
	PIT_ITDMAConfig(HW_PIT_CH2, kPIT_IT_TOF, ENABLE);
	
	pidd.err   = 0;
	pidd.err_1 = 0;
	pidd.kp    = 10;
	pidd.kd    = 10;
	
	pidv.err   = 0;
	pidv.err_1 = 0;
	pidv.kp    = 10;
	pidv.kd    = 10;
	
}
