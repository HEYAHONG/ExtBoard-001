#include "interface_cdc_acm.h"

RT_WEAK bool cdc_acm_set_line_coding(line_coding_t *line_coding)
{
    rt_kprintf("\nCDC:Set Line Code(Baud=%d,Type(Stopbit)=%d,Parity=%d,Databits=%d)\n",line_coding->bitrate,line_coding->format,line_coding->paritytype,line_coding->datatype);
    return true;
}

RT_WEAK bool cdc_acm_get_line_coding(line_coding_t *line_coding)
{
    rt_kprintf("\nCDC:Get Line Code(Baud=%d,Type(Stopbit)=%d,Parity=%d,Databits=%d)\n",line_coding->bitrate,line_coding->format,line_coding->paritytype,line_coding->datatype);
    return true;
}
RT_WEAK bool cdc_acm_send_break()
{
    return true;
}
RT_WEAK void cdc_acm_set_dtr(bool dtr)
{
    rt_kprintf("\nCDC:DTR=%s\n",dtr?"true":"false");
}
RT_WEAK void cdc_acm_set_rts(bool rts)
{
    rt_kprintf("\nCDC:RTS=%s\n",rts?"true":"false");
}

RT_WEAK void cdc_acm_on_data(uint8_t *data,size_t length)
{
    if(data != NULL && length !=0)
    {
        for(size_t i=0; i<length; i++)
        {
            cdc_acm_write_data(data[i]);
        }
    }
}

extern "C" void USB_UART_Write_FIFO(uint8_t ch);
void cdc_acm_write_data(uint8_t ch)
{
    USB_UART_Write_FIFO(ch);
}


