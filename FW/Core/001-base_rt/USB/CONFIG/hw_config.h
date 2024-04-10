#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "platform_config.h"
#include "usb_type.h"



#define USB_USART_TXFIFO_SIZE   4096	//USB虚拟串口发送FIFO大小		
#define USB_USART_REC_LEN	 	200		//USB串口接收缓冲区最大字节数

//定义一个USB USART FIFO结构体
typedef struct
{
    u8  buffer[USB_USART_TXFIFO_SIZE];	//buffer
    vu16 writeptr;						//写指针
    vu16 readptr;						//读指针
} _usb_usart_fifo;
extern _usb_usart_fifo uu_txfifo;		//USB串口发送FIFO


//USB通用代码函数声明
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_Port_Set(u8 enable);
void IntToUnicode (u32 value,u8 *pbuf,u8 len);
void Get_SerialNum(void);


#endif
