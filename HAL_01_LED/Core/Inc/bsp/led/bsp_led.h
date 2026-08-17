#ifndef __BSP_LED_H__
#define __BSP_LED_H__


#include "main.h"
#define LED_F9_ON					do{HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);}while(0)
#define LED_F9_OFF					do{HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);}while(0)
#define LED_F9_Toggle			do{HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);}while(0)

#define LED_F10_ON					do{HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET);}while(0)
#define LED_F10_OFF					do{HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);}while(0)
#define LED_F10_Toggle			do{HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);}while(0)
void LED_GPIO_Init(void);



#endif
