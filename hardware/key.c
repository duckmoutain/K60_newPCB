#include "key.h"
#include "gpio.h"
#include "pit.h"

uint8_t time_up_flag;

void pit_key_isr(void)
{
	time_up_flag = 1;
	PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, DISABLE);
}


void key_init(void)
{
	GPIO_QuickInit(HW_GPIOA, 19, kGPIO_Mode_IPU);			//KEY_I_LEFT
	GPIO_QuickInit(HW_GPIOA, 29, kGPIO_Mode_IPU);			//KEY_I_RIGHT
	
	GPIO_QuickInit(HW_GPIOA, 28, kGPIO_Mode_IPU);			//KEY_5_UP
	GPIO_QuickInit(HW_GPIOA, 26, kGPIO_Mode_IPU);			//KEY_5_DOWN
	GPIO_QuickInit(HW_GPIOA, 25, kGPIO_Mode_IPU);			//KEY_5_LEFT
	GPIO_QuickInit(HW_GPIOA, 27, kGPIO_Mode_IPU);			//KEY_5_RIGHT
	GPIO_QuickInit(HW_GPIOA, 24, kGPIO_Mode_IPU);			//KEY_5_MIDDLE
	
	/*--PIT 200ms--*/
	PIT_QuickInit(HW_PIT_CH1, 250 * 1000);		
	PIT_CallbackInstall(HW_PIT_CH1,pit_key_isr);
	time_up_flag = 0;
}


int key_scan(void)
{
	if(( KEY_I_LEFT == 0 || KEY_I_RIGHT == 0 || KEY_5_UP == 0 || KEY_5_DOWN == 0 ||  
		KEY_5_LEFT == 0 || KEY_5_RIGHT == 0 || KEY_5_MIDDLE == 0 )){
			
		/* delay to eliminate random jitter of key */		
		if(time_up_flag == 0){
				PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, ENABLE); 
				return -1;
		}
		else if(time_up_flag == 1){
			time_up_flag = 0;
			if	   (KEY_I_LEFT==0)		return 1;
			else if(KEY_I_RIGHT==0)		return 3;
			else if(KEY_5_UP==0)		return 8;
			else if(KEY_5_DOWN==0)		return 2;
			else if(KEY_5_LEFT==0)		return 4;
			else if(KEY_5_RIGHT==0)		return 6;
			else if(KEY_5_MIDDLE==0)	return 5;
		}
	}
	else time_up_flag = 0;
	return -1;
}

