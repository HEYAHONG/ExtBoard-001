#ifndef __RS485_H__
#define __RS485_H__
#include "stdint.h"
#include "stdbool.h"
#include <rtthread.h>
#include <rtdevice.h>
#ifdef __cplusplus
extern "C"
{
#endif

bool rs485_get_config(struct serial_configure *config);
bool rs485_set_config(struct serial_configure *config);
bool rs485_write(uint8_t *buff,size_t len);
bool rs485_set_on_data(void (*on)(uint8_t ch));

#ifdef __cplusplus
}
#endif
#endif



