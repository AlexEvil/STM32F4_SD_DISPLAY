
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"



#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#ifndef RTE_CMSIS_RTOS_RTX
void SVC_Handler(void)
{
}
#endif

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#ifndef RTE_CMSIS_RTOS_RTX
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#ifndef RTE_CMSIS_RTOS_RTX
void SysTick_Handler(void)
{
  HAL_IncTick();
}
#endif

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void WWDG_IRQHandler(void){while(1){}}
void PVD_IRQHandler(void){while(1){}}
void TAMP_STAMP_IRQHandler(void){while(1){}}
void RTC_WKUP_IRQHandler(void){while(1){}}
void FLASH_IRQHandler(void){while(1){}}
void RCC_IRQHandler(void){while(1){}}
void EXTI0_IRQHandler(void){while(1){}}
void EXTI1_IRQHandler(void){while(1){}}
void EXTI2_IRQHandler(void){while(1){}}
void EXTI3_IRQHandler(void){while(1){}}
void EXTI4_IRQHandler(void){while(1){}}
void DMA1_Stream0_IRQHandler(void){while(1){}}
void DMA1_Stream1_IRQHandler(void){while(1){}}
void DMA1_Stream2_IRQHandler(void){while(1){}}
void DMA1_Stream3_IRQHandler(void){while(1){}}
void DMA1_Stream4_IRQHandler(void){while(1){}}
void DMA1_Stream5_IRQHandler(void){while(1){}}
void DMA1_Stream6_IRQHandler(void){while(1){}}
void ADC_IRQHandler(void){while(1){}}
void CAN1_TX_IRQHandler(void){while(1){}}
void CAN1_RX0_IRQHandler(void){while(1){}}
void CAN1_RX1_IRQHandler(void){while(1){}}
void CAN1_SCE_IRQHandler(void){while(1){}}
///-----------------------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
	static int a = 0;
	if(HAL_NVIC_GetActive(EXTI9_5_IRQn) == 1)
	{
		a++;
		HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
		HAL_GPIO_EXTI_IRQHandler(SD_IO_DETECT_PIN);
		//__HAL_GPIO_EXTI_CLEAR_FLAG(SD_IO_DETECT_PIN);
		__HAL_GPIO_EXTI_CLEAR_IT(SD_IO_DETECT_PIN);
	}
}
///-----------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == SD_IO_DETECT_PIN)
	{
		BSP_SD_CardDetect();
	}
}
///-----------------------------------------------------------------------------
void TIM1_BRK_TIM9_IRQHandler(void){while(1){}}
void TIM1_UP_TIM10_IRQHandler(void){while(1){}}
void TIM1_TRG_COM_TIM11_IRQHandler(void){while(1){}}
void TIM1_CC_IRQHandler(void){while(1){}}
void TIM2_IRQHandler(void){while(1){}}
void TIM3_IRQHandler(void){while(1){}}
void TIM4_IRQHandler(void){while(1){}}
void I2C1_EV_IRQHandler(void){while(1){}}
void I2C1_ER_IRQHandler(void){while(1){}}
void I2C2_EV_IRQHandler(void){while(1){}}
void I2C2_ER_IRQHandler(void){while(1){}}
//void SPI1_IRQHandler(void){}
void SPI2_IRQHandler(void){while(1){}}
void USART1_IRQHandler(void){while(1){}}
void USART2_IRQHandler(void){while(1){}}
void USART3_IRQHandler(void){while(1){}}
void EXTI15_10_IRQHandler(void){while(1){}}
void RTC_Alarm_IRQHandler(void){while(1){}}
void OTG_FS_WKUP_IRQHandler(void){while(1){}}
void TIM8_BRK_TIM12_IRQHandler(void){while(1){}}
void TIM8_UP_TIM13_IRQHandler(void){while(1){}}
void TIM8_TRG_COM_TIM14_IRQHandler(void){while(1){}}
void TIM8_CC_IRQHandler(void){while(1){}}
void DMA1_Stream7_IRQHandler(void){while(1){}}
void FMC_IRQHandler(void){while(1){}}
void SDIO_IRQHandler(void){while(1){}}
void TIM5_IRQHandler(void){while(1){}}
void SPI3_IRQHandler(void){while(1){}}
void UART4_IRQHandler(void){while(1){}}
void UART5_IRQHandler(void){while(1){}}
void TIM6_DAC_IRQHandler(void){while(1){}}
void TIM7_IRQHandler(void){while(1){}}
void DMA2_Stream0_IRQHandler(void){while(1){}}
void DMA2_Stream1_IRQHandler(void){while(1){}}
void DMA2_Stream2_IRQHandler(void){while(1){}}
void DMA2_Stream3_IRQHandler(void){while(1){}}
void DMA2_Stream4_IRQHandler(void){while(1){}}
void ETH_IRQHandler(void){while(1){}}
void ETH_WKUP_IRQHandler(void){while(1){}}
void CAN2_TX_IRQHandler(void){while(1){}}
void CAN2_RX0_IRQHandler(void){while(1){}}
void CAN2_RX1_IRQHandler(void){while(1){}}
void CAN2_SCE_IRQHandler(void){while(1){}}
void OTG_FS_IRQHandler(void){while(1){}}
void DMA2_Stream5_IRQHandler(void){while(1){}}
void DMA2_Stream6_IRQHandler(void){while(1){}}
void DMA2_Stream7_IRQHandler(void){while(1){}}
void USART6_IRQHandler(void){while(1){}}
void I2C3_EV_IRQHandler(void){while(1){}}
void I2C3_ER_IRQHandler(void){while(1){}}
void OTG_HS_EP1_OUT_IRQHandler(void){while(1){}}
void OTG_HS_EP1_IN_IRQHandler(void){while(1){}}
void OTG_HS_WKUP_IRQHandler(void){while(1){}}
void OTG_HS_IRQHandler(void){while(1){}}
void DCMI_IRQHandler(void){while(1){}}
void HASH_RNG_IRQHandler(void){while(1){}}
void FPU_IRQHandler(void){while(1){}}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
