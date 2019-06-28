/****************************************************************************
 *  Copyright (C) 2019 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

/* Includes ------------------------------------------------------------------*/
#include "protocol_common.h"
//#include "sys.h"
#include "common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------GERR-----------------------------------------*/

//各种结构体
//extern struct chassis_info info;
//extern struct uwb_data uwb_data1;

//extern struct cmd_gimbal_info cmd_gimbal_info;
//extern struct cmd_chassis_info cmd_chassis_info;
extern struct manifold_cmd manifold_cmd;
extern  struct uwb_data uwb_data;
extern struct cmd_chassis_info cmd_chassis_info;



struct cmd_chassis_param
{
  uint16_t wheel_perimeter; /* the perimeter(mm) of wheel */
  uint16_t wheel_track;     /* wheel track distance(mm) */
  uint16_t wheel_base;      /* wheelbase distance(mm) */
  int16_t rotate_x_offset;
  int16_t rotate_y_offset;
};

struct cmd_chassis_info
{
  int16_t gyro_angle;
  int16_t gyro_palstance;
  int32_t position_x_mm;
  int32_t position_y_mm;
  int16_t angle_deg;
  int16_t v_x_mm;
  int16_t v_y_mm;
};

struct cmd_gimbal_info
{
  uint8_t   mode;
  /* unit: degree */
  int16_t pitch_ecd_angle;
  int16_t yaw_ecd_angle;
  int16_t pitch_gyro_angle;
  int16_t yaw_gyro_angle;
  /* uint: degree/s */
  int16_t yaw_rate;
  int16_t pitch_rate;
};

struct cmd_gimbal_angle
{
  union{
    uint8_t flag;
    struct{
        uint8_t yaw_mode:1;  // 0 code angle
        uint8_t pitch_mode:1;
    }bit;
  } ctrl;
  int16_t pitch;
  int16_t yaw;
};

struct cmd_chassis_speed
{
  int16_t vx; // forward/back
  int16_t vy; // left/right
  int16_t vw; // anticlockwise/clockwise
  int16_t rotate_x_offset;
  int16_t rotate_y_offset;
};

struct cmd_chassis_spd_acc
{
  int16_t   vx; 
  int16_t   vy;
  int16_t   vw; 

  int16_t   ax; 
  int16_t   ay; 
  int16_t   wz; 
  
  int16_t rotate_x_offset;
  int16_t rotate_y_offset;
};

struct cmd_firction_speed
{
  uint16_t left;
  uint16_t right;
};

struct cmd_shoot_num
{
  uint8_t  shoot_cmd;
  uint32_t shoot_add_num;
  uint16_t shoot_freq;
};

struct uwb_data{
  int16_t   x;
  int16_t   y;
  uint16_t  yaw;
  int16_t   distance[6];
  uint16_t  error;
  uint16_t  res;
};



struct manifold_cmd
{
  struct cmd_gimbal_angle gimbal_angle;
  struct cmd_chassis_speed chassis_speed;
  struct cmd_chassis_spd_acc chassis_spd_acc;
  struct cmd_firction_speed firction_speed;
  struct cmd_shoot_num shoot_num; 
};








//extern local_info_t protocol_local_info;

int32_t protocol_send_cmd_config(uint16_t cmd,
                                 uint8_t resend_times,
                                 uint16_t resend_timeout,
                                 uint8_t ack_enable,
                                 ack_handle_fn_t ack_callback,
                                 no_ack_handle_fn_t no_ack_callback);

int32_t protocol_rcv_cmd_register(uint16_t cmd, rcv_handle_fn_t rcv_callback);

int32_t protocol_rcv_cmd_unregister(uint16_t cmd);
int32_t protocol_send_cmd_unregister(uint16_t cmd);

uint32_t protocol_local_init(uint8_t address);

uint32_t protocol_send(uint8_t reciver, uint16_t cmd, void *p_data, uint32_t data_len);

uint32_t protocol_ack(uint8_t reciver, uint8_t session, void *p_data,
                      uint32_t data_len, uint16_t ack_seq);

uint32_t protocol_send_flush(void);

uint32_t protocol_unpack_flush(void);

uint32_t protocol_rcv_data(void *p_data, uint32_t data_len, struct perph_interface *perph);

uint32_t protocol_send_list_add_callback_reg(void_fn_t fn);

int32_t chassis_speed_ctrl(uint8_t *buff, uint16_t len);

int32_t chassis_spd_acc_ctrl(uint8_t *buff, uint16_t len);

int32_t usb_rcv_callback(uint8_t *buf, uint32_t len);


int32_t Send_data(uint8_t *s, uint32_t len);

void protocol_send_success_callback(void);

int32_t manifold2_heart_package(uint8_t *buff, uint16_t len);
int32_t report_firmware_version(uint8_t *buff, uint16_t len);

#endif /* SUPPORT_PROTOCOL_H_ */
