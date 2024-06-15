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
bool rs485_set_on_data(void (*on)(uint8_t * data,size_t data_length));
typedef void (*rs485_on_data_t)(uint8_t * data,size_t data_length);
rs485_on_data_t rs485_get_default_on_data();


#ifdef __cplusplus
}
#endif
#endif



