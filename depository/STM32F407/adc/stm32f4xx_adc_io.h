///-----------------------------------------------------------------------------
#ifndef __STM32F4XX_ADC_IO_H
#define __STM32F4XX_ADC_IO_H
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include "stm32f4xx_hal.h"
#include <stddef.h>
///-----------------------------------------------------------------------------   
#define ADC_IO_SPIx                                                         SPI3
#define ADC_IO_SPIx_CLK_ENABLE()                             __SPI3_CLK_ENABLE()
///-----------------------------------------------------------------------------
#define ADC_IO_SPIx_SCK_AF                                         GPIO_AF6_SPI3
#define ADC_IO_SPIx_SCK_GPIO_PORT                                          GPIOB
#define ADC_IO_SPIx_SCK_PIN                                           GPIO_PIN_3
#define ADC_IO_SPIx_SCK_GPIO_CLK_ENABLE()                   __GPIOB_CLK_ENABLE()
#define ADC_IO_SPIx_SCK_GPIO_CLK_DISABLE()                 __GPIOB_CLK_DISABLE()
///-----------------------------------------------------------------------------
#define ADC_IO_SPIx_MISO_MOSI_AF                                   GPIO_AF6_SPI3
#define ADC_IO_SPIx_MISO_MOSI_GPIO_PORT                                    GPIOB
#define ADC_IO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()             __GPIOB_CLK_ENABLE()
#define ADC_IO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()           __GPIOB_CLK_DISABLE()
#define ADC_IO_SPIx_MISO_PIN                                          GPIO_PIN_4
#define ADC_IO_SPIx_MOSI_PIN                                          GPIO_PIN_5
///-----------------------------------------------------------------------------
#define ADC_IO_CS_1_PIN                                               GPIO_PIN_9
#define ADC_IO_CS_1_GPIO_PORT                                              GPIOE
#define ADC_IO_CS_1_GPIO_CLK_ENABLE()                       __GPIOE_CLK_ENABLE()
#define ADC_IO_CS_1_GPIO_CLK_DISABLE()                     __GPIOE_CLK_DISABLE()
///-----------------------------------------------------------------------------
#define ADC_IO_CS_2_PIN                                              GPIO_PIN_10
#define ADC_IO_CS_2_GPIO_PORT                                              GPIOE
#define ADC_IO_CS_2_GPIO_CLK_ENABLE()                       __GPIOE_CLK_ENABLE()
#define ADC_IO_CS_2_GPIO_CLK_DISABLE()                     __GPIOE_CLK_DISABLE()
///-----------------------------------------------------------------------------
#define ADC_IO_SYNC_1_PIN                                             GPIO_PIN_7
#define ADC_IO_SYNC_1_GPIO_PORT                                            GPIOE
#define ADC_IO_SYNC_1_GPIO_CLK_ENABLE()                     __GPIOE_CLK_ENABLE()
#define ADC_IO_SYNC_2_PIN                                             GPIO_PIN_8
#define ADC_IO_SYNC_2_GPIO_PORT                                            GPIOE
#define ADC_IO_SYNC_2_GPIO_CLK_ENABLE()                     __GPIOE_CLK_ENABLE()
///-----------------------------------------------------------------------------
#define ADC_IO_DRDY_1_PIN                                            GPIO_PIN_11
#define ADC_IO_DRDY_1_GPIO_PORT                                            GPIOE
#define ADC_IO_DRDY_1_GPIO_CLK_ENABLE()                     __GPIOE_CLK_ENABLE()
#define ADC_IO_DRDY_2_PIN                                            GPIO_PIN_12
#define ADC_IO_DRDY_2_GPIO_PORT                                            GPIOE
#define ADC_IO_DRDY_2_GPIO_CLK_ENABLE()                     __GPIOE_CLK_ENABLE()
///-----------------------------------------------------------------------------
#define ADC_IO_CS_1_LOW()  HAL_GPIO_WritePin(ADC_IO_CS_1_GPIO_PORT,ADC_IO_CS_1_PIN,GPIO_PIN_RESET)
#define ADC_IO_CS_1_HIGH() HAL_GPIO_WritePin(ADC_IO_CS_1_GPIO_PORT,ADC_IO_CS_1_PIN,GPIO_PIN_SET)
///-----------------------------------------------------------------------------
#define ADC_IO_CS_2_LOW()  HAL_GPIO_WritePin(ADC_IO_CS_2_GPIO_PORT,ADC_IO_CS_2_PIN,GPIO_PIN_RESET)
#define ADC_IO_CS_2_HIGH() HAL_GPIO_WritePin(ADC_IO_CS_2_GPIO_PORT,ADC_IO_CS_2_PIN,GPIO_PIN_SET)
///-----------------------------------------------------------------------------
#define ADC_IO_SYNC_1_LOW()  HAL_GPIO_WritePin(ADC_IO_SYNC_1_GPIO_PORT,ADC_IO_SYNC_1_PIN,GPIO_PIN_RESET)
#define ADC_IO_SYNC_1_HIGH() HAL_GPIO_WritePin(ADC_IO_SYNC_1_GPIO_PORT,ADC_IO_SYNC_1_PIN,GPIO_PIN_SET)
///-----------------------------------------------------------------------------
#define ADC_IO_SYNC_2_LOW()  HAL_GPIO_WritePin(ADC_IO_SYNC_2_GPIO_PORT,ADC_IO_SYNC_2_PIN,GPIO_PIN_RESET)
#define ADC_IO_SYNC_2_HIGH() HAL_GPIO_WritePin(ADC_IO_SYNC_2_GPIO_PORT,ADC_IO_SYNC_2_PIN,GPIO_PIN_SET)
///-----------------------------------------------------------------------------
#define ADC_IO_DRDY_1_GET() HAL_GPIO_ReadPin(ADC_IO_DRDY_1_GPIO_PORT,ADC_IO_DRDY_1_PIN)
#define ADC_IO_DRDY_2_GET() HAL_GPIO_ReadPin(ADC_IO_DRDY_2_GPIO_PORT,ADC_IO_DRDY_2_PIN)
///-----------------------------------------------------------------------------
#define ADC_IO_SPIx_TIMEOUT_MAX                                               10
///-----------------------------------------------------------------------------
#define ADC_IO_CS_DELAY                                                        1
#define ADC_IO_DUMMY_BYTE                                                   0xFF
#define ADC_IO_NO_RESPONSE_EXPECTED                                         0x80
///-----------------------------------------------------------------------------
typedef enum
{
	ADC_CHIP_A = 0x01,
	ADC_CHIP_B = 0x02
}ADC_IO_CHIP;
///-----------------------------------------------------------------------------
extern void    ADC_IO_Init(void);
extern void    ADC_IO_WriteDummy(void);
extern void    ADC_IO_Delay(uint32_t ms);
extern uint8_t ADC_IO_ReadByte(ADC_IO_CHIP chip);
extern void    ADC_IO_WriteByte(ADC_IO_CHIP chip,uint8_t Data);
///-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif /* __STM32F4XX_ADC_IO_H */
///-----------------------------------------------------------------------------
