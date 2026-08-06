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

#endif /* INC_DRIVERS_USART_DRIVER_H_ */
