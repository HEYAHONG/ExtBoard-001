
#include "stdio.h"
#include "stdint.h"
#include "libserialport.h"
#include "string.h"
#include "Version.h"
#include <string>

int main(int argc,const char *argv[])
{
    printf("version:%s\nlibserialport:%s\n\n",PROJECT_VERSION_STR,SP_PACKAGE_VERSION_STRING);

    struct sp_port **port_list=NULL;
    if(SP_OK!=sp_list_ports(&port_list))
    {
        printf("list port failed!\n");
        return -1;
    }

    for (size_t i = 0; port_list[i] != NULL; i++)
    {
        struct sp_port *port = port_list[i];

        //USB传输
        if(SP_TRANSPORT_USB == sp_get_port_transport(port))
        {
            int vid=0,pid=0;
            if(SP_OK != sp_get_port_usb_vid_pid(port,&vid,&pid))
            {
                continue;
            }
            int usb_bus=0,usb_dev=0;
            if(SP_OK != sp_get_port_usb_bus_address(port,&usb_bus,&usb_dev))
            {
                continue;
            }

            printf("Found USB(%d-%d) SerialPort(%04X:%04X) %s\n",usb_bus,usb_dev,vid,pid,sp_get_port_name(port));

            //判断DAPLink的VID与PID
            if(vid==0x0D28 && pid == 0x0204)
            {
                const char *Manufacturer=sp_get_port_usb_manufacturer(port);
                const char *Product=sp_get_port_usb_product(port);
                if(Manufacturer!=NULL && Product!=NULL)
                {
                    //判断是否是ExtBoard-001的DAPLink
                    if(strcmp(Manufacturer,"HYH") == 0 && strcmp(Product,"ExtBoard-001") == 0)
                    {
                        //打印找到的ExtBoard-001的Console串口
                        printf("Found ExtBoard-001 Console %s\n",sp_get_port_name(port));
                    }
                }
            }

            //判断Air32的VID与PID
            if(vid == 0x0483 && pid == 0x5740)
            {
                const char *Manufacturer=sp_get_port_usb_manufacturer(port);
                const char *Product=sp_get_port_usb_product(port);
                if(Manufacturer!=NULL && Product!=NULL)
                {
                    //判断是否是ExtBoard-001的DAPLink
                    if(strcmp(Manufacturer,"HYH") == 0 && strcmp(Product,"ExtBoard-001-Core") == 0)
                    {
                        //打印找到的ExtBoard-001的Console串口
                        printf("Found ExtBoard-001-Core VCOM %s\n",sp_get_port_name(port));
                    }
                }
            }
        }
    }


    sp_free_port_list(port_list);

    return 0;
}
