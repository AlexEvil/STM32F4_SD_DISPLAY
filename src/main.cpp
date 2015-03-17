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
///-----------------------------------------------------------------------------
#ifdef RTE_CMSIS_RTOS_RTX
    extern uint32_t os_time;
    uint32_t HAL_GetTick(void)
    {
        return os_time;
    }
#endif
///-----------------------------------------------------------------------------    
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
    while(1)
    {
    }
}
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
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /// Select PLL as system clock source and configure the
    /// HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
    return;
}
///-----------------------------------------------------------------------------
void MainSystemInit(void)
{
    HAL_StatusTypeDef hal_status;
    hal_status = HAL_Init();
    SystemClock_Config();
    if(hal_status != HAL_OK)
    {
        Error_Handler();
    }
    if(osKernelInitialize() != osOK)
    {
        Error_Handler();
    }
    if(osKernelStart() != osOK)
    {
        Error_Handler();
    }
    return;
}
///-----------------------------------------------------------------------------
int main(void)
{
    char text[] = "thise is new version board";	
    SD_CardInfo pCardInfo;
    uint32_t PreemptPriority = 0;
    uint32_t SubPriority = 1;
    ///-------------------------------------------------------------------------
    MainSystemInit();
    LedInit();
	osDelay(500);

			HAL_NVIC_SetPriority(EXTI9_5_IRQn,PreemptPriority,SubPriority);
			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    ///-------------------------------------------------------------------------
    #ifdef USES_SD
        if(SD_Connect() == false)
        {
            LedOn(LED_RED);
        }
		else
		{

		}
    #endif
    ///-------------------------------------------------------------------------
    #ifdef USES_SD
        BSP_SD_GetCardInfo(&pCardInfo);
        
        if(SD_WriteLn("E_V_I_L_TEST.TXT",&text[0],sizeof(text)) == false)
        {
            LedOn(LED_RED);
        }
        else
        {
            LedOn(LED_BLUE);
        }
    #endif
    ///-------------------------------------------------------------------------
    #ifdef USES_SD
        SD_Disconnect();
    #endif
    ///-------------------------------------------------------------------------
    while(1)
    {
        osDelay(200);
        LedOn(LED_GREEN);
        osDelay(200);
        LedOff(LED_GREEN);
    }
    ///-------------------------------------------------------------------------
}
///-----------------------------------------------------------------------------
