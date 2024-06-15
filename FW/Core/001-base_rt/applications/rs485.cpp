#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>
#include <inttypes.h>
#include <cxx_mutex.h>
#include <cxx_thread.h>
#include "rs485.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <string>

#define RS485_UART_DEVICE_NAME "uart2"
#define RS485_UART_RTS_PIN GET_PIN(A, 1)
static rt_device_t rs485_dev=RT_NULL;
static rtthread::Mutex rs485_lock("rs485");
static class rs485_rx_thread:public rtthread::Thread
{
public:
    static void default_on_data(uint8_t *data,size_t data_length)
    {
        if(data!=NULL && data_length >0)
        {
            rt_kprintf("rs485(len=%d): ",data_length);
            for(size_t i=0; i<data_length; i++)
            {
                if(!iscntrl((char)data[i]))
                {
                    rt_kprintf("%02X(%c) ",(int)data[i],(char)data[i]);
                }
                else
                {
                    rt_kprintf("%02X ",(int)data[i]);
                }
            }
            rt_kprintf("\r\n");
        }
    }
    void (*on_data)(uint8_t *data,size_t data_length);
    rs485_rx_thread():rtthread::Thread(4096,(RT_THREAD_PRIORITY_MAX*2)/3,1,"rs485_rx"),on_data(default_on_data)
    {
    }
    virtual ~rs485_rx_thread()
    {
    }
protected:
    virtual void run(void *para)
    {
        std::string data_cache;
        rt_tick_t last_read_tick=rt_tick_get();
        while(true)
        {
            uint8_t ch=0;
            if(rt_device_read(rs485_dev,-1,&ch,1) == 1)
            {
                last_read_tick=rt_tick_get();
                data_cache+=std::string((char*)&ch,1);
            }
            {
                //判断空闲状态
                if(!data_cache.empty() && (rt_tick_get()-last_read_tick)>(1000/(((struct rt_serial_device *)rs485_dev)->config.baud_rate/10)+2))
                {
                    {
                        on_data((uint8_t *)data_cache.c_str(),data_cache.length());
                        data_cache.clear();
                    }
                }

                //判断最大字节数,一般来说单次数据不能大于256字节
                if(data_cache.length()>=256)
                {
                    {
                        on_data((uint8_t *)data_cache.c_str(),data_cache.length());
                        data_cache.clear();
                    }
                }

            }
            rt_thread_delay(1);

        }
    }
} g_rs485_rx_thread;

bool rs485_get_config(struct serial_configure *config)
{
    if(rs485_dev == NULL)
    {
        return false;
    }
    if(config != NULL)
    {
        (*config)=	((struct rt_serial_device *)rs485_dev)->config;
    }
    return true;
}

bool rs485_set_config(struct serial_configure *config)
{
    if(rs485_dev == NULL || config == NULL)
    {
        return false;
    }

    return RT_EOK==rt_device_control(rs485_dev,RT_DEVICE_CTRL_CONFIG,config);
}

bool rs485_write(uint8_t *buff,size_t len)
{
    if(rs485_dev == NULL )
    {
        return false;
    }
    if(buff == NULL || len == 0)
    {
        return true;
    }
    bool ret=false;
    if(!rs485_lock.lock())
    {
        return false;
    }
    rt_pin_write(RS485_UART_RTS_PIN,PIN_HIGH);
    ret=(len == rt_device_write(rs485_dev,0,buff,len));
    //delay 3 char (mcu feature)
    if(ret)
    {
        rt_thread_mdelay(3*1000/(((struct rt_serial_device *)rs485_dev)->config.baud_rate/10));
    }
    rt_pin_write(RS485_UART_RTS_PIN,PIN_LOW);
    rs485_lock.unlock();
    return ret;
}

bool rs485_set_on_data(void (*on)(uint8_t * data,size_t data_length))
{
    if(on==NULL)
    {
        on=	rs485_rx_thread::default_on_data;
    }
    g_rs485_rx_thread.on_data=on;
    if(rs485_dev == NULL )
    {
        return false;
    }
    return true;
}

rs485_on_data_t rs485_get_default_on_data()
{
    return rs485_rx_thread::default_on_data;
}

static int rs485_init()
{
    //init RTS
    rt_pin_mode(RS485_UART_RTS_PIN,PIN_MODE_OUTPUT);
    //default RX
    rt_pin_write(RS485_UART_RTS_PIN,PIN_LOW);

    rt_device_t dev=rt_device_find(RS485_UART_DEVICE_NAME);
    RT_ASSERT(dev != RT_NULL);
    if(dev->type == RT_Device_Class_Char)
    {
        rs485_dev = dev;
    }
    RT_ASSERT(rs485_dev != RT_NULL);

    {
        //init serialport 9600 8N1
        struct serial_configure config=RT_SERIAL_CONFIG_DEFAULT;
        config.baud_rate=BAUD_RATE_9600;
        config.data_bits=DATA_BITS_8;
        config.stop_bits=STOP_BITS_1;
        config.bufsz=1024;
        config.parity=PARITY_NONE;
        rt_device_control(rs485_dev,RT_DEVICE_CTRL_CONFIG,&config);
    }

    if(RT_EOK == rt_device_open(rs485_dev,RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX))
    {
        g_rs485_rx_thread.start();
        rt_kprintf("RS485:Init Ok!\n");
        return 0;
    }
    else
    {
        return -1;
    }
}

INIT_ENV_EXPORT(rs485_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <stdint.h>
#include <inttypes.h>
static void rs485(int argc,const char *argv[])
{
    if(argc==2)
    {
        rs485_write((uint8_t *)argv[1],rt_strlen(argv[1]));
    }
}
MSH_CMD_EXPORT_ALIAS(rs485,rs485,rs485 [send string]);
#endif
