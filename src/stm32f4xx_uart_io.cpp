///-----------------------------------------------------------------------------
#include "stm32f4xx_uart_io.h"
///-----------------------------------------------------------------------------
static UART_HandleTypeDef UartHandle;
///-----------------------------------------------------------------------------
static void UARTx_Init(void);
static void UARTx_Error(void);
///-----------------------------------------------------------------------------
/**
 * @brief  Initializes UART HAL.
 * @param  None
 * @retval None
 */
static void UARTx_Init(void)
{
    /// UART4 configured as follow:
    ///  - Word Length = 8 Bits
    ///  - Stop Bit = One Stop bit
    ///  - Parity = None
    ///  - BaudRate = 9600 baud
    ///  - Hardware flow control disabled (RTS and CTS signals)
    UartHandle.Instance        = UART_IO;
    UartHandle.Init.BaudRate   = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
    HAL_UART_Init(&UartHandle);
    return;
}
///-----------------------------------------------------------------------------
/**
 * @brief  UART error treatment function.
 * @param  None
 * @retval None
 */
static void UARTx_Error(void)
{
	/// De-initialize the SPI communication BUS
	HAL_UART_DeInit(&UartHandle);
	/// Re-Initiaize the SPI communication BUS
	UARTx_Init();
}
///-----------------------------------------------------------------------------
/**
 * @brief  UART transmite data
 * @param  None
 * @retval None
 */
HAL_StatusTypeDef UART_IO_Write(uint8_t *pData,uint16_t Size,uint32_t Timeout)
{
    HAL_StatusTypeDef status;
    status = HAL_UART_Transmit(&UartHandle,pData,Size,Timeout);
    if(status == HAL_ERROR)
    {
        UARTx_Error();
    }
	return(status);
}
///-----------------------------------------------------------------------------
/**
 * @brief  Initializes the UART
 * @param  None
 * @retval None
 */
void UART_IO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;    
    #ifndef RTE_UART4
//        #error "Invalid RTE_UART4 defination in Configuration!"
    #endif
    #ifdef RTE_UART4_RX_DMA
        #error "Invalid UART. Now not uses DMA Interface!"
    #endif
    #ifdef RTE_UART4_TX_DMA
        #error "Invalid UART. Now not uses DMA Interface!"
    #endif
    /// Enable GPIO TX/RX clock
    UART_IO_RX_TX_GPIO_CLK_ENABLE();
    UART_IO_CLK_ENABLE(); 
    /// Configure GPIO
    GPIO_InitStruct.Pin       = UART_IO_TX_GPIO_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = UART_IO_RX_TX_AF;
    HAL_GPIO_Init(UART_IO_RX_TX_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = UART_IO_RX_GPIO_PIN;
    GPIO_InitStruct.Alternate = UART_IO_RX_TX_AF;
    HAL_GPIO_Init(UART_IO_RX_TX_GPIO_PORT, &GPIO_InitStruct);
	UARTx_Init();
}
///-----------------------------------------------------------------------------
