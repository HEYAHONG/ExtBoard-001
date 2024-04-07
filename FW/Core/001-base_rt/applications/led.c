#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>

#define LED_RUN_PIN GET_PIN(A, 0)
static uint64_t led_run_table=0x5555555555555555;
static void led_run_timer(void *para)
{
    RT_UNUSED(para);
    static uint8_t i=0;
    if(i>=64)
    {
        i=0;
    }
    if(led_run_table&(1ULL << i))
    {
        rt_pin_write(LED_RUN_PIN,PIN_HIGH);
    }
    else
    {
        rt_pin_write(LED_RUN_PIN,PIN_LOW);
    }
    i++;
}

static int led_init()
{
    rt_pin_mode(LED_RUN_PIN,PIN_MODE_OUTPUT);
    rt_pin_write(LED_RUN_PIN,PIN_HIGH);
    rt_timer_t timer=rt_timer_create("led_run",led_run_timer,NULL,rt_tick_from_millisecond(100),RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
    RT_ASSERT(timer != RT_NULL);
    if(timer!=NULL)
    {
        rt_timer_start(timer);
    }
    return 0;
}


INIT_ENV_EXPORT(led_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <inttypes.h>
static void led(int argc,const char *argv[])
{
    if(argc == 1)
    {
        rt_kprintf("led run table=%016" PRIX64 "\n",led_run_table);
    }
    if(argc == 2)
    {
        uint64_t new_table=(uint64_t)strtoll(argv[1],NULL,16);
        rt_kprintf("led run table=%016" PRIX64 "\n",new_table);
        led_run_table=new_table;
    }
}

MSH_CMD_EXPORT_ALIAS(led,led,led [table]);
#endif

