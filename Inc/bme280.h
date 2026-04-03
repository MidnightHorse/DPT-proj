#ifndef BME280_H_
#define BME280_H_

#include "stm32f4xx_hal.h"

#define BME280_ADDR (0x76 << 1)

void BME280_Init(I2C_HandleTypeDef *hi2c);
float BME280_ReadTemperature(void);
float BME280_ReadPressure(void);
float BME280_ReadHumidity(void);
void BME280_PrintCalibration(UART_HandleTypeDef *huart);

#endif
