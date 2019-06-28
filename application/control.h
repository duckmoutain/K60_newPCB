#ifndef __CONTROL_H
#define __CONTROL_H
#include <stdint.h>


typedef struct
{
	float err;
	float err_1;
	float target;
	float kp;
	float ki;
	float kd;
	float integral;
	float out;
}pid_struct;

extern pid_struct pidv;
extern pid_struct pidd;
extern uint8_t motor_run_flag;

extern uint8_t bluetooth_control_flag;
extern uint8_t top_computer_con ;
extern	int e_speed;
extern	uint8_t e_direct ;

void control_init(void);

#endif
