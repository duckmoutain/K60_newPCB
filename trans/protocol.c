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

/* Includes ------------------------------------------------------------------*/
#include "oled.h"
#include "uart.h"
#include "protocol.h"
#include "protocol_transmit.h"
#include "protocol_cfg.h"
#include "common.h"
//#include "protocol_log.h"
//#include "UART1.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//struct chassis_info info;
//struct uwb_data uwb_data1;


struct manifold_cmd manifold_cmd;
struct uwb_data uwb_data;
struct cmd_chassis_info cmd_chassis_info;


boardcast_object_t boardcast_object;
local_info_t protocol_local_info =
    {
        .is_valid = 0};

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/



struct send_cmd_info *protocol_get_send_cmd_info(uint16_t cmd)
{
  for (int i = 0; i < PROTOCOL_CMD_MAX_NUM; i++)
  {
    if ((protocol_local_info.send_cmd_info[i].cmd == cmd) && (protocol_local_info.send_cmd_info[i].used == 1))
    {
      return &protocol_local_info.send_cmd_info[i];
    }
  }
  return NULL;
}

static void protocol_rcv_pack_handle(uint8_t *pack_data, uint16_t cmd, uint8_t session, uint8_t source_add)
{
  protocol_pack_desc_t *pack = NULL;
  uint16_t rcv_seq;
  int32_t err;
  rcv_seq = pack->seq_num;
  pack = (protocol_pack_desc_t *)(pack_data);

  for (int i = 0; i < PROTOCOL_CMD_MAX_NUM; i++)
  {
    if ((protocol_local_info.rcv_cmd_info[i].cmd == cmd) && (protocol_local_info.rcv_cmd_info[i].rcv_callback != NULL) && (protocol_local_info.rcv_cmd_info[i].used == 1))
    {
      // printf("111111");
      // printf("pack->data_len:%d",pack->data_len);
      err = protocol_local_info.rcv_cmd_info[i].rcv_callback(pack->pdata + 2, pack->data_len - PACK_HEADER_TAIL_LEN);
      // printf("%d",err);
      if (session != 0)
      {
        protocol_ack(source_add, session, &err, sizeof(err), rcv_seq);
      }
    }
  }

  return;
}

int32_t protocol_rcv_cmd_register(uint16_t cmd, rcv_handle_fn_t rcv_callback)
{
  for (int i = 0; i < PROTOCOL_CMD_MAX_NUM; i++)
  {
    if (protocol_local_info.rcv_cmd_info[i].used == 0)
    {
      protocol_local_info.rcv_cmd_info[i].used = 1;
      protocol_local_info.rcv_cmd_info[i].cmd = cmd;
      protocol_local_info.rcv_cmd_info[i].rcv_callback = rcv_callback;
      return 0;
    }
  }
//  PROTOCOL_ERR_INFO_PRINTF(PROTOCOL_ERR_REGISTER_FAILED, __FILE__, __LINE__);
  // printf("PROTOCOL_ERR_REGISTER_FAILED_rcv1\n");
  return -1;
}

int32_t protocol_send_cmd_config(uint16_t cmd,
                                 uint8_t resend_times,
                                 uint16_t resend_timeout,
                                 uint8_t ack_enable,
                                 ack_handle_fn_t ack_callback,
                                 no_ack_handle_fn_t no_ack_callback)
{
  for (int i = 0; i < PROTOCOL_CMD_MAX_NUM; i++)
  {
    if (protocol_local_info.send_cmd_info[i].used == 0)
    {
      protocol_local_info.send_cmd_info[i].used = 1;
      protocol_local_info.send_cmd_info[i].cmd = cmd;
      protocol_local_info.send_cmd_info[i].resend_times = resend_times;
      protocol_local_info.send_cmd_info[i].resend_timeout = resend_timeout;
      protocol_local_info.send_cmd_info[i].ack_enable = ack_enable;
      protocol_local_info.send_cmd_info[i].ack_callback = ack_callback;
      protocol_local_info.send_cmd_info[i].no_ack_callback = no_ack_callback;
      return 0;
    }
  }
//  PROTOCOL_ERR_INFO_PRINTF(PROTOCOL_ERR_REGISTER_FAILED, __FILE__, __LINE__);
  // printf("PROTOCOL_ERR_REGISTER_FAILED_send_cmd2\n");
  return -1;
}

int32_t protocol_rcv_cmd_unregister(uint16_t cmd)
{
  for (int i = 0; i < PROTOCOL_CMD_MAX_NUM; i++)
  {
    if ((protocol_local_info.rcv_cmd_info[i].cmd == cmd) && (protocol_local_info.rcv_cmd_info[i].used == 1))
    {
      protocol_local_info.rcv_cmd_info[i].used = 0;
      return 0;
    }
  }
  return -1;
}

int32_t protocol_send_cmd_unregister(uint16_t cmd)
{
  for (int i = 0; i < PROTOCOL_CMD_MAX_NUM; i++)
  {
    if ((protocol_local_info.send_cmd_info[i].cmd == cmd) && (protocol_local_info.send_cmd_info[i].used == 1))
    {
      protocol_local_info.send_cmd_info[i].used = 0;
      return 0;
    }
  }
  return -1;
}

/**
  * @brief  协议本地信息初始化函数
  * @param  address	协议本地地址，初始化以后不可改变，相同网络中每一个设备独占一个地址
  * @retval 协议返回状态
  */
uint32_t protocol_local_init(uint8_t address)
{

  uint32_t status;

  status = PROTOCOL_SUCCESS;

  //判断是否是小端机器
  const uint16_t endian_test = 0xAABB;
  if (*((uint8_t *)(&endian_test)) == 0xAA)
  {
    //是大端模式
    status = PROTOCOL_ERR_UNSUPPORT_CPU;
//    PROTOCOL_ERR_INFO_PRINTF(status, __FILE__, __LINE__);
	  // printf("PROTOCOL_ERR_UNSUPPORT_CPU3\n");

    while (1)
      ;
  }

  MUTEX_INIT(protocol_local_info.mutex_lock);

  memset(protocol_local_info.route_table, 0xFF, PROTOCOL_ROUTE_TABLE_MAX_NUM);
  memset(protocol_local_info.interface, 0, sizeof(protocol_local_info.interface));
  memset(&boardcast_object, 0, sizeof(boardcast_object_t));
  memset(&protocol_local_info.send_cmd_info, 0, sizeof(protocol_local_info.send_cmd_info));
  memset(&protocol_local_info.rcv_cmd_info, 0, sizeof(protocol_local_info.rcv_cmd_info));

  protocol_local_info.address = address;
  protocol_local_info.rcv_nor_callBack = protocol_rcv_pack_handle;

  MUTEX_INIT(boardcast_object.mutex_lock);
  INIT_LIST_HEAD(&boardcast_object.send_list_header);
  boardcast_object.is_valid = 1;
  protocol_local_info.is_valid = 1;
//  PROTOCOL_OTHER_INFO_PRINTF("Local info has been initialized.");
	// printf("Local info has been initialized4\n");
  return status;
}

/**
  * @brief  协议发送正常帧。
  * @param  reciver 接收设备地址
  *         session 会话号，范围为0~63，当session为0时不要求接收方Ack，否则要求Ack。同一时间内一个接收设备不能有两个
  *         相同的会话号
  *         cmd 命令值
  *         p_data 发送数据指针
  *         data_len 发送数据长度
  * @retval 协议返回状态
  */
uint32_t protocol_send(uint8_t reciver, uint16_t cmd, void *p_data, uint32_t data_len)
{
  uint32_t status;
  uint8_t session = 1;
  uint8_t ack = 1;

//  struct send_cmd_info *cmd_info;
//  cmd_info = protocol_get_send_cmd_info(cmd);

  struct perph_interface *int_obj;
  int_obj = protocol_s_get_route(reciver);

//  if (cmd_info != NULL)
//  {
//    ack = cmd_info->ack_enable;
//  }
  //cmd_info->ack_enable = 1;
  ack = 0;

  if (reciver == PROTOCOL_BROADCAST_ADDR)
  {
    status = protocol_s_broadcast_add_node(p_data, data_len, cmd);
  }
  else
  {
    if (ack == 1)
    {
      session = protocol_get_session(int_obj);
    }
    status = protocol_s_add_sendnode(reciver, session, PROTOCOL_PACK_NOR, p_data,
                                     data_len, cmd, 0);
  }

  if (status == PROTOCOL_SUCCESS)
  {
    if (protocol_local_info.send_list_add_callBack != NULL)
    {
      protocol_local_info.send_list_add_callBack();
    }
  }
  else
  {
    if (ack == 1)
    {
      protocol_release_session(int_obj, session);
    }
  }
  return status;
}

/**
  * @brief  协议发送Ack帧，当收到Session不为0的正常帧以后，需要调用此函数回复Ack。
  * @param  reciver 接收设备地址
  *         session 会话号，范围为0~63，当session为0时不要求接收方Ack，否则要求Ack。同一时间内一个接收设备不能有两个
  *         相同的会话号
  *         p_data 发送数据指针
  *         data_len 发送数据长度
  *         ack_seq	发送Ack包的序列号
  * @retval 协议返回状态
  */
uint32_t protocol_ack(uint8_t reciver, uint8_t session, void *p_data, uint32_t data_len, uint16_t ack_seq)
{
  uint32_t status;
  status = protocol_s_add_sendnode(reciver, session, PROTOCOL_PACK_ACK, p_data,
                                   data_len, 0, ack_seq);
  if (status == PROTOCOL_SUCCESS)
  {
    if (protocol_local_info.send_list_add_callBack != NULL)
    {
      protocol_local_info.send_list_add_callBack();
    }
  }
  return status;
}

/**
  * @brief  刷新发送列表，调用此函数将发送列表中的数据发送。在调用protocol_send，protocol_ack后调用或者定时调用
  * @param  void
  * @retval 协议返回状态
  */
uint32_t protocol_send_flush(void)
{
  for (uint8_t i = 0; i < PROTOCOL_INTERFACE_MAX; i++)
  {
    if (protocol_local_info.interface[i].is_valid)
    {
      if (protocol_local_info.interface[i].send.normal_node_num > 0)
      {
        protocol_s_interface_normal_send_flush(protocol_local_info.interface + i);
      }
      if (protocol_local_info.interface[i].send.ack_node_num > 0)
      {
        protocol_s_interface_ack_send_flush(protocol_local_info.interface + i);
      }
    }
  }

  if (boardcast_object.is_valid)
  {
    if (boardcast_object.send_node_num > 0)
    {
      protocol_s_broadcast_send_flush();
    }
  }
  return 0;
}

/**
  * @brief  协议刷新接收缓冲区，调用此函数将接收缓冲区内的数据解包。在接收数据之后或者定时调用
  * @param  void
  * @retval 协议返回状态
  */
uint32_t protocol_unpack_flush(void)
{

  //messege_flag = 100;
  for (uint8_t i = 0; i < PROTOCOL_INTERFACE_MAX; i++)
  {
    if (protocol_local_info.interface[i].is_valid)
    {
		//messege_flag = 100;
      protocol_s_extract(&(protocol_local_info.interface[i]));
    }
  }
  return 0;

}

/**
  * @brief  协议接收数据，在接收到数据时使用，如串口中断函数等
  * @param  p_data 接收到数据指针
  *         data_len 数据长度
  *         interface 接口序号，填写接收到数据的接口的序列号
  * @retval 协议返回状态
  */
uint32_t protocol_rcv_data(void *p_data, uint32_t data_len, struct perph_interface *perph)
{
  FIFO_CPU_SR_TYPE cpu_sr;
  struct perph_interface *obj;
  uint32_t rcv_length;
  uint32_t status;

  //Interrupt Off;
  cpu_sr = FIFO_GET_CPU_SR();
  FIFO_ENTER_CRITICAL();

  status = PROTOCOL_SUCCESS;

  if (protocol_local_info.is_valid == 0)
  {
    status = PROTOCOL_ERR_PROTOCOL_NOT_INIT;
    //Interrupt On
    FIFO_RESTORE_CPU_SR(cpu_sr);
    return status;
  }

  obj = &(protocol_local_info.interface[perph->idx]);

  //TODO:取消了这里的保护，因为考虑到此函数对于同一个协议接口没有重入，需要仔细思考
  //添加保护，高速传输仍然可能出现嵌套重入
  rcv_length = fifo_s_puts_noprotect(&(obj->rcvd.fifo), p_data, data_len);

  if (rcv_length < data_len)
  {
    status = PROTOCOL_ERR_FIFO_FULL;
//    PROTOCOL_ERR_INFO_PRINTF(status, __FILE__, __LINE__);
	  // printf("PROTOCOL_ERR_FIFO_FULL5\n");
  }
  //Interrupt On
  FIFO_RESTORE_CPU_SR(cpu_sr);
  return status;
}

/**
  * @brief  协议注册包发送加入列表回调函数，当协议数据包打包好添加至发送列表后，调用本函数注册的处理回调函数。
  * @param  fn 回调处理函数指针，格式参见pack_handle_fn_t
  * @retval 0
  */
uint32_t protocol_send_list_add_callback_reg(void_fn_t fn)
{
  protocol_local_info.send_list_add_callBack = fn;
  return 0;
}


int32_t chassis_speed_ctrl(uint8_t *buff, uint16_t len)
{
  // printf("aaaaa%d \n",len);
  // printf("bbbbbb%d \n",sizeof(struct cmd_chassis_speed));
  if (len == sizeof(struct cmd_chassis_speed))
  {
    // printf("ccccc%d \n",len);
    //messege_flag = 100;
    memcpy(&manifold_cmd.chassis_speed, buff, len);
    // printf("123124324532%d \n",len);

    //osSignalSet(cmd_task_t, MANIFOLD2_CHASSIS_SIGNAL);
  }
  return 0;
}

int32_t chassis_spd_acc_ctrl(uint8_t *buff, uint16_t len)
{
  if (len == sizeof(struct cmd_chassis_spd_acc))
  {
    memcpy(&manifold_cmd.chassis_spd_acc, buff, len);
    //osSignalSet(cmd_task_t, MANIFOLD2_CHASSIS_ACC_SIGNAL);
  }
  return 0;
}

int32_t usb_rcv_callback(uint8_t *buf, uint32_t len)
{
  protocol_uart_rcv_data(PROTOCOL_USB_PORT, buf, len);
  return len;
}

/**
 * @NOTE: Uart发送基础
*/
int32_t Send_data(uint8_t *s, uint32_t len)
{
	int i = 0;
	while(i != len)
	{
    UART_WriteByte(HW_UART0,*s);
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
		//USART_SendData(USART1,*s);
		s++;
		i++;
	}
	return 0;
}

void protocol_send_success_callback(void)
{
  //flag =1;
}


int32_t manifold2_heart_package(uint8_t *buff, uint16_t len)
{
  return 0;
}

int32_t report_firmware_version(uint8_t *buff, uint16_t len)
{
  return FIRMWARE_VERSION;
}



