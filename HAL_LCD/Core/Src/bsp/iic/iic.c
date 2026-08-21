#include "iic/iic.h"


I2C_HandleTypeDef hi2c1;
#define I2C_ADDRESS 0x68 // 7位I2C地址，右对齐
/* I2C1 init function */
void I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// 写数据到I2C设备
HAL_StatusTypeDef I2C_Write(uint8_t DevAddress, uint8_t *pData, uint16_t Size) {
   
    return HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
}

// 从I2C设备读取数据
HAL_StatusTypeDef I2C_Read(uint8_t DevAddress, uint8_t *pData, uint16_t Size) {
   
    return HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
}

// 从I2C设备的特定寄存器读取数据
HAL_StatusTypeDef I2C_ReadRegister(uint8_t DevAddress, uint8_t Reg, uint8_t *pData, uint16_t Size) {
   
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, &Reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
   
        return status;
    }
    return HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
}

// 写数据到I2C设备的特定寄存器
HAL_StatusTypeDef I2C_WriteRegister(uint8_t DevAddress, uint8_t Reg, uint8_t *pData, uint16_t Size) {
   
    uint8_t buffer[Size + 1];
    buffer[0] = Reg;
    memcpy(&buffer[1], pData, Size);
    return HAL_I2C_Master_Transmit(&hi2c1, DevAddress, buffer, Size + 1, HAL_MAX_DELAY);
}

/* USER CODE END 1 */

