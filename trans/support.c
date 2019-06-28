#include "support.h"

/********************************************************************************************************/
//???��???????
/********************************************************************************************************/
void real_protocol_init(void)
{
	//????????
	protocol_local_init(CHASSIS_ADDRESS);

	//????????
	/*Send_data?????????????*/
	protocol_uart_interface_register("manifold2", 4096, 1, PROTOCOL_USB_PORT, Send_data);

	//???��??
	protocol_set_route(MANIFOLD2_ADDRESS, "manifold2");

	//?????????
	protocol_rcv_cmd_register(CMD_MANIFOLD2_HEART, manifold2_heart_package);

	//???��???
	protocol_rcv_cmd_register(CMD_REPORT_VERSION, report_firmware_version);

	//?????????????
	protocol_rcv_cmd_register(CMD_SET_CHASSIS_SPEED, chassis_speed_ctrl);

	//??????????????
	protocol_rcv_cmd_register(CMD_SET_CHASSIS_SPD_ACC, chassis_spd_acc_ctrl);

	//???????????
	protocol_send_list_add_callback_reg(protocol_send_success_callback);
	//??????????
	//??????1
	//???????????????disable
	//USART_Cmd(USART1, ENABLE);
}


/********************************************************************************************************/
//???????
//??????????
//?????????void
//?�¦�?:                       uwb??????                     ???????
/********************************************************************************************************/
void real_protocol_send(void)
{
	protocol_send(MANIFOLD2_ADDRESS, CMD_PUSH_CHASSIS_INFO, &cmd_chassis_info, sizeof(cmd_chassis_info));
	//protocol_send(MANIFOLD2_ADDRESS, CMD_PUSH_UWB_INFO, &uwb_data, sizeof(uwb_data));
	protocol_send_flush();
}


/********************************************************************************************************/
//???????
//????????????????????
//?�¦�???void
/********************************************************************************************************/
void real_protocol_receive(void)
{
	//???????
	protocol_unpack_flush();

	//return &manifold_cmd;

}


