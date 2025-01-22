#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifdef BSP_USING_I2C1
#include <drivers/dev_i2c.h>

static struct rt_i2c_bus_device *  aht20_i2c_bus=RT_NULL;

#define AHT20_I2C_ADDRESS_7BIT 0x38

static uint8_t aht20_read_state()
{
    uint8_t state=0;
    struct rt_i2c_msg msg= {0};
    msg.addr=AHT20_I2C_ADDRESS_7BIT;
    msg.flags=RT_I2C_RD;
    msg.buf=&state;
    msg.len=sizeof(state);
    rt_i2c_transfer(aht20_i2c_bus,&msg,1);
    return state;
}

static void aht20_start_measure()
{
    uint8_t cmd[]= {0xAC,0x33,0x00};
    struct rt_i2c_msg msg= {0};
    msg.addr=AHT20_I2C_ADDRESS_7BIT;
    msg.flags=RT_I2C_WR;
    msg.buf=cmd;
    msg.len=sizeof(cmd);
    rt_i2c_transfer(aht20_i2c_bus,&msg,1);
}

static uint8_t aht20_crc(uint8_t *msg,size_t len)
{
    if(msg==NULL)
    {
        return 0;
    }
    uint8_t i=0;
    uint8_t byte=0;
    uint8_t crc=0xFF;
    for(byte=0; byte<len; byte++)
    {
        crc^=(msg[byte]);
        for(i=8; i>0; --i)
        {
            if(crc&0x80)
            {
                crc=(crc<<1)^0x31;
            }
            else
            {
                crc=(crc<<1);
            }
        }
    }
    return crc;
}

static bool aht20_read(double *T,double *RH)
{
    int count=0;

    aht20_start_measure();

    while(aht20_read_state()&0x80)
    {
        rt_thread_mdelay(10);
        if(count++ > 10)
        {
            return false;
        }
    }

    uint8_t data[7]= {0};
    struct rt_i2c_msg msg= {0};
    msg.addr=AHT20_I2C_ADDRESS_7BIT;
    msg.flags=RT_I2C_RD;
    msg.buf=data;
    msg.len=sizeof(data);
    rt_i2c_transfer(aht20_i2c_bus,&msg,1);

    if(aht20_crc(data,6) == data[6])
    {
        uint32_t RH_uint=	data[1];
        RH_uint <<= 8;
        RH_uint +=	data[2];
        RH_uint <<= 4;
        RH_uint+= ((data[3]>>4)&0xF);
        if(RH!=NULL)
        {
            (*RH)=(RH_uint*(double)1.0)/(1<<20);
        }

        uint32_t T_uint=(data[3]&0xF);
        T_uint <<=8;
        T_uint+=data[4];
        T_uint <<=8;
        T_uint+=data[5];
        if(T!=NULL)
        {
            (*T)=(T_uint*(double)1.0)/(1<<20)*200.0-50.0;
        }

        return true;
    }

    return false;
}

static int aht20_init()
{
    rt_device_t dev=rt_device_find("i2c1");
    RT_ASSERT(dev != RT_NULL);
    if(dev->type == RT_Device_Class_I2CBUS)
    {
        aht20_i2c_bus=(struct rt_i2c_bus_device *)dev;
    }
    RT_ASSERT(aht20_i2c_bus != RT_NULL);

    double T=0,RH=0;
    if(aht20_read(&T,&RH))
    {
        char RH_str[32]= {0};
        sprintf(RH_str,"%.2lf",RH*100);
        char T_str[32]= {0};
        sprintf(T_str,"%.2lf",T);
        rt_kprintf("AHT20:T=%s,RH=%s\n",T_str,RH_str);
    }

    return 0;
}

INIT_ENV_EXPORT(aht20_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <inttypes.h>
static void aht20(int argc,const char *argv[])
{
    double T=0,RH=0;
    if(aht20_read(&T,&RH))
    {
        char RH_str[32]= {0};
        sprintf(RH_str,"%.2lf",RH*100);
        char T_str[32]= {0};
        sprintf(T_str,"%.2lf",T);
        rt_kprintf("AHT20:T=%s,RH=%s\n",T_str,RH_str);
    }
}

MSH_CMD_EXPORT_ALIAS(aht20,aht20,read aht20);

#endif

#endif
