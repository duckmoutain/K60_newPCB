#ifndef __COMMUNICATE_H
#define __COMMUNICATE_H
#include <stdint.h>

extern uint16_t motor_recv_pwm;
extern uint16_t servo_recv_pwm;
extern uint8_t pwm_get_ok;

extern int16_t yaw_recv;
extern int16_t pitch_recv;
extern int16_t roll_recv;
extern int16_t x_acc_recv;
extern int16_t y_acc_recv;
extern int16_t z_acc_recv;

extern int16_t x_gyro_recv;
extern int16_t y_gyro_recv;
extern int16_t z_gyro_recv;
extern uint8_t imu_get_ok;
extern int bluetooth_rx_dat;


void bluetooth_init(void);
void uart5_comm_init(void);
void uart1_comm_init(void);
void imu_init(void);


#endif

