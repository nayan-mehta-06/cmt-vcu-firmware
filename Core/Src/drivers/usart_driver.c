/*
 * usart_driver.h
 *
 *  Created on: 29 Jul 2026
 *      Author: mehta
 */

#include <string.h>
#include <stdio.h>

#include "drivers/usart_driver.h"
#include "usart.h"

void UART_Transmit(const uint8_t *data, uint16_t size, uint32_t timeout)
{
	HAL_UART_Transmit(&huart1, data, size, timeout);
}

void UART_Print(char *str)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

void UART_Print_Hex(uint8_t val)
{
    char buf[5];
    snprintf(buf, sizeof(buf), "%02X ", val);
    uart_print(buf);
}

void UART_Print_Dec(uint16_t val)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%d ", val);
    uart_print(buf);
}

void UART_Print_Uint(uint32_t val)
{
    char buf[12]; // Enough for 32-bit uint + null
    snprintf(buf, sizeof(buf), "%lu", val);
    uart_print(buf);
}

