///-----------------------------------------------------------------------------
/// Low level disk I/O module skeleton for FatFs (C)ChaN, 2013
///-----------------------------------------------------------------------------
/**
 * @file    diskio.c 
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    22-April-2014
 * @brief   FatFs low level disk I/O module.
 */
///-----------------------------------------------------------------------------
#include "diskio.h"
#include "ff_gen_drv.h"
///-----------------------------------------------------------------------------
extern Disk_drvTypeDef  disk;
///-----------------------------------------------------------------------------
/**
 * @brief  Initializes a Drive
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS disk_initialize(BYTE pdrv)
{
    DSTATUS stat = RES_OK;
    if(disk.is_initialized[pdrv] == 0)
    {
        disk.is_initialized[pdrv] = 1;
        stat = disk.drv[pdrv]->disk_initialize();
    }
    return stat;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Gets Disk Status 
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS disk_status(BYTE pdrv)
{
    DSTATUS stat;
    stat = disk.drv[pdrv]->disk_status();
    return stat;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Reads Sector(s) 
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data buffer to store read data
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to read (1..128)
 * @retval DRESULT: Operation result
 */
DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, BYTE count)
{
    DRESULT res;
    res = disk.drv[pdrv]->disk_read(buff, sector, count);
    return res;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Writes Sector(s)  
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data to be written
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to write (1..128)
 * @retval DRESULT: Operation result
 */
#if _USE_WRITE == 1
DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, BYTE count)
    {
        DRESULT res;
        res = disk.drv[pdrv]->disk_write(buff, sector, count);
        return res;
    }
#endif /* _USE_WRITE == 1 */
///-----------------------------------------------------------------------------
/**
 * @brief  I/O control operation  
 * @param  pdrv: Physical drive number (0..)
 * @param  cmd: Control code
 * @param  *buff: Buffer to send/receive control data
 * @retval DRESULT: Operation result
 */
#if _USE_IOCTL == 1
    DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
    {
        DRESULT res;
        res = disk.drv[pdrv]->disk_ioctl(cmd, buff);
        return res;
    }
#endif /* _USE_IOCTL == 1 */
///-----------------------------------------------------------------------------
/**
 * @brief  Gets Time from RTC 
 * @param  None
 * @retval Time in DWORD
 */
DWORD get_fattime (void)
{
    return 0;
}
///-----------------------------------------------------------------------------
