///-----------------------------------------------------------------------------
#include "led.h"
///-----------------------------------------------------------------------------
void LedInit(void)
{
	GPIO_InitTypeDef PinDef;
	__GPIOD_CLK_ENABLE();
	PinDef.Pin   = LED_PORT;
	PinDef.Mode  = GPIO_MODE_OUTPUT_PP;
	PinDef.Pull  = GPIO_PULLDOWN;
	PinDef.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD,&PinDef);
}
///-----------------------------------------------------------------------------
void LedDeInit(void)
{
	uint32_t PinDef;
	PinDef = LED_PORT;
	HAL_GPIO_DeInit(GPIOD,PinDef);
}
///-----------------------------------------------------------------------------
void LedOn(int num)
{
	switch(num)
	{
		case LED_GREEN  : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); break;
		case LED_ORANGE : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET); break;
		case LED_RED    : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET); break;
		case LED_BLUE   : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET); break;
	}
}
///-----------------------------------------------------------------------------
void LedOff(int num)
{
	switch(num)
	{
		case LED_GREEN  : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET); break;
		case LED_ORANGE : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET); break;
		case LED_RED    : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET); break;
		case LED_BLUE   : HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET); break;
	}
}
///-----------------------------------------------------------------------------
