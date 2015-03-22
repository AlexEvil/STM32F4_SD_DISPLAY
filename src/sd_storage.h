///-----------------------------------------------------------------------------
#ifndef _SD_STORAGE
#define _SD_STORAGE
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
///-----------------------------------------------------------------------------
#include "../fat/sd_diskio.h"
///-----------------------------------------------------------------------------     
extern FATFS SD_FatFs;   /// File system object for User logical drive
extern FIL   SD_File;    /// File object
extern char  SD_Path[4]; /// User logical drive path
///-----------------------------------------------------------------------------
void StorageInit(void);
bool StorageConnect(void);
bool StorageDisconnect(void);
bool StorageWriteLn(const char* file_name,char* buffer,uint32_t length);
extern bool StorageGetInfo(SD_CardInfo* Info);
///-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif
///-----------------------------------------------------------------------------
