#ifndef __IIC_H__
#define __IIC_H__

#include "main.h"

#define I2C_ADDRESS 0x68 // 7ŒªI2Cµÿ÷∑£¨”“∂‘∆Î
extern I2C_HandleTypeDef hi2c1;
void I2C1_Init(void);
HAL_StatusTypeDef I2C_Write(uint8_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef I2C_Read(uint8_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef I2C_ReadRegister(uint8_t DevAddress, uint8_t Reg, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef I2C_WriteRegister(uint8_t DevAddress, uint8_t Reg, uint8_t *pData, uint16_t Size);

#endif


