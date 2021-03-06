///-----------------------------------------------------------------------------
#include "sd_storage.h"
///-----------------------------------------------------------------------------
FATFS SD_FatFs;   /// File system object for User logical drive
FIL   SD_File;    /// File object
char  SD_Path[4]; /// User logical drive path
///-----------------------------------------------------------------------------
/// struct for control write protection and card detect
//struct
//{
//	bool CD;
//	bool WP;
//}SD_WP_CD;
///-----------------------------------------------------------------------------
bool SD_Connect(void)
{
	if(FATFS_LinkDriver(&SD_Driver,SD_Path) == 0)
	{
		if(f_mount(&SD_FatFs,(TCHAR const*)SD_Path,1) == FR_OK)
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
bool SD_Disconnect(void)
{
	if(f_mount(NULL,(TCHAR const*)SD_Path,1) == FR_OK)
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
bool SD_WriteLn(const char* file_name,char* buffer,uint32_t length)
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

//if(f_puts(text, &MyFile) == FR_OK)
//f_close(&MyFile);
  
