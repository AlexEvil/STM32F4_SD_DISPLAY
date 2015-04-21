///-----------------------------------------------------------------------------
#include "Led.h"
///-----------------------------------------------------------------------------
#include <cmsis_os.h>
///-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
///-----------------------------------------------------------------------------
#include "sd_storage.h"
#include "../adc/ad7714a.h"
///-----------------------------------------------------------------------------
///#define USES_SD
///#define USES_ADC
///-----------------------------------------------------------------------------
#ifdef RTE_CMSIS_RTOS_RTX
	extern uint32_t os_time;
	uint32_t HAL_GetTick(void)
	{
		return os_time;
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
	RCC_OscInitStruct.HSEState      = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
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
	if(osKernelInitialize() != osOK)
	{
		while(1);
	}
	if(osKernelStart() != osOK)
	{
		while(1);
	}
	
}
///-----------------------------------------------------------------------------
int main(void)
{
	///-------------------------------------------------------------------------
	#ifdef USES_ADC
		char buffer[255];
		uint32_t length = 10;
		AdcSettings Settings;
		uint32_t AdcDataChipA = 0xFFFFFFFF;
		uint16_t cnt = 0;
	#endif
	///-------------------------------------------------------------------------
	MainSystemInit();
	LedInit();
	///-------------------------------------------------------------------------
	#ifdef USES_SD
		if(SD_Connect() == false)
		{
			LedOn(LED_GREEN);
		}
	#endif
	///-------------------------------------------------------------------------
	#ifdef USES_ADC
		AdcInit();
		Settings.Calibration  = ADC_MODE_SELF;             /// Calibration Type
		Settings.Gain         = ADC_MODE_GAIN_1;           /// Gain Amplifaer
		Settings.BurnoutOnOff = ADC_MODE_BURNOUT_OFF;      /// Burnout current On /Off
		Settings.FilterOnOff  = ADC_MODE_FILTER_OFF;       /// Digital filter On/Off
		Settings.Channel      = ADC_COMM_CHANNEL_1_2;      /// Select Channel
		Settings.Polar        = ADC_FILTERH_UNIPOLAR;      /// Type operation Bipolar/Unipolar
		Settings.WordLength   = ADC_FILTERH_LENGTH_24_BIT; /// Data word length 16/24 bit
		Settings.Boost        = ADC_FILTERH_BST_OFF;       /// Current Boost On/Off
		Settings.FilterHz     = 5;                         /// Digital filter response
		AdcWriteSettings(ADC_CHIP_A,&Settings);
		LedOn(LED_ORANGE);
		while(cnt < 1000)
		{
			AdcCalibration(ADC_CHIP_A,ADC_COMM_CHANNEL_1_2,ADC_MODE_BURNOUT_OFF | ADC_MODE_GAIN_1 | ADC_MODE_FILTER_OFF);
			AdcDataChipA = AdcReadData(ADC_CHIP_A,ADC_COMM_CHANNEL_1_2,ADC_FILTERH_LENGTH_24_BIT);
			AdcDataChipA = AdcCalculate(AdcDataChipA);
			AdcBinToASCII(AdcDataChipA,&buffer[0]);
			#ifdef USES_SD
				if(SD_WriteLn("E_V_I_L_TEST.TXT",&buffer[0],length) == false)
				{
					LedOn(LED_RED);
					break;
				}
			#endif
			HAL_Delay(1);
			cnt++;
		}
	#endif
	///-------------------------------------------------------------------------
	LedOn(LED_BLUE);
	#ifdef USES_SD
		SD_Disconnect();
	#endif
	///-------------------------------------------------------------------------
	while(1){}
	///-------------------------------------------------------------------------
}
///-----------------------------------------------------------------------------




