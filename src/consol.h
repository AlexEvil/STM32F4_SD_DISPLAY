///-----------------------------------------------------------------------------
#ifndef CONSOL_H
#define CONSOL_H
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
///-----------------------------------------------------------------------------
#include "stm32f4xx_hal.h"
///-----------------------------------------------------------------------------     
extern UART_HandleTypeDef ConsolHandle;
///-----------------------------------------------------------------------------
#define CONSOL_IO_UARTx                                                    UART4
#define CONSOL_IO_UARTx_CLK_ENABLE()                              __SPI1_CLK_ENABLE()
///-----------------------------------------------------------------------------
//#define SD_IO_DETECT_PIN                                              GPIO_PIN_6
//#define SD_IO_DETECT_GPIO_PORT                                             GPIOB
//#define SD_IO_DETECT_GPIO_CLK_ENABLE()                      __GPIOB_CLK_ENABLE()
///-----------------------------------------------------------------------------	 
//#define SD_IO_SPIx_SCK_AF                                          GPIO_AF5_SPI1
//#define SD_IO_SPIx_SCK_GPIO_PORT                                           GPIOA
//#define SD_IO_SPIx_SCK_PIN                                            GPIO_PIN_5
//#define SD_IO_SPIx_SCK_GPIO_CLK_ENABLE()                    __GPIOA_CLK_ENABLE()
//#define SD_IO_SPIx_SCK_GPIO_CLK_DISABLE()                  __GPIOA_CLK_DISABLE()
///-----------------------------------------------------------------------------

     
enum
{
    BACKSPACE = 0x08,
    LF        = 0x0A,
    CR        = 0x0D,
    CNTLQ     = 0x11,
    CNTLS     = 0x13,
    ESC       = 0x1B,
    DEL       = 0x7F
};
///-----------------------------------------------------------------------------
/// External functions
//extern bool getline  (char *buf, int32_t bufsz);

void ConsolInit(void);
void ConsolTest(char* cmd,uint16_t length);
///-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif /// #ifndef CONSOL_H
///-----------------------------------------------------------------------------
