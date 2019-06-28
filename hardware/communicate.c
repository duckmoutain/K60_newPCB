#include "communicate.h"
#include "common.h"
#include "oled.h"
#include "uart.h"

int bluetooth_rx_dat;

uint16_t motor_recv_pwm;
uint16_t servo_recv_pwm;
uint8_t pwm_get_ok = 0;

int16_t yaw_recv;
int16_t pitch_recv;
int16_t roll_recv;

int16_t x_acc_recv;
int16_t y_acc_recv;
int16_t z_acc_recv;

int16_t x_gyro_recv;
int16_t y_gyro_recv;
int16_t z_gyro_recv;
uint8_t imu_get_ok;
void bluetooth_call_back(uint16_t rx_dat)
{
	bluetooth_rx_dat = rx_dat;
}
/*����λ��*/
void uart3_call_black(uint16_t rx_dat)
{
	static uint8_t i = 0, rebuf[20];
	
	rebuf[i] = rx_dat;
	
//	oled_printf("i %d\n",i);
	
	if(rebuf[0] != 0xAA)
	{
		i = 0;
		return ;
	}
	
	if(i >= 6)
	{
		if(rebuf[6] == 0x55)
		{
			uint8_t check = 0;
			for(int i=1;i<=4;i++)
			{
				check += rebuf[i];
				//oled_printf("0x%.2x ",rebuf[i]);
			}
			
			if(check != rebuf[5])
			{
				i = 0;
				return ;
			}
			else
			{
				//oled_printf("\ncheck %x\n",check);
				motor_recv_pwm = (uint16_t)(rebuf[2]<<8 | rebuf[1]);
				servo_recv_pwm = (uint16_t)(rebuf[4]<<8 | rebuf[3]);
				pwm_get_ok = 1;
				i = 0;
				return ;
			}
			
		}
		i = 0;
	}
	i++;
}
/*��IMU*/
void uart1_call_black(uint16_t rx_dat)
{
	static uint8_t i = 0, rebuf[40];
	
	rebuf[i] = rx_dat;

	if(i==0)
	{
		if(rebuf[0] != 0xA5)
		{
			return ;
		}
	}
	else
	{
		if(rebuf[0] != 0xA5 || rebuf[1] != 0x5A)
		{
			i = 0; 
			return ;
		}
	}

	
	if(i >= 39)
	{
		if(rebuf[39] == 0xAA)
		{
			uint8_t check = 0;
			for(int i=2;i<=37;i++)
			{
				check += rebuf[i];

			}
			if(check != rebuf[38])
			{
				i = 0;
				return ;
			}
			else
			{
				yaw_recv = ~((rebuf[3]<<8 | rebuf[4])-0x01);
				pitch_recv = ~((rebuf[5]<<8 | rebuf[6])-0x01);
				roll_recv = ~((rebuf[7]<<8 | rebuf[8])-0x01);
				
				x_acc_recv = (~((rebuf[9]<<8 | rebuf[10])-0x01))/16384.0;
				y_acc_recv = (~((rebuf[11]<<8 | rebuf[12])-0x01))/16384.0;
				z_acc_recv = (~((rebuf[13]<<8 | rebuf[14])-0x01))/16384.0;
				
				x_gyro_recv = (~((rebuf[15]<<8 | rebuf[16])-0x01))/32.8;
				y_gyro_recv = (~((rebuf[17]<<8 | rebuf[18])-0x01))/32.8;
				z_gyro_recv = (~((rebuf[19]<<8 | rebuf[20])-0x01))/32.8;
				
				
				imu_get_ok = 1;
				i = 0;
				return ;
			}
		}
		i = 0;
	}
	i++;
}

void bluetooth_init(void)
{
	UART_InitTypeDef UART_InitStruct1;     
    UART_InitStruct1.instance = HW_UART0;   
	UART_InitStruct1.baudrate = 9600;    
    UART_InitStruct1.parityMode = kUART_ParityDisabled;
    UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;   
    UART_Init(&UART_InitStruct1);
	
	/*Config the port's smultiplexing*/
	PORT_PinMuxConfig(HW_GPIOA,14,kPinAlt3);
	PORT_PinMuxConfig(HW_GPIOA,15,kPinAlt3);
	
	UART_CallbackRxInstall(HW_UART0,bluetooth_call_back);
	UART_ITDMAConfig(HW_UART0,kUART_IT_Rx,ENABLE);
}


void uart3_comm_init(void)
{
	UART_InitTypeDef UART_InitStruct1;     
    UART_InitStruct1.instance = HW_UART3;   
	UART_InitStruct1.baudrate = 38400;    
    UART_InitStruct1.parityMode = kUART_ParityDisabled;
    UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;   
    UART_Init(&UART_InitStruct1);
	
	/*Config the port's smultiplexing*/
	PORT_PinMuxConfig(HW_GPIOC,16,kPinAlt3);
	PORT_PinMuxConfig(HW_GPIOC,17,kPinAlt3);
	
	UART_CallbackRxInstall(HW_UART3,uart3_call_black);
	UART_ITDMAConfig(HW_UART3,kUART_IT_Rx,ENABLE);
}


void uart1_comm_init(void)
{
	UART_InitTypeDef UART_InitStruct1;     
    UART_InitStruct1.instance = HW_UART1;   
	UART_InitStruct1.baudrate = 115200;    
    UART_InitStruct1.parityMode = kUART_ParityDisabled;
    UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;   
    UART_Init(&UART_InitStruct1);
	
	/*Config the port's smultiplexing*/
	PORT_PinMuxConfig(HW_GPIOE,0,kPinAlt3);
	PORT_PinMuxConfig(HW_GPIOE,1,kPinAlt3);
	
	UART_CallbackRxInstall(HW_UART1,uart1_call_black);
	UART_ITDMAConfig(HW_UART1,kUART_IT_Rx,ENABLE);	
}
/*imu��������ָ�Ƶ��Ĭ��100HZ*/
void imu_init(void)
{
	uint8_t i;
	uint16_t start[]={0xA5,0x5A,0x04,0x01,0x05,0xAA};
//	uint16_t stop[]={0xA5,0x5A,0x04,0x02,0x06,0xAA};
	uint16_t magn_cali[]={0xA5,0x5A,0x04,0xE3,0xE7,0xAA};
	uint16_t magn_save[]={0xA5,0x5A,0x04,0xE1,0xE5,0xAA};
	uint16_t open_magn[]={0xA5,0x5A,0x04,0xE2,0xE6,0xAA};
//	uint16_t close_magn[]={0xA5,0x5A,0x04,0xE4,0xE8,0xAA};

	for(i=0;i<6;i++)
	{
		UART_WriteByte(HW_UART1, *(magn_cali+ i));
	}	
	for(i=0;i<6;i++)
	{
		UART_WriteByte(HW_UART1, *(magn_save + i));
	}
	for(i=0;i<6;i++)
	{
		UART_WriteByte(HW_UART1, *(open_magn + i));
	}	
//	for(i=0;i<6;i++)
//	{
//		UART_WriteByte(HW_UART1, *(close_magn + i));
//	}	
	for(i=0;i<6;i++)
	{
		UART_WriteByte(HW_UART1, *(start + i));
	}
}
	
	

