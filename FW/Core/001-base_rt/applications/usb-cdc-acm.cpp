#include "usb-cdc-acm.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>
#include <inttypes.h>
#include <cxx_mutex.h>
#include <cxx_thread.h>
#include <string>
#include <functional>
#include <queue>
extern "C" bool USB_Is_CONFIGURED();
static rtthread::Mutex usb_acm_lock("usb_acm");
class usb_lock_gaurd
{
public:
    usb_lock_gaurd()
    {
        usb_acm_lock.lock();
    }
    ~usb_lock_gaurd()
    {
        usb_acm_lock.unlock();
    }
};
static void usb_send(uint8_t *buff,size_t length)
{
    if(buff==NULL || length == 0)
    {
        return;
    }
    usb_lock_gaurd lock;
    for(size_t i=0; i<length; i++)
    {
        cdc_acm_write_data(buff[i]);
    }
}

static class usb_rx_thread:public rtthread::Thread
{
public:
    std::queue<std::string> data_queue;
    usb_rx_thread():rtthread::Thread(4096,(RT_THREAD_PRIORITY_MAX*2)/3,5,"usb_acm")
    {
    }
    virtual ~usb_rx_thread()
    {
    }
private:
    void print(uint8_t *data,size_t data_length)
    {
        if(data!=NULL && data_length >0)
        {
            rt_kprintf("usb(len=%d): ",data_length);
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
    virtual void run(void *para)
    {
        std::string data_cache;
        while(true)
        {
            while(!data_queue.empty())
            {
                data_cache=data_queue.front();
                rt_enter_critical();
                data_queue.pop();
                rt_exit_critical();
                {
                    //测试代码
                    //直接打印数据
                    print((uint8_t *)data_cache.c_str(),data_cache.length());
                }
            }
            rt_thread_delay(1);
        }
    }
} g_usb_rx_thread;

void cdc_acm_on_data(uint8_t *data,size_t length)
{
    if(data==NULL || length == 0)
    {
        return;
    }
    rt_enter_critical();
    g_usb_rx_thread.data_queue.push(std::string((char *)data,length));
    rt_exit_critical();
}

static int usb_init()
{
    g_usb_rx_thread.start();
    return 0;
};
INIT_APP_EXPORT(usb_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <stdint.h>
#include <inttypes.h>
static void usb(int argc,const char *argv[])
{
    if(!USB_Is_CONFIGURED())
    {
        rt_kprintf("Not Configured!\r\n");
        return;
    }
    if(argc==2)
    {
        usb_send((uint8_t *)argv[1],rt_strlen(argv[1]));
    }
}
MSH_CMD_EXPORT_ALIAS(usb,usb,usb [send string]);
#endif
