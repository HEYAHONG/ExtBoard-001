
#include "stdio.h"
#include "stdint.h"
#include "libusb.h"
#include "Version.h"
#include "string"
#include "string.h"
#include "vector"

#ifndef LIBUSB_API_VERSION
#error "libusb is too old!"
#endif // LIBUSB_API_VERSION


typedef struct
{
    uint8_t  bus_number;//ExtBoard-001所在的usb总线号
    uint8_t  core_dev_address;//Core设备地址(USB)
    uint8_t  port_path[8];//ExtBoard-001所在的usb总线路径,即USB Hub芯片连接的端口
    bool      core_is_use_usb;//ExtBoard-001是否使用usb总线
} extboard_001_info_t;

std::vector<extboard_001_info_t> extboard_001_list;

int main(int argc,const char *argv[])
{
    printf("version:%s\nlibusb(%d.%d.%d) api version:%08X \n",PROJECT_VERSION_STR,libusb_get_version()->major,libusb_get_version()->minor,libusb_get_version()->micro,LIBUSB_API_VERSION);


    printf("\nusb enum start!\n");

    //初始化libusb上下文
    libusb_context *ctx=NULL;
    libusb_init(&ctx);

    if(ctx == NULL)
    {
        printf("libusb init failed!\n");
        return -1;
    }

    {
        //查找ExtBoard-001的DAPLink

        ssize_t usb_device_count=0;
        libusb_device **usb_device_list=NULL;
        //获取usb设备列表
        usb_device_count=libusb_get_device_list(ctx,&usb_device_list);
        if(usb_device_count > 0 && usb_device_list !=NULL)
        {

            for(ssize_t i=0; i<usb_device_count; i++)
            {
                libusb_device *usb_device=usb_device_list[i];
                libusb_device_descriptor usb_device_desc= {0};
                if(0!=libusb_get_device_descriptor(usb_device,&usb_device_desc))
                {
                    continue;
                }

                //通过DAPLink的vid与pid查找ExtBoard-001
                if(usb_device_desc.idVendor == 0x0D28 && usb_device_desc.idProduct == 0x0204)
                {
                    libusb_device_handle *usb_device_handle=NULL;
                    libusb_open(usb_device,&usb_device_handle);
                    if(usb_device_handle == NULL)
                    {
                        continue;
                    }

                    //制造商
                    std::string Manufacturer;
                    {
                        //读取制造商
                        char buff[256]= {0};
                        libusb_get_string_descriptor_ascii(usb_device_handle,usb_device_desc.iManufacturer,(uint8_t *)buff,sizeof(buff));
                        Manufacturer=buff;
                    }

                    //产品名称
                    std::string Product;
                    {
                        //读取产品名称
                        char buff[256]= {0};
                        libusb_get_string_descriptor_ascii(usb_device_handle,usb_device_desc.iProduct,(uint8_t *)buff,sizeof(buff));
                        Product=buff;
                    }

                    libusb_close(usb_device_handle);

                    if(Manufacturer == "HYH" && Product == "ExtBoard-001")
                    {
                        //已找到ExtBoard-001的DAPLink

                        printf("DAPLink is found!\n");

                        //读取bus_number
                        uint8_t usb_bus=libusb_get_bus_number(usb_device);
                        printf("\tbus number:\t%d\n",(int)usb_bus);

                        //读取port_path
                        uint8_t usb_port_path[8]= {0};
                        libusb_get_port_path(ctx,usb_device,usb_port_path,sizeof(usb_port_path));
                        uint8_t extboard_usb_port_length=0;
                        bool is_extboard_daplink=true;
                        {
                            printf("\tport_path:\t");
                            for(size_t i=0; i<sizeof(usb_port_path); i++)
                            {
                                if(usb_port_path[i] == 0)
                                {
                                    break;
                                }

                                if(usb_port_path[i+1] == 0)
                                {
                                    extboard_usb_port_length=i;

                                    if(usb_port_path[i] != 1)
                                    {
                                        // DAPLink不在端口1,可能不是ExtBoard-001的DAPLink

                                        is_extboard_daplink=false;
                                    }
                                }

                                if(i > 0)
                                {
                                    printf(".");
                                }

                                printf("%d",usb_port_path[i]);
                            }
                            printf("\n");
                        }

                        if(is_extboard_daplink && extboard_usb_port_length > 0)
                        {
                            //添加到ExtBoard-001列表
                            extboard_001_info_t info;
                            info.bus_number=usb_bus;
                            memset(info.port_path,0,sizeof(info.port_path));
                            memcpy(info.port_path,usb_port_path,extboard_usb_port_length);
                            info.core_is_use_usb=false;

                            extboard_001_list.push_back(info);
                        }

                    }

                }


            }

            //释放设备列表
            libusb_free_device_list(usb_device_list,usb_device_count);
        }
    }

    {
        //判断ExtBoard-001的Core是否使用USB

        ssize_t usb_device_count=0;
        libusb_device **usb_device_list=NULL;
        //获取usb设备列表
        usb_device_count=libusb_get_device_list(ctx,&usb_device_list);
        if(usb_device_count > 0 && usb_device_list !=NULL)
        {

            for(ssize_t i=0; i<usb_device_count; i++)
            {
                libusb_device *usb_device=usb_device_list[i];

                //读取bus_number
                uint8_t usb_bus=libusb_get_bus_number(usb_device);

                //读取port_path
                uint8_t usb_port_path[8]= {0};
                libusb_get_port_path(ctx,usb_device,usb_port_path,sizeof(usb_port_path));

                for(std::vector<extboard_001_info_t>::iterator it=extboard_001_list.begin(); it!=extboard_001_list.end(); it++)
                {
                    extboard_001_info_t & info=(*it);

                    if(info.bus_number == usb_bus)
                    {
                        uint8_t core_port_path[8] = {0};
                        size_t core_port_path_length=0;
                        memcpy(core_port_path,info.port_path,sizeof(info.port_path));

                        for(size_t i =0 ; i < sizeof(core_port_path); i++)
                        {
                            if(core_port_path[i]==0)
                            {
                                //Core连接到端口2
                                core_port_path[i]=2;
                                core_port_path_length=i+2;
                                break;
                            }
                        }

                        if(core_port_path_length > 0 && memcmp(core_port_path,usb_port_path,core_port_path_length) == 0)
                        {
                            //Core连接到USB，如需对Core进行操作,可在此处打开usb_device
                            info.core_dev_address=libusb_get_device_address(usb_device);
                            info.core_is_use_usb=true;
                        }
                    }
                }
            }

            //释放设备列表
            libusb_free_device_list(usb_device_list,usb_device_count);
        }
    }

    //销毁libusb上下文
    libusb_exit(ctx);

    printf("\nusb enum done!\n");

    {

        printf("\nlist ExtBoard-001!\n");

        //打印ExtBoard-001信息
        for(size_t i =0 ; i<extboard_001_list.size(); i++)
        {
            extboard_001_info_t & info=extboard_001_list[i];
            printf("ExtBoard-001 %d:\n",(int)i);
            printf("\tusb bus number:\t%d\n",(int)info.bus_number);
            printf("\tusb port path:\t");
            for(size_t i=0; i<sizeof(info.port_path); i++)
            {
                if(info.port_path[i] == 0)
                {
                    break;
                }
                if(i > 0)
                {
                    printf(".");
                }

                printf("%d",info.port_path[i]);
            }
            printf("\n");
            printf("\tcore is use usb:%s\n",info.core_is_use_usb?"true":"false");
            if(info.core_is_use_usb)
            {
                 printf("\tcore usb dev address:\t%d\n",(int)info.core_dev_address);
            }

        }
    }

    return 0;
}

