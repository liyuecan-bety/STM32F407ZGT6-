#include "exti\bsp_exti.h"
#include "key\bsp_key.h"
#include "led\bsp_led.h"
void EXTI_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PE2 PE3 PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

//中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    HAL_Delay(20);      /* 消抖 */
    switch(GPIO_Pin)
    {
        case GPIO_PIN_4:
            if (KEY0 == 0)
            {
                LED_F9_Toggle;  /* LED0 状态取反 */ 
            }
            break;

        case GPIO_PIN_3:
            if (KEY1 == 0)
            {
                LED_F10_Toggle;  /* LED1 状态取反 */ 
            }
            break;

        case GPIO_PIN_2:
            if (KEY2 == 0)
            {
                LED_F9_Toggle;  /* LED1 状态取反 */
                LED_F10_Toggle; /* LED0 状态取反 */ 
            }
            break;

//        case GPIO_PIN_0:
//            if (WK_UP == 1)
//            {
//                BEEP_TOGGLE();  /* 蜂鸣器状态取反 */ 
//            }
//            break;

        default : break;
    }
}