#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

//USB�����жϷ�����
void USBWakeUp_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line18);//���USB�����жϹ���λ
}

//USB�жϴ�����
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    rt_interrupt_enter();
    extern void USB_Istr();
    USB_Istr();
    rt_interrupt_leave();
}

static int usb_init()
{
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();
    DP_PUUP = 1;
    return 0;
}

INIT_ENV_EXPORT(usb_init);

