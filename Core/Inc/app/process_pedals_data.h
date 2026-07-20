/*
 * process_pedals_data.h
 *
 *  Created on: 17 Jul 2026
 *      Author: mehta
 */

#ifndef INC_APP_PROCESS_PEDALS_DATA_H_
#define INC_APP_PROCESS_PEDALS_DATA_H_

#include <stdint.h>

#include "config/vehicle_data_structs.h"

extern PedalsState_t* p_pedals_state_data;

uint8_t calc_adc_percentage(uint16_t val);

#endif /* INC_APP_PROCESS_PEDALS_DATA_H_ */
