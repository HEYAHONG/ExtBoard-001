#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "RC.h"


#ifdef RT_USING_DFS_MNTTABLE
#include <dfs_fs.h>
const struct dfs_mount_tbl mount_table[]=
{
    {NULL,"/","tmp",0,NULL},
    {0}
};
#endif

int main(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);

    {
        const char * banner=(const char *)RCGetHandle((const char *)"banner");
        if(banner!=NULL)
        {
            rt_kprintf("%s\r\n",banner);
        }
    }

    rt_kprintf("SYSCLK: %dMhz, \nHCLK: %dMhz, \nPCLK1: %dMhz, \nPCLK2: %dMhz, \nADCCLK: %dMhz\n",
               clocks.SYSCLK_Frequency / 1000000, clocks.HCLK_Frequency / 1000000,
               clocks.PCLK1_Frequency / 1000000, clocks.PCLK2_Frequency / 1000000, clocks.ADCCLK_Frequency / 1000000);
    {
        extern void fal_init();
        fal_init();
    }
}


#if !defined(RT_USING_DFS_MNTTABLE) && defined(RT_USING_DFS_TMPFS) && defined(RT_USING_DFS_V2)
#include "dfs_fs.h"
static int root_mount()
{
    /*
    * TODO:tmpfsπ“‘ÿ“Ï≥£
    */
    return dfs_mount(NULL,"tmp","tmp",0,NULL);
};
INIT_ENV_EXPORT(root_mount);
#endif

