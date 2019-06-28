#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

#define SERVO_PWM_MAX 1300
#define SERVO_PWM_MID 1000
#define SERVO_PWM_MIN 700
#define MOTOR_PWM_MID 5000
#define MOTOR_PWM_MAX 5500
#define MOTOR_PWM_MIN 4500


void motion_init(void);
void motor_set_pwm(uint16_t pwm);
void servo_set_pwm(uint16_t pwm);

#endif
