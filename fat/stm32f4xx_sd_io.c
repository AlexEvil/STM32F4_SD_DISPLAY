///-----------------------------------------------------------------------------
#include "stm32f4xx_sd_io.h"
///-----------------------------------------------------------------------------
#define __STM32F4xx_SD_BSP_VERSION_MAIN (0x01) /*!< [31:24] main version      */
#define __STM32F4xx_SD_BSP_VERSION_SUB1 (0x01) /*!< [23:16] sub1 version      */
#define __STM32F4xx_SD_BSP_VERSION_SUB2 (0x00) /*!< [15:8]  sub2 version      */
#define __STM32F4xx_SD_BSP_VERSION_RC   (0x00) /*!< [7:0]  release candidate  */ 
#define __STM32F4xx_SD_BSP_VERSION ((__STM32F4xx_SD_BSP_VERSION_MAIN << 24)    \
                                   |(__STM32F4xx_SD_BSP_VERSION_SUB1 << 16)    \
                                   |(__STM32F4xx_SD_BSP_VERSION_SUB2 << 8 )    \
                                   |(__STM32F4xx_SD_BSP_VERSION_RC))   
///-----------------------------------------------------------------------------
#define SD_DUMMY_BYTE                                                       0xFF
#define SD_NO_RESPONSE_EXPECTED                                             0x80
///-----------------------------------------------------------------------------
uint32_t SpixTimeout = SD_IO_SPIx_TIMEOUT_MAX;
static SPI_HandleTypeDef handleSD_Spi;
///-----------------------------------------------------------------------------
static void       SPIx_Init(void);
static void       SPIx_Write(uint8_t Value);
static uint32_t   SPIx_Read(void);
static void       SPIx_Error(void);
static void       SPIx_MspInit(SPI_HandleTypeDef *hspi);
///-----------------------------------------------------------------------------
void              SD_IO_Init(void);
HAL_StatusTypeDef SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);
HAL_StatusTypeDef SD_IO_WaitResponse(uint8_t Response);
void              SD_IO_WriteDummy(void);
void              SD_IO_WriteByte(uint8_t Data);
uint8_t           SD_IO_ReadByte(void);
///-----------------------------------------------------------------------------
uint32_t BSP_GetVersion(void)
{
	return __STM32F4xx_SD_BSP_VERSION;
}
///-----------------------------------------------------------------------------
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;  
    /// Configure the GPIOs
    /// Enable GPIO clock
    SD_IO_SPIx_SCK_GPIO_CLK_ENABLE();
    SD_IO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();
    /// Configure SPI SCK
    GPIO_InitStruct.Pin       = SD_IO_SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = SD_IO_SPIx_SCK_AF;
    HAL_GPIO_Init(SD_IO_SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
    /// Configure SPI MISO and MOSI
    GPIO_InitStruct.Pin       = SD_IO_SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SD_IO_SPIx_MISO_MOSI_AF;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    HAL_GPIO_Init(SD_IO_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = SD_IO_SPIx_MISO_PIN;
    HAL_GPIO_Init(SD_IO_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);
    /// Configure the SPI peripheral. Enable SPI clock
    SD_IO_SPIx_CLK_ENABLE();
}
///-----------------------------------------------------------------------------
/**
 * @brief  Initializes SPI HAL.
 * @param  None
 * @retval None
 */
static void SPIx_Init(void)
{
    if(HAL_SPI_GetState(&handleSD_Spi) == HAL_SPI_STATE_RESET)
    {
        /// SPI Config
        handleSD_Spi.Instance = SD_IO_SPIx;
        /// SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz) 
        ///	to verify these constraints:
        ///		- SD card SPI interface max baudrate is 25MHz for write/read
        handleSD_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
        handleSD_Spi.Init.Direction         = SPI_DIRECTION_2LINES;
        handleSD_Spi.Init.CLKPhase          = SPI_PHASE_2EDGE;
        handleSD_Spi.Init.CLKPolarity       = SPI_POLARITY_HIGH;
        handleSD_Spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
        handleSD_Spi.Init.CRCPolynomial     = 7;
        handleSD_Spi.Init.DataSize          = SPI_DATASIZE_8BIT;
        handleSD_Spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
        handleSD_Spi.Init.NSS               = SPI_NSS_SOFT;
        handleSD_Spi.Init.TIMode            = SPI_TIMODE_DISABLED;
        handleSD_Spi.Init.Mode              = SPI_MODE_MASTER;
        SPIx_MspInit(&handleSD_Spi);
        HAL_SPI_Init(&handleSD_Spi);
	}
    return;
}
///-----------------------------------------------------------------------------
/**
 * @brief  SPI Read 4 bytes from device.
 * @param  None
 * @retval Read data
*/
static uint32_t SPIx_Read(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint32_t readvalue = 0;
	uint32_t writevalue = 0xFFFFFFFF;
	status = HAL_SPI_TransmitReceive(&handleSD_Spi, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);
	/// Check the communication status
	if(status != HAL_OK)
	{
		/// Execute user timeout callback
		SPIx_Error();
	}
	return readvalue;
}
///-----------------------------------------------------------------------------
/**
 * @brief  SPI Write a byte to device.
 * @param  Value: value to be written
 * @retval None
 */
static void SPIx_Write(uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_SPI_Transmit(&handleSD_Spi, (uint8_t*) &Value, 1, SpixTimeout);
	/// Check the communication status
	if(status != HAL_OK)
	{
		/// Execute user timeout callback
		SPIx_Error();
	}
}
///-----------------------------------------------------------------------------
/**
 * @brief  SPI error treatment function.
 * @param  None
 * @retval None
 */
static void SPIx_Error (void)
{
	/// De-initialize the SPI communication BUS
	HAL_SPI_DeInit(&handleSD_Spi);
	/// Re-Initiaize the SPI communication BUS
	SPIx_Init();
}
///-----------------------------------------------------------------------------
/**
 * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
 *         data transfer).
 * @param  None
 * @retval None
 */
void SD_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	uint8_t counter;
	/// SD_CS_GPIO Periph clock enable
	SD_CS_GPIO_CLK_ENABLE();
    /// SD_CD_GPIO Periph clock enable
	GPIO_InitStruct.Pin   = SD_CS_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStruct);
	/// Put SD in SPI mode
	/// SD SPI Config
	SPIx_Init();
	/// SD chip select high
	SD_CS_HIGH();
	/// Send dummy byte 0xFF, 10 times with CS high
	/// Rise CS and MOSI for 80 clocks cycles
	for (counter = 0; counter <= 9; counter++)
	{
		/// Send dummy byte 0xFF
		SD_IO_WriteByte(SD_DUMMY_BYTE);
	}
}
///-----------------------------------------------------------------------------
/**
 * @brief  Writes a byte on the SD.
 * @param  Data: byte to send.
 * @retval None
 */
void SD_IO_WriteByte(uint8_t Data)
{
	/// Send the byte
	SPIx_Write(Data);
}
///-----------------------------------------------------------------------------
/**
 * @brief  Reads a byte from the SD.
 * @param  None
 * @retval The received byte.
 */
uint8_t SD_IO_ReadByte(void)
{
	uint8_t data = 0;
	/// Get the received data
	data = SPIx_Read();
	/// Return the shifted data
	return data;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Sends 5 bytes command to the SD card and get response.
 * @param  Cmd: The user expected command to send to SD card.
 * @param  Arg: The command argument
 * @param  Crc: The CRC
 * @param  Response: Expected response from the SD card
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
{
	uint32_t counter = 0x00;
	uint8_t frame[6];
	/// Prepare Frame to send
	frame[0] = (Cmd | 0x40);         /// Construct byte 1
	frame[1] = (uint8_t)(Arg >> 24); /// Construct byte 2
	frame[2] = (uint8_t)(Arg >> 16); /// Construct byte 3
	frame[3] = (uint8_t)(Arg >> 8);  /// Construct byte 4
	frame[4] = (uint8_t)(Arg);       /// Construct byte 5
	frame[5] = (Crc);                /// Construct byte 6
	/// SD chip select low
	SD_CS_LOW();
	/// Send Frame
	for (counter = 0; counter < 6; counter++)
	{
		SD_IO_WriteByte(frame[counter]); /// Send the Cmd bytes
	}
	if(Response != SD_NO_RESPONSE_EXPECTED)
	{
		return SD_IO_WaitResponse(Response);
	}
	return HAL_OK;
}
///-----------------------------------------------------------------------------
/**
 * @brief  Waits response from the SD card
 * @param  Response: Expected response from the SD card
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef SD_IO_WaitResponse(uint8_t Response)
{
	uint32_t timeout = 0xFFFF;
	/// Check if response is got or a timeout is happen
	while ((SD_IO_ReadByte() != Response) && timeout)
	{
		timeout--;
	}
	if (timeout == 0)
	{
		/// After time out
		return HAL_TIMEOUT;
	}
	else
	{
		/// Right response got
		return HAL_OK;
	}
}
///-----------------------------------------------------------------------------
/**
 * @brief  Sends dummy byte with CS High.
 * @param  None
 * @retval None
 */
void SD_IO_WriteDummy(void)
{
	/// SD chip select high
	SD_CS_HIGH();
	/// Send Dummy byte 0xFF
	SD_IO_WriteByte(SD_DUMMY_BYTE);
}
///-----------------------------------------------------------------------------
