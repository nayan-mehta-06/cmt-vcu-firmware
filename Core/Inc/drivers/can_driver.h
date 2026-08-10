/*
 * can_driver.h
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#ifndef INC_DRIVERS_CAN_DRIVER_H_
#define INC_DRIVERS_CAN_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/vehicle_data_structs.h"

extern QueueErrors_t* p_queue_errors_data;

bool CAN_transmit(uint8_t* data, int dlc, int id, int can_line, int CAN_ID_format);
void CAN_1_Error_Handler();
void CAN_2_Error_Handler();

#define CAN_1 1
#define CAN_2 2

#define CAN_STD_ID_FORMAT 0
#define CAN_EXT_ID_FORMAT 1

#define ONE_BYTE 1
#define TWO_BYTES 2
#define THREE_BYTES 3
#define FOUR_BYTES 4
#define FIVE_BYTES 5
#define SIX_BYTES 6
#define SEVEN_BYTES 7
#define EIGHT_BYTES 8

typedef struct
{

	uint32_t StdId;
	uint32_t ExtId;
	uint32_t DLC;
	uint8_t data[8];
	uint8_t can_line;

} CanMsg_t;

#endif /* INC_DRIVERS_CAN_DRIVER_H_ */
