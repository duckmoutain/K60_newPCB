#include "menu.h"
#include "communicate.h"
#include "oled.h"
#include "key.h"
#include "motor.h"
#include "control.h"

#define MENU_oled_index_MAX	39				
int oled_index = 28;					

extern int servo_out;
extern int motor_out;

void menu_refresh()
{
	if(oled_index <= 7){
		/*------------------- page 0 ----------------------*/
		oled_display(0,115,"%d",oled_index);
		oled_display(0,6,"     ^_^    ");
		oled_display(3,6,"  blueCtrl    %d  ",bluetooth_control_flag);
		oled_display(4,6,"  yaw         %d  ",yaw_recv);
		oled_display(5,6,"  pitch       %d  ",pitch_recv);
		oled_display(6,6,"  roll        %d  ",roll_recv);
		oled_display(oled_index,0,">");
		
	}else if(oled_index >= 8 && oled_index <= 15){
		/*------------------- page 1 ----------------------*/
		oled_display(0,115,"%d",oled_index);
		oled_display(0,6," debug"); 
		oled_display(1,6,"servo pwm %d  ",servo_recv_pwm);
		oled_display(2,6,"motor pwm %d  ",motor_recv_pwm);
		oled_display(3,6,"servo_out %d  ",servo_out);
		oled_display(4,6,"motor_out %d  ",motor_out);
		oled_display(5,6,"top_con  %d  ",top_computer_con);
		oled_display(oled_index-8,0,">");
		
	}else if(oled_index >= 16 && oled_index <= 23){	
		/*------------------- page 2 ----------------------*/
		oled_display(0,115,"%d",oled_index);
		oled_display(0,6," dir_circle");
		oled_display(1,6,"error:   %d  ",(int)pidd.err);
		oled_display(2,6,"out:     %d  ",(int)pidd.out);
		oled_display(3,6,"kp:      %d  ",(int)pidd.kp);
		oled_display(4,6,"kd:      %d  ",(int)pidd.kd);
		oled_display(5,6,"run_flag:%d  ",motor_run_flag);
		
		
		oled_display(oled_index-16,0,">");
		
	}else if(oled_index >= 24 && oled_index <= 31){
		/*------------------- page 3 ----------------------*/
		oled_display(0,115,"%d",oled_index);
		oled_display(0,6," v_circle");
		oled_display(1,6,"error:   %d  ",(int)pidv.err);
		oled_display(2,6,"out:     %d  ",(int)pidv.out);
		oled_display(3,6,"kp:      %d  ",(int)pidv.kp);
		oled_display(4,6,"kd:      %d  ",(int)pidv.kd);
		oled_display(5,6,"espeed   %d      ",(int)e_speed);
		oled_display(oled_index-24,0,">");
	}else if(oled_index >= 32 && oled_index <= 39){
		/*-------------------�̨�??��3----------------------*/
		oled_display(0,115,"%d",oled_index);

		oled_display(oled_index-32,0,">");
	}
}



void menu_handle(uint8_t mode)
{
	switch(oled_index)
	{
		case 3: 
			if(mode) bluetooth_control_flag = !bluetooth_control_flag;
			else 	 bluetooth_control_flag = !bluetooth_control_flag;
			break;
		
		case 9: 
			if(mode) servo_out += 100;
			else 	 servo_out -= 100;
			break;
		
		case 14:
			if(mode) top_computer_con = !top_computer_con;
			else 	 top_computer_con = !top_computer_con;
			break;
		
		case 19:
			if(mode) pidd.kp += 1;
			else 	 pidd.kp -= 1;
			break;
		
		case 20:
			if(mode) pidd.kd += 1;
			else 	 pidd.kd -= 1;
			break;
		
		case 21:
			if(!motor_run_flag) motor_run_flag = 1 ;
			else motor_run_flag = 0;
		break;
		
		case 27:
			if(mode) pidv.kp += 1;
			else 	 pidv.kp -= 1;
			break;
		
		case 28:
			if(mode) pidv.kd += 1;
			else 	 pidv.kd -= 1;
			break;
		
		case 29:
			if(!motor_run_flag) motor_run_flag = 1 ;
			else motor_run_flag = 0;
		break;
		default: 
			break;
	}
}


void menu_load(void)
{	
	switch(key_scan())
	{
		case 9://I_LEFT
			if(oled_index >= MENU_oled_index_MAX-7 && oled_index <= MENU_oled_index_MAX) 	oled_index = 0;
			else 				    oled_index += 8;
			oled_clear_all(0x00);
			menu_refresh();
			break;
		case 7://I_RIGHT
			if(oled_index >= 0 && oled_index <= 7) 		oled_index = MENU_oled_index_MAX-7;
			else 				    oled_index -= 8;
			oled_clear_all(0x00);
			menu_refresh();
			break;
		case 8://UP
			if(oled_index <= 0) 	oled_index = MENU_oled_index_MAX;
			else 			oled_index -= 1;
			oled_clear_all(0x00);
			menu_refresh();
			break;
		case 2://DOWM
			if(oled_index >= MENU_oled_index_MAX) oled_index = 0;
			else 			oled_index += 1;
			oled_clear_all(0x00);
			menu_refresh();
			break;
		case 4://LEFT
			menu_handle(0);
			oled_clear_all(0x00);
			menu_refresh();
			break;
		case 6://RIGHET
			menu_handle(1);
			oled_clear_all(0x00);
			menu_refresh();
			break;
		case 5://MIDDLE
			oled_clear_all(0x00);
			menu_refresh();
			break;
		default: 
			menu_refresh();
			break;
	}
}
