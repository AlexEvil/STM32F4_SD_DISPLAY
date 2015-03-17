/**
  * @file    ff_gen_drv.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    22-April-2014
  * @brief   Header for ff_gen_drv.c module.
  */
///-----------------------------------------------------------------------------
#ifndef __FF_GEN_DRV_H
#define __FF_GEN_DRV_H
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include "diskio.h"
#include "ff.h"
///-----------------------------------------------------------------------------
/** 
 * @brief  Disk IO Driver structure definition  
 */ 
typedef struct
{
    DSTATUS (*disk_initialize) (void);                     /// Initialize Disk Drive
    DSTATUS (*disk_status)     (void);                     /// Get Disk Status
    DRESULT (*disk_read)       (BYTE*, DWORD, BYTE);       /// Read Sector(s)
    #if _USE_WRITE == 1 /// _USE_WRITE == 1
    DRESULT (*disk_write)      (const BYTE*, DWORD, BYTE); /// Write Sector(s) when _USE_WRITE = 0
    #endif
	#if _USE_IOCTL == 1 /// _USE_IOCTL == 1
    DRESULT (*disk_ioctl)      (BYTE, void*);              /// I/O control operation when _USE_IOCTL = 1
    #endif
}Diskio_drvTypeDef;
///-----------------------------------------------------------------------------
/** 
 * @brief  Global Disk IO Drivers structure definition  
 */ 
typedef struct
{ 
  uint8_t           is_initialized[_VOLUMES];
  Diskio_drvTypeDef *drv[_VOLUMES];
  __IO uint8_t      nbr;

}Disk_drvTypeDef;
///-----------------------------------------------------------------------------
uint8_t FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path);
uint8_t FATFS_UnLinkDriver(char *path);
uint8_t FATFS_GetAttachedDriversNbr(void);
///-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif /* __FF_GEN_DRV_H */
///-----------------------------------------------------------------------------
