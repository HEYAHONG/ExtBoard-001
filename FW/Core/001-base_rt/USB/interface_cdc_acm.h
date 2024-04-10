#ifndef __INTERFACE_CDC_ACM_H__
#define __INTERFACE_CDC_ACM_H__
#include "stdint.h"
#include "stdbool.h"
#include <rtthread.h>
#include <rtdevice.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************/
/* Line Coding Structure                                                       */
/*-----------------------------------------------------------------------------*/
/* Offset | Field       | Size | Value  | Description                          */
/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
/* 4      | bCharFormat |   1  | Number | Stop bits                            */
/*                                        0 - 1 Stop bit                       */
/*                                        1 - 1.5 Stop bits                    */
/*                                        2 - 2 Stop bits                      */
/* 5      | bParityType |  1   | Number | Parity                               */
/*                                        0 - None                             */
/*                                        1 - Odd                              */
/*                                        2 - Even                             */
/*                                        3 - Mark                             */
/*                                        4 - Space                            */
/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
/*******************************************************************************/
typedef struct
{
    uint32_t bitrate;
    uint8_t format;
    uint8_t paritytype;
    uint8_t datatype;
} line_coding_t;

//注意:绝大多数接口均运行于中断当中，建议将数据转入线程后处理。

bool cdc_acm_set_line_coding(line_coding_t *line_coding);
bool cdc_acm_get_line_coding(line_coding_t *line_coding);
bool cdc_acm_send_break();
void cdc_acm_set_dtr(bool dtr);
void cdc_acm_set_rts(bool rts);
void cdc_acm_on_data(uint8_t *data,size_t length);
void cdc_acm_write_data(uint8_t ch);


#ifdef __cplusplus
}
#endif

#endif
