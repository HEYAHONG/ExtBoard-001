#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>
#include <inttypes.h>

#define WDT_DEVICE_NAME    "wdt"

rt_uint32_t timeout = 3;
static rt_device_t wdg_dev=NULL;

static void idle_hook(void)
{
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

static int wdt_init()
{
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    RT_ASSERT(wdg_dev!=NULL);
    rt_device_init(wdg_dev);
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, NULL);
    rt_thread_idle_sethook(idle_hook);
    rt_kprintf("WDT:Init Ok!\n");
    return 0;
}

INIT_ENV_EXPORT(wdt_init);
