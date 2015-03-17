/**
 * @file    ff_gen_drv.c 
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    22-April-2014
 * @brief   FatFs generic low level driver.
 */
///-----------------------------------------------------------------------------
#include "ff_gen_drv.h"
///-----------------------------------------------------------------------------
Disk_drvTypeDef  disk = {0};
///-----------------------------------------------------------------------------
/**
 * @brief  Links a compatible diskio driver and increments the number of active
 *         linked drivers.          
 * @note   The number of linked drivers (volumes) is up to 10 due to FatFs limits
 * @param  drv: pointer to the disk IO Driver structure
 * @param  path: pointer to the logical drive path 
 * @retval Returns 0 in case of success, otherwise 1.
 */
uint8_t FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path)
{
    uint8_t ret = 1;
    uint8_t DiskNum = 0;
    if(disk.nbr <= _VOLUMES)
    {
        disk.is_initialized[disk.nbr] = 0;
        disk.drv[disk.nbr] = drv;  
        DiskNum = disk.nbr++;
        path[0] = DiskNum + '0';
        path[1] = ':';
        path[2] = '/';
        path[3] = 0;
        ret = 0;
    }
    return ret;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Unlinks a diskio driver and decrements the number of active linked
 *         drivers.
 * @param  path: pointer to the logical drive path  
 * @retval Returns 0 in case of success, otherwise 1.
 */
uint8_t FATFS_UnLinkDriver(char *path)
{ 
    uint8_t DiskNum = 0;
    uint8_t ret = 1;
    if(disk.nbr >= 1)
    {    
        DiskNum = path[0] - '0';
        if(DiskNum <= disk.nbr)
        {
            disk.drv[disk.nbr--] = 0;
            ret = 0;
        }
    }
    return ret;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Gets number of linked drivers to the FatFs module.
 * @param  None
 * @retval Number of attached drivers.
 */
uint8_t FATFS_GetAttachedDriversNbr(void)
{
    return disk.nbr;
}
///-----------------------------------------------------------------------------
