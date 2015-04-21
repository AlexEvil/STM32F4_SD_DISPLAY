///-----------------------------------------------------------------------------
#ifndef AD7714_H
#define AD7714_H
///-----------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
///-----------------------------------------------------------------------------
#include "stm32f4xx_adc_io.h"
#include <stddef.h>
#include <stdbool.h>
///-----------------------------------------------------------------------------
/// COMMUNICATION REGISTER -----------------------------------------------------
///-----------------------------------------------------------------------------
typedef enum
{
	ADC_COMM_COMM        = (0<<4), /// Communications Register 8 Bits
	ADC_COMM_MODE        = (1<<4), /// Mode Register 8 Bits
	ADC_COMM_FILTERH     = (2<<4), /// Filter High Register 8 Bits
	ADC_COMM_FILTERL     = (3<<4), /// Filter Low Register 8 Bits
	ADC_COMM_TEST        = (4<<4), /// Test Register 8 Bits
	ADC_COMM_DATA        = (5<<4), /// Data Register 16 Bits or 24 Bits
	ADC_COMM_ZERO_SCALE  = (6<<4), /// Zero-Scale Calibration Register 24 Bits
	ADC_COMM_FULL_SCALE  = (7<<4), /// Full-Scale Calibration Register 24 Bits
	ADC_COMM_READ        = (1<<3), /// next operation to be read
	ADC_COMM_WRITE       = (0<<3), /// next operation to be write
	ADC_COMM_CHANNEL_1_6 = (0<<0), /// 0 0 0 AIN1 AIN6 Pseudo
	ADC_COMM_CHANNEL_2_6 = (1<<0), /// 0 0 1 AIN2 AIN6 Pseudo
	ADC_COMM_CHANNEL_3_6 = (2<<0), /// 0 1 0 AIN3 AIN6 Pseudo
	ADC_COMM_CHANNEL_4_6 = (3<<0), /// 0 1 1 AIN4 AIN6 Pseudo
	ADC_COMM_CHANNEL_1_2 = (4<<0), /// 1 0 0 AIN1 AIN2 Fully
	ADC_COMM_CHANNEL_3_4 = (5<<0), /// 1 0 1 AIN3 AIN4 Fully
	ADC_COMM_CHANNEL_5_6 = (6<<0), /// 1 1 0 AIN5 AIN6 Fully
	ADC_COMM_CHANNEL_6_6 = (7<<0)  /// 1 1 1 AIN6 AIN6 Test Mode
}ADC_COMM_REG;
///-----------------------------------------------------------------------------
/// MODE REGISTER --------------------------------------------------------------
///-----------------------------------------------------------------------------
typedef enum
{
	ADC_MODE_NORMAL      = (0<<5), /// Normal Mode
	ADC_MODE_SELF        = (1<<5), /// Self-Calibration
	ADC_MODE_ZERO        = (2<<5), /// Zero-Scale System Calibration
	ADC_MODE_FULL        = (3<<5), /// Full-Scale System Calibration
	ADC_MODE_SYSTEM      = (4<<5), /// System-Offset Calibration;
	ADC_MODE_GROUND      = (5<<5), /// Background Calibration;
	ADC_MODE_ZERO_SELF   = (6<<5), /// Zero-Scale Self-Calibration;
	ADC_MODE_FULL_SELF   = (7<<5), /// Full-Scale Self-Calibration;
	ADC_MODE_GAIN_1      = (0<<2), /// GAIN SET 1
	ADC_MODE_GAIN_2      = (1<<2), /// GAIN SET 2
	ADC_MODE_GAIN_4      = (2<<2), /// GAIN SET 4
	ADC_MODE_GAIN_8      = (3<<2), /// GAIN SET 8
	ADC_MODE_GAIN_16     = (4<<2), /// GAIN SET 16
	ADC_MODE_GAIN_32     = (5<<2), /// GAIN SET 32
	ADC_MODE_GAIN_64     = (6<<2), /// GAIN SET 64
	ADC_MODE_GAIN_128    = (7<<2), /// GAIN SET 128
	ADC_MODE_BURNOUT_ON  = (1<<1), /// On burnout current
	ADC_MODE_BURNOUT_OFF = (0<<1), /// Off burnout current
	ADC_MODE_FILTER_ON   = (1<<0), /// On digital filter
	ADC_MODE_FILTER_OFF	 = (0<<0)  /// Off digital filter
}ADC_MODE_REG;
///-----------------------------------------------------------------------------
/// FILTERS REGISTER -----------------------------------------------------------
///-----------------------------------------------------------------------------
typedef enum
{
	ADC_FILTERH_BIPOLAR       = (0<<7), /// bipolar type operation
	ADC_FILTERH_UNIPOLAR      = (1<<7), /// unipolar type operation
	ADC_FILTERH_LENGTH_16_BIT = (0<<6), /// Data word length is 16 bit
	ADC_FILTERH_LENGTH_24_BIT = (1<<6), /// Data word length is 24 bit
	ADC_FILTERH_BST_ON        = (1<<5), /// on Current Boost see datasheet
	ADC_FILTERH_BST_OFF       = (0<<5)  /// off Current Boost see datasheet
}ADC_FILTERH_REG;
///-----------------------------------------------------------------------------
typedef struct
{
	uint8_t Calibration;  /// Calibration Type
	uint8_t Gain;         /// Gain Amplifaer
	uint8_t BurnoutOnOff; /// Burnout current On /Off
	uint8_t FilterOnOff;  /// Digital filter On/Off
	uint8_t Channel;      /// Select Channel
	uint8_t Polar;        /// Type operation Bipolar/Unipolar
	uint8_t WordLength;   /// Data word length 16/24 bit
	uint8_t Boost;        /// Current Boost On/Off
	float   FilterHz;     /// Digital filter response
}AdcSettings;
///-----------------------------------------------------------------------------
extern void     AdcInit(void);
extern void     AdcReset(ADC_IO_CHIP chip);
extern uint32_t AdcCalculate(uint32_t data);
extern bool     AdcBinToASCII(uint32_t bin_data,char* ascii_buffer);
extern void     AdcWriteSettings(ADC_IO_CHIP chip,AdcSettings* Settings);
extern void     AdcCalibration(ADC_IO_CHIP chip,uint8_t channel,uint8_t data);
extern uint32_t AdcReadData(ADC_IO_CHIP chip,uint8_t channel,const uint8_t length);
///-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
///-----------------------------------------------------------------------------
#endif /// #ifndef AD7714_H
///-----------------------------------------------------------------------------
