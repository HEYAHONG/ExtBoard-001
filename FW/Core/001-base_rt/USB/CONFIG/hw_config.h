#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "platform_config.h"
#include "usb_type.h"



#define USB_USART_TXFIFO_SIZE   4096	//USB���⴮�ڷ���FIFO��С		
#define USB_USART_REC_LEN	 	200		//USB���ڽ��ջ���������ֽ���

//����һ��USB USART FIFO�ṹ��
typedef struct
{
    u8  buffer[USB_USART_TXFIFO_SIZE];	//buffer
    vu16 writeptr;						//дָ��
    vu16 readptr;						//��ָ��
} _usb_usart_fifo;
extern _usb_usart_fifo uu_txfifo;		//USB���ڷ���FIFO


//USBͨ�ô��뺯������
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_Port_Set(u8 enable);
void IntToUnicode (u32 value,u8 *pbuf,u8 len);
void Get_SerialNum(void);


#endif