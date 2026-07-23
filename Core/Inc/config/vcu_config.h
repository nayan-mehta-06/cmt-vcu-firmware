/*
 * vcu_config.h
 *
 *  Created on: 17 May 2026
 *      Author: mehta
 */

#ifndef INC_CONFIG_VCU_CONFIG_H_
#define INC_CONFIG_VCU_CONFIG_H_

#include "stm32f4xx_hal.h"

#define MIN_ADC_VAL 30
#define MAX_ADC_VAL 3070

#define TORQUE_LIM_POS 1000
#define TORQUE_LIM_NEG -100

#define INVERTER_TORQUE_COMMAND_CAN_TRANSMIT_RATE pdMS_TO_TICKS(20) // 50Hz


#endif /* INC_CONFIG_VCU_CONFIG_H_ */
