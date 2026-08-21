#ifndef __24CXX_HAL_H
#define __24CXX_HAL_H

#include "main.h"      // 包含HAL库头文件
#include "iic/iic.h"       // 包含HAL生成的I2C配置

/* 24CXX型号定义（和原版完全一致） */
#define AT24C01     127
#define AT24C02     255
#define AT24C04     511
#define AT24C08     1023
#define AT24C16     2047
#define AT24C32     4095
#define AT24C64     8191
#define AT24C128    16383
#define AT24C256    32767

/* 根据实际芯片选择，正点原子探索者用的是24C02 */
#define EE_TYPE     AT24C02

/* 函数声明（和原版完全一致，方便替换） */
void at24cxx_init(void);
uint8_t at24cxx_check(void);
uint8_t at24cxx_read_one_byte(uint16_t addr);
void at24cxx_write_one_byte(uint16_t addr, uint8_t data);
void at24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen);
void at24cxx_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen);

#endif


