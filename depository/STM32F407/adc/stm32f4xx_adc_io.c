///-----------------------------------------------------------------------------
#include "stm32f4xx_adc_io.h"
#include <cmsis_os.h>
///-----------------------------------------------------------------------------
#define __STM32F4xx_ADC_IO_BSP_VERSION_MAIN  (0x01) /*!< [31:24] main version */
#define __STM32F4xx_ADC_IO_BSP_VERSION_SUB1  (0x01) /*!< [23:16] sub1 version */
#define __STM32F4xx_ADC_IO_BSP_VERSION_SUB2  (0x00) /*!< [15:8]  sub2 version */
#define __STM32F4xx_ADC_IO_BSP_VERSION_RC    (0x00) /*!< [7:0] release candidate */ 
#define __STM32F4xx_ADC_IO_BSP_VERSION ((__STM32F4xx_ADC_IO_BSP_VERSION_MAIN << 24)\
                                       |(__STM32F4xx_ADC_IO_BSP_VERSION_SUB1 << 16)\
                                       |(__STM32F4xx_ADC_IO_BSP_VERSION_SUB2 << 8 )\
                                       |(__STM32F4xx_ADC_IO_BSP_VERSION_RC))   
///-----------------------------------------------------------------------------
uint32_t AdcSpiTimeout = ADC_IO_SPIx_TIMEOUT_MAX;
static   SPI_HandleTypeDef handleADC_Spi;
///-----------------------------------------------------------------------------
static void     SPIx_Init(void);
static uint32_t SPIx_Read(void);
static void     SPIx_Error(void);
static void     SPIx_Write(uint8_t Value);
static void     SPIx_MspInit(SPI_HandleTypeDef *hspi);
///-----------------------------------------------------------------------------
void    ADC_IO_Init(void);
void    ADC_IO_WriteDummy(void);
uint8_t ADC_IO_ReadByte(ADC_IO_CHIP chip);
void    ADC_IO_WriteByte(ADC_IO_CHIP chip,uint8_t Data);
///-----------------------------------------------------------------------------
uint32_t ADC_GetVersion(void)
{
	return __STM32F4xx_ADC_IO_BSP_VERSION;
}
///-----------------------------------------------------------------------------
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;  
	/// Configure the GPIOs
	/// Enable GPIO clock
	ADC_IO_SPIx_SCK_GPIO_CLK_ENABLE();
	ADC_IO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();
	/// Configure SPI SCK
	GPIO_InitStruct.Pin       = ADC_IO_SPIx_SCK_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = ADC_IO_SPIx_SCK_AF;
	HAL_GPIO_Init(ADC_IO_SPIx_SCK_GPIO_PORT,&GPIO_InitStruct);
	/// Configure SPI MISO and MOSI
	GPIO_InitStruct.Pin       = ADC_IO_SPIx_MOSI_PIN;
	GPIO_InitStruct.Alternate = ADC_IO_SPIx_MISO_MOSI_AF;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	HAL_GPIO_Init(ADC_IO_SPIx_MISO_MOSI_GPIO_PORT,&GPIO_InitStruct);
	GPIO_InitStruct.Pin       = ADC_IO_SPIx_MISO_PIN;
	HAL_GPIO_Init(ADC_IO_SPIx_MISO_MOSI_GPIO_PORT,&GPIO_InitStruct);
	/// Configure the SPI peripheral
	/// Enable SPI clock
	ADC_IO_SPIx_CLK_ENABLE();
}
///-----------------------------------------------------------------------------
static void SPIx_Init(void)
{
	if(HAL_SPI_GetState(&handleADC_Spi) == HAL_SPI_STATE_RESET)
	{
		/// SPI Config
		handleADC_Spi.Instance = ADC_IO_SPIx;
		handleADC_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
		handleADC_Spi.Init.Direction         = SPI_DIRECTION_2LINES;
		handleADC_Spi.Init.CLKPhase          = SPI_PHASE_2EDGE;
		handleADC_Spi.Init.CLKPolarity       = SPI_POLARITY_HIGH;//SPI_POLARITY_LOW;
		handleADC_Spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
		handleADC_Spi.Init.CRCPolynomial     = 0;
		handleADC_Spi.Init.DataSize          = SPI_DATASIZE_8BIT;
		handleADC_Spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
		handleADC_Spi.Init.NSS               = SPI_NSS_SOFT;
		handleADC_Spi.Init.TIMode            = SPI_TIMODE_DISABLED;//SPI_TIMODE_ENABLED;
		handleADC_Spi.Init.Mode              = SPI_MODE_MASTER;
		SPIx_MspInit(&handleADC_Spi);
		HAL_SPI_Init(&handleADC_Spi);
	}
}
///-----------------------------------------------------------------------------
static uint32_t SPIx_Read(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint32_t readvalue  = 0;
	uint32_t writevalue = 0xFFFFFFFF;
	status = HAL_SPI_TransmitReceive(&handleADC_Spi,(uint8_t*)&writevalue,(uint8_t*)&readvalue,1,AdcSpiTimeout);
	/// Check the communication status
	if(status != HAL_OK)
	{
		/// Execute user timeout callback
		SPIx_Error();
	}
	return readvalue;
}
///-----------------------------------------------------------------------------
static void SPIx_Write(uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_SPI_Transmit(&handleADC_Spi,(uint8_t*)&Value,1,AdcSpiTimeout);
	/// Check the communication status
	if(status != HAL_OK)
	{
		/// Execute user timeout callback
		SPIx_Error();
	}
}
///-----------------------------------------------------------------------------
static void SPIx_Error (void)
{
	/// De-initialize the SPI communication BUS
	HAL_SPI_DeInit(&handleADC_Spi);
	/// Re-Initiaize the SPI communication BUS
	SPIx_Init();
}
///-----------------------------------------------------------------------------
void ADC_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/// ADC_CS_1_GPIO Periph clock enable
	ADC_IO_CS_1_GPIO_CLK_ENABLE();
	/// ADC_CS_2_GPIO Periph clock enable
	ADC_IO_CS_2_GPIO_CLK_ENABLE();
	/// ADC_SYNC_1_GPIO Periph clock enable
	ADC_IO_SYNC_1_GPIO_CLK_ENABLE();
	/// ADC_SYNC_2_GPIO Periph clock enable
	ADC_IO_SYNC_2_GPIO_CLK_ENABLE();
	/// ADC_DRDY_1_GPIO Periph clock enable
	ADC_IO_DRDY_1_GPIO_CLK_ENABLE();
	/// ADC_DRDY_2_GPIO Periph clock enable
	ADC_IO_DRDY_2_GPIO_CLK_ENABLE();
	/// Configure ADC_CS_1_PIN pin
	GPIO_InitStruct.Pin   = ADC_IO_CS_1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_IO_CS_1_GPIO_PORT,&GPIO_InitStruct);
	/// Configure ADC_CS_2_PIN pin
	GPIO_InitStruct.Pin   = ADC_IO_CS_2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_IO_CS_2_GPIO_PORT,&GPIO_InitStruct);
	/// Configure ADC_SYNC_1_PIN pin
	GPIO_InitStruct.Pin   = ADC_IO_SYNC_1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_IO_SYNC_1_GPIO_PORT,&GPIO_InitStruct);
	/// Configure ADC_SYNC_2_PIN pin
	GPIO_InitStruct.Pin   = ADC_IO_SYNC_2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_IO_SYNC_2_GPIO_PORT,&GPIO_InitStruct);
	/// Configure ADC_DRDY_1_PIN pin
	GPIO_InitStruct.Pin   = ADC_IO_DRDY_1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_IO_DRDY_1_GPIO_PORT,&GPIO_InitStruct);
	/// Configure ADC_DRDY_2_PIN pin
	GPIO_InitStruct.Pin   = ADC_IO_DRDY_2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_IO_DRDY_2_GPIO_PORT,&GPIO_InitStruct);
	/// Put ADC in SPI mode
	/// ADC SPI Config
	SPIx_Init();
	/// ADCs chip select high
	ADC_IO_CS_1_HIGH();
	ADC_IO_CS_2_HIGH();
}
///-----------------------------------------------------------------------------
void ADC_IO_WriteDummy(void)
{
	/// Send Dummy byte 0xFF
	SPIx_Write(ADC_IO_DUMMY_BYTE);
	return;
}
///-----------------------------------------------------------------------------
void ADC_IO_WriteByte(ADC_IO_CHIP chip,uint8_t Data)
{
	/// Set ADC Chip Select ----------------------------------------------------
	switch(chip)
	{
		case ADC_CHIP_A : ADC_IO_CS_1_LOW(); break;
		case ADC_CHIP_B : ADC_IO_CS_2_LOW(); break;
		default : return;
	}
	/// Send the byte
	SPIx_Write(Data);
	/// Clear ADC Chip Select --------------------------------------------------
	switch(chip)
	{
		case ADC_CHIP_A : ADC_IO_CS_1_HIGH(); break;
		case ADC_CHIP_B : ADC_IO_CS_2_HIGH(); break;
		default : return;
	}
	return;
}
///-----------------------------------------------------------------------------
uint8_t ADC_IO_ReadByte(ADC_IO_CHIP chip)
{
	uint8_t data = 0;
	/// Set ADC Chip Select ----------------------------------------------------
	switch(chip)
	{
        case ADC_CHIP_A : ADC_IO_CS_1_LOW(); break;
        case ADC_CHIP_B : ADC_IO_CS_2_LOW(); break;
	}
	/// Get the received data
	data = SPIx_Read();
	/// Clear ADC Chip Select --------------------------------------------------
	switch(chip)
	{
		case ADC_CHIP_A : ADC_IO_CS_1_HIGH(); break;
		case ADC_CHIP_B : ADC_IO_CS_2_HIGH(); break;
	}
	/// Return the shifted data
	return data;
}
///-----------------------------------------------------------------------------
void ADC_IO_Delay(uint32_t ms)
{
	HAL_Delay(ms);
	return;
}
///-----------------------------------------------------------------------------
