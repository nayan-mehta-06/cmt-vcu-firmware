/*
 * usart_driver.h
 *
 *  Created on: 29 Jul 2026
 *      Author: mehta
 */

#ifndef INC_DRIVERS_USART_DRIVER_H_
#define INC_DRIVERS_USART_DRIVER_H_

#include <stdint.h>

void UART_Transmit(const uint8_t *data, uint16_t size, uint32_t timeout);

void uart_print(char *str);
void uart_print_hex(uint8_t val);
void uart_print_dec(uint16_t val);
void uart_print_uint(uint32_t val);

#endif /* INC_DRIVERS_USART_DRIVER_H_ */
