#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

//USB唤醒中断服务函数
void USBWakeUp_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line18);//清除USB唤醒中断挂起位
}

//USB中断处理函数
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

