#ifndef ESP32_COMM_H
#define ESP32_COMM_H

#include "main.h"
#include <stdbool.h>

// Buffer sizes
#define ESP32_RX_BUFFER_SIZE 128
#define ESP32_TX_BUFFER_SIZE 256

// Function prototypes
void ESP32_Init(UART_HandleTypeDef *huart);
void ESP32_SendStatus(float temp, float setpoint, float fan_percent, const char *status);
bool ESP32_CheckCommand(float *new_setpoint);
void ESP32_SendAck(void);

#endif
