///-----------------------------------------------------------------------------
#ifndef __STM32F4XX_SD_IO_H
#define __STM32F4XX_SD_IO_H
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include "stm32f4xx_hal.h"
///-----------------------------------------------------------------------------   
#define SD_IO_SPIx                                                          SPI1
#define SD_IO_SPIx_CLK_ENABLE()                              __SPI1_CLK_ENABLE()
///-----------------------------------------------------------------------------
#define SD_IO_DETECT_PIN                                              GPIO_PIN_6
#define SD_IO_DETECT_GPIO_PORT                                             GPIOB
#define SD_IO_DETECT_GPIO_CLK_ENABLE()                      __GPIOB_CLK_ENABLE()
///-----------------------------------------------------------------------------
#define SD_IO_PROTECT_PIN                                             GPIO_PIN_7
#define SD_IO_PROTECT_GPIO_PORT                                            GPIOB
#define SD_IO_PROTECT_GPIO_CLK_ENABLE()                     __GPIOB_CLK_ENABLE()
///-----------------------------------------------------------------------------	 
#define SD_IO_SPIx_SCK_AF                                          GPIO_AF5_SPI1
#define SD_IO_SPIx_SCK_GPIO_PORT                                           GPIOA
#define SD_IO_SPIx_SCK_PIN                                            GPIO_PIN_5
#define SD_IO_SPIx_SCK_GPIO_CLK_ENABLE()                    __GPIOA_CLK_ENABLE()
#define SD_IO_SPIx_SCK_GPIO_CLK_DISABLE()                  __GPIOA_CLK_DISABLE()
///-----------------------------------------------------------------------------
#define SD_IO_SPIx_MISO_MOSI_AF                                    GPIO_AF5_SPI1
#define SD_IO_SPIx_MISO_MOSI_GPIO_PORT                                     GPIOA
#define SD_IO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()              __GPIOA_CLK_ENABLE()
#define SD_IO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()            __GPIOA_CLK_DISABLE()
#define SD_IO_SPIx_MISO_PIN                                           GPIO_PIN_6
#define SD_IO_SPIx_MOSI_PIN                                           GPIO_PIN_7
///-----------------------------------------------------------------------------
/// Maximum Timeout values for flags waiting loops. These timeouts are not based
/// on accurate values, they just guarantee that the application will not remain
/// stuck if the SPI communication is corrupted. You may modify these timeout
/// values depending on CPU frequency and application conditions
/// (interrupts routines ...).
#define SD_IO_SPIx_TIMEOUT_MAX                                              1000
///-----------------------------------------------------------------------------
#define SD_CS_LOW() HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()  HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)
///-----------------------------------------------------------------------------
#define SD_CS_PIN                                                    GPIO_PIN_15
#define SD_CS_GPIO_PORT                                                    GPIOA
#define SD_CS_GPIO_CLK_ENABLE()                             __GPIOA_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                           __GPIOA_CLK_DISABLE()
///-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif /* __STM32F4XX_SD_IO_H */
///-----------------------------------------------------------------------------
