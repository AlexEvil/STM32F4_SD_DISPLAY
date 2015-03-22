///-----------------------------------------------------------------------------
#include "sd_storage.h"
///-----------------------------------------------------------------------------
FATFS SD_FatFs;   /// File system object for User logical drive
FIL   SD_File;    /// File object
char  SD_Path[4]; /// User logical drive path
///-----------------------------------------------------------------------------
void StorageInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    SD_IO_DETECT_GPIO_CLK_ENABLE();
    /// Configure SPI Card Detect Pin
	GPIO_InitStruct.Pin       = SD_IO_DETECT_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING_FALLING; //GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
    HAL_GPIO_Init(SD_IO_DETECT_GPIO_PORT, &GPIO_InitStruct);
    /// initialization interrupt for uSD driver
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,0,1);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
///-----------------------------------------------------------------------------
bool StorageConnect(void)
{
    bool Result;
    Result = FATFS_LinkDriver(&SD_Driver,SD_Path);
	if(Result == 0)
	{
        Result = BSP_SD_MountCardDetect();
	}
    return(Result);
}
///-----------------------------------------------------------------------------
bool StorageDisconnect(void)
{
	if(f_mount((FATFS*)NULL,(TCHAR const*)SD_Path,1) == FR_OK)
	{
		if(FATFS_UnLinkDriver(SD_Path) == 0)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}
	else
	{
		return(false);
	}
}
///-----------------------------------------------------------------------------
bool StorageGetInfo(SD_CardInfo* Info)
{
    uint8_t Ret;
    Ret = BSP_SD_GetCardInfo(Info);
    if(Ret == MSD_OK)
    {
        return(true);
    }
    return(false);
}
///-----------------------------------------------------------------------------
bool StorageWriteLn(const char* file_name,char* buffer,uint32_t length)
{
	FRESULT f_res;
	FILINFO f_info;
	uint32_t wr_length;
	f_res = f_open(&SD_File,file_name,FA_OPEN_EXISTING | FA_WRITE);
	if(f_res == FR_NO_FILE)
	{ /// if file no found then create new file at sd storage
		f_res = f_open(&SD_File,file_name,FA_CREATE_NEW | FA_WRITE);
	}
    if(f_res == FR_OK)
	{
		f_res = f_stat(file_name,&f_info);
		if(f_res == FR_OK)
		{
			f_res = f_lseek(&SD_File,f_info.fsize);
			if(f_res == FR_OK)
			{
				f_res = f_write(&SD_File,&buffer[0],length,&wr_length);
				if(f_res == FR_OK)
				{
					f_res = f_write(&SD_File,"\n",0x01,&wr_length);
					if(f_res == FR_OK)
					{
						f_res = f_sync(&SD_File);
					}
				}
			}
		}
		f_close(&SD_File);
	}
	if(f_res == FR_OK)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
///-----------------------------------------------------------------------------
