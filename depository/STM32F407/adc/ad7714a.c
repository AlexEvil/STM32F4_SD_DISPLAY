///-----------------------------------------------------------------------------
#include "ad7714a.h"
///-----------------------------------------------------------------------------
#include <cmsis_os.h>
///-----------------------------------------------------------------------------
#define ADC_TIME_RESET												        1000
#define ADC_CLK_FREQUENCY                                              2457600.0
#define ADC_DATA_LSB_NANO_VOLT                                               149
///-----------------------------------------------------------------------------
uint8_t  AdcReadReg(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel);
uint32_t AdcReadRegWord(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel);
///-----------------------------------------------------------------------------
void AdcWriteReg(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel,uint8_t data);
void AdcWriteRegWord(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel,uint32_t data);
///-----------------------------------------------------------------------------
void AdcWriteReg(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel,uint8_t data)
{
	uint8_t Reg;
	uint8_t Data;
	Data = data;
	Reg  = reg | ADC_COMM_WRITE | channel;
	ADC_IO_WriteByte(chip,Reg);
	ADC_IO_WriteByte(chip,Data);
	return;
}
///-----------------------------------------------------------------------------
void AdcWriteRegWord(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel,uint32_t data)
{
	uint8_t Reg;
	uint8_t Data;
	Reg  = reg | ADC_COMM_WRITE | channel;
	/// write to communication register data
	ADC_IO_WriteByte(chip,Reg);
	/// write H byte register data
	Data = (uint8_t)(data >> 16);
	ADC_IO_WriteByte(chip,Data);
	/// write M byte register data
	Data = (uint8_t)(data >> 8);
	ADC_IO_WriteByte(chip,Data);
	/// write L byte register data
	Data = (uint8_t)(data);
	ADC_IO_WriteByte(chip,Data);
	return;
}
///-----------------------------------------------------------------------------
uint8_t AdcReadReg(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel)
{
	uint8_t Reg;
	uint8_t Response;
	Reg  = reg | ADC_COMM_READ | channel;
	ADC_IO_WriteByte(chip,Reg);
	Response = ADC_IO_ReadByte(chip);
	return(Response);
}
///-----------------------------------------------------------------------------
/// read from 24-bits registers data -------------------------------------------
///-----------------------------------------------------------------------------
uint32_t AdcReadRegWord(ADC_IO_CHIP chip,uint8_t reg,uint8_t channel)
{
	uint8_t  Reg;
	uint8_t  Response;
	uint32_t Ret;
	Ret = 0x00000000;
	Reg  = reg | ADC_COMM_READ | channel;
	/// write to communication register data
	ADC_IO_WriteByte(chip,Reg);
	/// read H byte register data
	Response = ADC_IO_ReadByte(chip);
	Ret  |= Response;
	Ret <<= 8;
	/// read M byte register data
	Response = ADC_IO_ReadByte(chip);
	Ret  |= Response;
	Ret <<= 8;
	/// read L byte register data
	Response = ADC_IO_ReadByte(chip);
	Ret  |= Response;
	return(Ret);
}
///-----------------------------------------------------------------------------
///-------------------------- HIGH LEVEL ADC FUNCTION --------------------------
///-----------------------------------------------------------------------------
void AdcInit(void)
{
	ADC_IO_Init();
	ADC_IO_SYNC_1_HIGH();
	ADC_IO_SYNC_2_HIGH();
	AdcReset(ADC_CHIP_A);
	AdcReset(ADC_CHIP_B);
	return;
}
///-----------------------------------------------------------------------------
void AdcReset(ADC_IO_CHIP chip)
{
	register uint32_t cnt = 0;
	/// Set ADC Chip Select
	switch(chip)
	{
		case ADC_CHIP_A : ADC_IO_CS_1_LOW(); break;
		case ADC_CHIP_B : ADC_IO_CS_2_LOW(); break;
		default : return;
	}
	/// send 0xFF to ADC for software reset
    while(cnt < 4)
	{
		ADC_IO_WriteDummy();
	    cnt++;
	}
	/// Clear ADC Chip Select
	switch(chip)
	{
		case ADC_CHIP_A : ADC_IO_CS_1_HIGH(); break;
		case ADC_CHIP_B : ADC_IO_CS_2_HIGH(); break;
		default : return;
	}
	ADC_IO_Delay(ADC_TIME_RESET);
	return;
}
///-----------------------------------------------------------------------------
void AdcCalibration(ADC_IO_CHIP chip,uint8_t channel,uint8_t data)
{
	/// set calibration
	AdcWriteReg(chip,ADC_COMM_MODE,channel,data);
	/// wait while calibration the end
	switch(chip)
	{
		case ADC_CHIP_A :
						  while(1){if(ADC_IO_DRDY_1_GET() == GPIO_PIN_RESET){break;}}
						  while(1){if(ADC_IO_DRDY_1_GET() == GPIO_PIN_SET)  {break;}}
						  while(1){if(ADC_IO_DRDY_1_GET() == GPIO_PIN_RESET){break;}}
			              break;
		case ADC_CHIP_B :
						  while(1){if(ADC_IO_DRDY_2_GET() == GPIO_PIN_RESET){break;}}
						  while(1){if(ADC_IO_DRDY_2_GET() == GPIO_PIN_SET)  {break;}}
						  while(1){if(ADC_IO_DRDY_2_GET() == GPIO_PIN_RESET){break;}}
			              break;
		default : return;
	}
	return;
}
///-----------------------------------------------------------------------------
uint32_t AdcReadData(ADC_IO_CHIP chip,uint8_t channel,const uint8_t length)
{
	uint8_t  Reg;
	uint8_t  Response;
	uint32_t Data;
	Reg  = ADC_COMM_DATA | ADC_COMM_READ | channel;
	/// Wait DRDY ADC Chip Select
	switch(chip)
	{
		case ADC_CHIP_A : while(1){if(ADC_IO_DRDY_1_GET() == GPIO_PIN_RESET){break;}}
		                  break;
		case ADC_CHIP_B :
						  while(1){if(ADC_IO_DRDY_2_GET() == GPIO_PIN_RESET){break;}}
		                  break;
		default : return(0x00);
	}
	/// write to communication register data
	ADC_IO_WriteByte(chip,Reg);
	Response = ADC_IO_ReadByte(chip);
	Data   = Response;
	Data <<= 8;
	Response = ADC_IO_ReadByte(chip);
	Data  |= Response;
	if(length == ADC_FILTERH_LENGTH_24_BIT)
	{
		Data <<= 8;
		Response = ADC_IO_ReadByte(chip);
		Data  |= Response;
	}
	return(Data);
}
///-----------------------------------------------------------------------------
void AdcWriteSettings(ADC_IO_CHIP chip,AdcSettings* Settings)
{
	float    FilterFreq;
	uint16_t FilterWord;
	uint8_t  FilterFreqH;
	uint8_t  FilterFreqL;
	uint8_t  RegisterData;
	uint8_t  Calibration;
	uint8_t  Gain;
	uint8_t  BurnoutOnOff;
	uint8_t  FilterOnOff;
	uint8_t  Channel;
	uint8_t  Polar;
	uint8_t  WordLength;
	uint8_t  Boost;
	float    FilterFreqValue;
	/// coipy settings value
	FilterFreqValue = Settings->FilterHz;
	Calibration     = Settings->Calibration;
	Gain            = Settings->Gain;
	BurnoutOnOff    = Settings->BurnoutOnOff;
	FilterOnOff     = Settings->FilterOnOff;
	Channel         = Settings->Channel;
	Polar           = Settings->Polar;
	WordLength      = Settings->WordLength;
	Boost           = Settings->Boost;
	/// calculate HIGH and LOW filter frequency value
	FilterFreq  = (float)ADC_CLK_FREQUENCY;
	FilterFreq /= (float)128.0;
	FilterFreq /= (float)FilterFreqValue;
	FilterWord  = (uint16_t)FilterFreq;
	/// ckeck inputing data
	if(FilterWord < 19)
	{
		FilterWord = 19;
	}
	else
	{
		if(FilterWord > 4000)
		{
			FilterWord = 4000;
		}
	}
	switch(Gain)
	{
		case ADC_MODE_GAIN_8   :
		case ADC_MODE_GAIN_16  :
		case ADC_MODE_GAIN_32  :
		case ADC_MODE_GAIN_64  :
		case ADC_MODE_GAIN_128 :
                                 Boost = ADC_FILTERH_BST_ON;
			                     break;
	}
	FilterFreqH = (uint8_t)(FilterWord >> 8);
	FilterFreqL = (uint8_t)(FilterWord);
	/// write to high filter register current settings
	RegisterData = FilterFreqH | Polar | WordLength | Boost;
	AdcWriteReg(chip,ADC_COMM_FILTERH,Channel,RegisterData);
	/// write to low filter register current settings
	RegisterData = FilterFreqL | Polar | WordLength | Boost;
	AdcWriteReg(chip,ADC_COMM_FILTERL,Channel,RegisterData);
	/// write to mode register current settings
	RegisterData = Calibration | Gain | BurnoutOnOff | FilterOnOff;
	AdcCalibration(chip,Channel,RegisterData);
	return;
}
///-----------------------------------------------------------------------------
uint32_t AdcCalculate(uint32_t data)
{
	uint32_t Ret;
	Ret  = data;
	Ret *= ADC_DATA_LSB_NANO_VOLT;
	Ret /= 1000;
	return(Ret);
}
///-----------------------------------------------------------------------------
bool AdcBinToASCII(uint32_t bin_data,char* ascii_buffer)
{
	uint8_t  digit;
	uint8_t  cnt;
	uint32_t compare;
	if(ascii_buffer == NULL)
	{
		return(false);
	}
	for(cnt = 0;cnt < 10;cnt++)
	{
		switch(cnt)
		{
			case 0 : compare = 1000000000; break;
			case 1 : compare = 100000000;  break;
			case 2 : compare = 10000000;   break;
			case 3 : compare = 1000000;    break;
			case 4 : compare = 100000;     break;
			case 5 : compare = 10000;      break;
			case 6 : compare = 1000;       break;
			case 7 : compare = 100;        break;
			case 8 : compare = 10;         break;
			case 9 : compare = 1;          break;
		}
		digit = 0;
		while(bin_data >= compare)
		{
			bin_data -= compare;
			digit++;
		}
		digit += 0x30;
		*ascii_buffer = digit;
		ascii_buffer++;		
	}
	return(true);
}
///-----------------------------------------------------------------------------
