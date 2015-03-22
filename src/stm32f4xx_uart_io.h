///-----------------------------------------------------------------------------
#ifndef __STM32F4XX_UART_IO_H
#define __STM32F4XX_UART_IO_H
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include <stdint.h>
///-----------------------------------------------------------------------------
#include "stm32f4xx_hal.h"
///-----------------------------------------------------------------------------     
#define UART_IO                                                            UART4
#define UART_IO_CLK_ENABLE()                                __UART4_CLK_ENABLE()
///-----------------------------------------------------------------------------
#define UART_IO_RX_TX_AF                                          GPIO_AF8_UART4
#define UART_IO_RX_TX_GPIO_PORT                                            GPIOA
#define UART_IO_RX_GPIO_PIN                                           GPIO_PIN_1
#define UART_IO_TX_GPIO_PIN                                           GPIO_PIN_0
#define UART_IO_RX_TX_GPIO_CLK_ENABLE()                     __GPIOA_CLK_ENABLE()
#define UART_IO_RX_TX_GPIO_CLK_DISABLE()                   __GPIOA_CLK_DISABLE()
///-----------------------------------------------------------------------------	 
//#define SD_IO_SPIx_TIMEOUT_MAX                                              1000
///-----------------------------------------------------------------------------
void UART_IO_Init(void);
HAL_StatusTypeDef UART_IO_Write(uint8_t *pData,uint16_t Size,uint32_t Timeout);
///-----------------------------------------------------------------------------     
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif /* __STM32F4XX_UART_IO_H */
///-----------------------------------------------------------------------------
