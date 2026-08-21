#include "iic/24cxx.h"

extern I2C_HandleTypeDef hi2c1;  /* HAL生成的I2C句柄 */

/**
 * @brief       初始化I2C接口（HAL方式）
 * @param       无
 * @retval      无
 */
void at24cxx_hal_init(void)
{
    /* 不需要做任何事！因为HAL库已经在MX_I2C1_Init()里完成了所有配置 */
    /* 这里保留空函数，保持接口统一 */
}

/**
 * @brief       在AT24CXX指定地址读出一个数据
 * @param       addr: 开始读数的地址
 * @retval      读到的数据
 */
uint8_t at24cxx_read_one_byte(uint16_t addr)
{
    uint8_t temp = 0;
    uint8_t dev_addr = 0xA0;
    uint8_t addr_high, addr_low;
    HAL_StatusTypeDef status;
    
    /* 根据24CXX型号处理地址 */
    if (EE_TYPE > AT24C16)  /* 24C32及以上，需要2字节地址 */
    {
        addr_high = (addr >> 8) & 0xFF;
        addr_low = addr & 0xFF;
        
        /* 方式1：先写地址，再读数据（组合操作） */
        uint8_t tx_buffer[2] = {addr_high, addr_low};
        
        /* 发送设备地址 + 写位，然后发送2字节地址 */
        status = HAL_I2C_Master_Transmit(&hi2c1, dev_addr >> 1, tx_buffer, 2, 100);
        if (status != HAL_OK) return 0xFF;
        
        /* 重新开始，转为读模式 */
        status = HAL_I2C_Master_Receive(&hi2c1, dev_addr >> 1, &temp, 1, 100);
        if (status != HAL_OK) return 0xFF;
    }
    else  /* 24C16及以下，地址在器件地址中体现 */
    {
        /* 24C02的地址处理：7位器件地址 + 8位存储地址 */
        /* 器件地址是0xA0，存储地址直接是addr的低8位 */
        addr_low = addr & 0xFF;
        
        /* 方式1：使用HAL_I2C_Mem_Read（专门用于寄存器读） */
        /* 注意：HAL_I2C_Mem_Read的参数是7位地址（右移一位） */
        status = HAL_I2C_Mem_Read(&hi2c1, 
                                  dev_addr >> 1,  /* 7位设备地址：0x50 */
                                  addr_low,       /* 存储地址（寄存器地址） */
                                  I2C_MEMADD_SIZE_8BIT,  /* 8位地址 */
                                  &temp,          /* 接收缓冲区 */
                                  1,              /* 读取1字节 */
                                  100);           /* 超时时间 */
        if (status != HAL_OK) return 0xFF;
    }
    
    return temp;
}

/**
 * @brief       在AT24CXX指定地址写入一个数据
 * @param       addr: 写入数据的目的地址
 * @param       data: 要写入的数据
 * @retval      无
 */
void at24cxx_write_one_byte(uint16_t addr, uint8_t data)
{
    uint8_t dev_addr = 0xA0;
    uint8_t addr_high, addr_low;
    HAL_StatusTypeDef status;
    
    if (EE_TYPE > AT24C16)  /* 24C32及以上 */
    {
        uint8_t tx_buffer[3];
        addr_high = (addr >> 8) & 0xFF;
        addr_low = addr & 0xFF;
        
        tx_buffer[0] = addr_high;
        tx_buffer[1] = addr_low;
        tx_buffer[2] = data;
        
        status = HAL_I2C_Master_Transmit(&hi2c1, dev_addr >> 1, tx_buffer, 3, 100);
        if (status != HAL_OK) return;
    }
    else  /* 24C16及以下 */
    {
        addr_low = addr & 0xFF;
        
        /* 使用HAL_I2C_Mem_Write（专门用于寄存器写） */
        status = HAL_I2C_Mem_Write(&hi2c1,
                                   dev_addr >> 1,  /* 7位设备地址 */
                                   addr_low,       /* 存储地址 */
                                   I2C_MEMADD_SIZE_8BIT, /* 8位地址 */
                                   &data,          /* 数据缓冲区 */
                                   1,              /* 写入1字节 */
                                   100);
        if (status != HAL_OK) return;
    }
    
    /* EEPROM写入需要时间，必须等待 */
    HAL_Delay(10);
}

/**
 * @brief       在AT24CXX指定地址读出指定个数的数据
 * @param       addr    : 开始读出的地址
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要读出数据的个数
 * @retval      无
 */
void at24cxx_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    if (EE_TYPE > AT24C16)  /* 24C32及以上 */
    {
        uint8_t addr_high = (addr >> 8) & 0xFF;
        uint8_t addr_low = addr & 0xFF;
        uint8_t tx_buffer[2] = {addr_high, addr_low};
        
        /* 先发送地址 */
        HAL_I2C_Master_Transmit(&hi2c1, 0xA0 >> 1, tx_buffer, 2, 100);
        /* 再读取数据 */
        HAL_I2C_Master_Receive(&hi2c1, 0xA0 >> 1, pbuf, datalen, 100);
    }
    else  /* 24C16及以下 */
    {
        /* 使用HAL_I2C_Mem_Read连续读取 */
        HAL_I2C_Mem_Read(&hi2c1,
                         0xA0 >> 1,
                         addr & 0xFF,
                         I2C_MEMADD_SIZE_8BIT,
                         pbuf,
                         datalen,
                         100);
    }
}

/**
 * @brief       在AT24CXX指定地址写入指定个数的数据
 * @param       addr    : 开始写入的地址
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要写入数据的个数
 * @retval      无
 */
void at24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    uint16_t i;
    
    /* 注意：24C02每页8字节，跨页写入时需要处理页边界 */
    /* 简单起见，这里逐个字节写入（慢但可靠） */
    for (i = 0; i < datalen; i++)
    {
        at24cxx_write_one_byte(addr + i, pbuf[i]);
    }
}

/**
 * @brief       检查AT24CXX是否正常
 * @param       无
 * @retval      0: 检测成功, 1: 检测失败
 */
uint8_t at24cxx_check(void)
{
    uint8_t temp;
    uint16_t addr = EE_TYPE;  /* 测试地址 */
    
    /* 先读取测试地址的值 */
    temp = at24cxx_read_one_byte(addr);
    if (temp == 0x55)
    {
        return 0;  /* 数据正确，说明之前已经初始化过 */
    }
    else
    {
        /* 写入测试数据 */
        at24cxx_write_one_byte(addr, 0x55);
        /* 再读取验证 */
        temp = at24cxx_read_one_byte(addr);
        
        if (temp == 0x55)
        {
            return 0;  /* 测试成功 */
        }
    }
    
    return 1;  /* 测试失败 */
}