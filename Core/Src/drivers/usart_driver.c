/*
 * usart_driver.h
 *
 *  Created on: 29 Jul 2026
 *      Author: mehta
 */


#include "drivers/usart_driver.h"
#include "usart.h"

void UART_Transmit(const uint8_t *data, uint16_t size, uint32_t timeout)
{
	HAL_UART_Transmit(&huart1, data, size, timeout);
}

