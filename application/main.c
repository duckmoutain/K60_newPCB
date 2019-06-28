#include "communicate.h"
#include "menu.h"
#include "key.h"
#include "oled.h"
#include "motor.h"
#include "control.h"
#include "ftm.h"
#include "support.h"

int main()
{

	oled_init();
	key_init();
	bluetooth_init();
	uart5_comm_init();
	//imu_init();
    FTM_QD_QuickInit(FTM2_QD_PHA_PA10_PHB_PA11, kFTM_QD_NormalPolarity, kQD_CountDirectionEncoding);

	motion_init();
	control_init();

	real_protocol_init();
	 
	cmd_chassis_info.v_x_mm  = 100;
	
	while(1)
	{
		real_protocol_receive();
		real_protocol_send();
		menu_load(); 
	}
}

