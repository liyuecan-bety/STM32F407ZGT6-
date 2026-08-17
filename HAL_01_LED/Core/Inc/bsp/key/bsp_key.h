#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__


#include "main.h"

#define KEY_ON 0
#define KEY_OFF 1

#define KEY0        HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)     /* 读取KEY0引脚 */
#define KEY1        HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)          /* 读取KEY1引脚 */
#define KEY2        HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)          /* 读取KEY2引脚 */
#define KEY4       	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)          /* 读取WKUP引脚 */

void KEY_GPIO_Init(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif
