///-----------------------------------------------------------------------------
#include <cmsis_os.h>
///-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
///-----------------------------------------------------------------------------
#include "led.h"
#include "sd_storage.h"
///-----------------------------------------------------------------------------
#define USES_SD
#define USES_TFT
///-----------------------------------------------------------------------------
#ifdef RTE_CMSIS_RTOS_RTX
	extern uint32_t os_time;
	uint32_t HAL_GetTick(void)
	{
		return os_time;
	}
	void Delay(const uint32_t time)
	{
		uint32_t cur_os_time;
		uint32_t end_os_time;
		cur_os_time = os_time;
		end_os_time = cur_os_time + time;
		while(1)
		{
			if(end_os_time == os_time)
			{
				break;
			}
		}
		return;
	}
#endif
///-----------------------------------------------------------------------------
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/// Enable Power Control clock
	__PWR_CLK_ENABLE();
	/// The voltage scaling allows optimizing the power consumption when the
	/// device is clocked below the maximum system frequency (see datasheet).
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/// Enable HSE Oscillator and activate PLL with HSE as source
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/// Select PLL as system clock source and configure the
	/// HCLK, PCLK1 and PCLK2 clocks dividers
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |
	     						  RCC_CLOCKTYPE_PCLK1  |
								  RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_5);
	return;
}
///-----------------------------------------------------------------------------
void MainSystemInit(void)
{
	HAL_StatusTypeDef hal_status;
	hal_status = HAL_Init();
	if(hal_status != HAL_OK)
	{
		while(1);
	}
	SystemClock_Config();
	SystemCoreClockUpdate();
	if(osKernelInitialize() != osOK)
	{
		while(1);
	}
	if(osKernelStart() != osOK)
	{
		while(1);
	}
	return;
}
///-----------------------------------------------------------------------------
#ifdef USES_TFT
	#define TFT_LIGHT  GPIO_PIN_11
	#define TFT_DE     GPIO_PIN_12
	#define TFT_DISP   GPIO_PIN_13
	#define TFT_CLK    GPIO_PIN_14
	#define TFT_B7     GPIO_PIN_14
	#define TFT_B6     GPIO_PIN_13
	#define TFT_B5     GPIO_PIN_12
	#define TFT_B4     GPIO_PIN_11
	#define TFT_B3     GPIO_PIN_10
	#define TFT_B2     GPIO_PIN_9
	#define TFT_B1     GPIO_PIN_8
	#define TFT_B0     GPIO_PIN_7

	#define TFT_CTL_PORT    TFT_LIGHT | TFT_DE | TFT_DISP | TFT_CLK
	#define TFT_DATA_PORT   TFT_B7 | TFT_B6 | TFT_B5 | TFT_B4 | \
                            TFT_B3 | TFT_B2 | TFT_B1 | TFT_B0

	void FTF_Init(void)
	{
		GPIO_InitTypeDef PinDef;
		__GPIOB_CLK_ENABLE();
		__GPIOE_CLK_ENABLE();
		/// setting control
		PinDef.Pin   = TFT_CTL_PORT;
		PinDef.Mode  = GPIO_MODE_OUTPUT_PP;
		PinDef.Pull  = GPIO_PULLUP;
		PinDef.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB,&PinDef);
		/// setting data
		PinDef.Pin   = TFT_DATA_PORT;
		PinDef.Mode  = GPIO_MODE_OUTPUT_PP;
		PinDef.Pull  = GPIO_PULLDOWN;
		PinDef.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOE,&PinDef);
		return;
	}
	void FTF_Light(GPIO_PinState state)
	{
		HAL_GPIO_WritePin(GPIOB,TFT_LIGHT,state);
	}
	void FTF_DataEnable(GPIO_PinState state)
	{
		HAL_GPIO_WritePin(GPIOB,TFT_DE,state);
	}
	void FTF_Display(GPIO_PinState state)
	{
		HAL_GPIO_WritePin(GPIOB,TFT_DISP,state);
	}
	void FTF_Clock(void)
	{
		HAL_GPIO_WritePin(GPIOB,TFT_CLK,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,TFT_CLK,GPIO_PIN_RESET);
	}
	void FTF_Data(void)
	{
		HAL_GPIO_WritePin(GPIOE,TFT_B0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,TFT_B1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,TFT_B2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,TFT_B3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,TFT_B4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,TFT_B5,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,TFT_B6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,TFT_B7,GPIO_PIN_RESET);
	}
#endif
///-----------------------------------------------------------------------------
int main(void)
{
	#ifdef USES_TFT
		unsigned short cnt;
		unsigned short cnt_1;
	#endif
	#ifdef USES_SD
		char text[] = "thise is new version board";	
	#endif
    ///-------------------------------------------------------------------------
    MainSystemInit();
    LedInit();
	///-------------------------------------------------------------------------
	#ifdef USES_TFT
		FTF_Init();
	#endif
    ///-------------------------------------------------------------------------
	#ifdef USES_SD
		if(SD_Connect() == false)
		{
			LedOn(LED_RED);
		}
		else
		{
			if(SD_WriteLn("E_V_I_L_TEST.TXT",&text[0],26) == false)
			{
				LedOn(LED_RED);
			}
			LedOn(LED_BLUE);
			SD_Disconnect();
		}
	#endif
    ///-------------------------------------------------------------------------
	#ifdef USES_TFT
		FTF_Light(GPIO_PIN_SET);
		Delay(100);
		FTF_Display(GPIO_PIN_RESET);
		Delay(100);
		FTF_Data();
		Delay(100);

		for(cnt_1 = 0;cnt_1 < 272;cnt_1++)
		{
			FTF_DataEnable(GPIO_PIN_RESET);
			for(cnt = 0;cnt < 40;cnt++)
			{
				FTF_Clock();
			}
			FTF_DataEnable(GPIO_PIN_SET);
			for(cnt = 0;cnt < 480;cnt++)
			{
				FTF_Clock();
			}
		}
		LedOn(LED_GREEN);

		FTF_Display(GPIO_PIN_RESET);
	#endif
	///-------------------------------------------------------------------------
    //while(1){}
    ///-------------------------------------------------------------------------
}
///-----------------------------------------------------------------------------
