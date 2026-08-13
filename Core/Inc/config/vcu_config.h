/*
 * vcu_config.h
 *
 *  Created on: 17 May 2026
 *      Author: mehta
 */

#ifndef INC_CONFIG_VCU_CONFIG_H_
#define INC_CONFIG_VCU_CONFIG_H_

#include "stm32f4xx_hal.h"

// Inverter data
#define TORQUE_LIM_POS 1000
#define TORQUE_LIM_NEG -100
#define ID110 107.2f

// CAN transmit speeds
// Inverter must have messages sent at a minimum of 20Hz
#define INVERTER_TORQUE_COMMAND_CAN_TRANSMIT_RATE pdMS_TO_TICKS(20) // 50Hz
#define CAN_2_TRANSMIT_RATE pdMS_TO_TICKS(40) // 25Hz

// Software timer durations
#define RTD_BUTTON_PRESS_MILLISECONDS pdMS_TO_TICKS(200)
#define RTD_BUZZER_TIME_MILLISECONDS pdMS_TO_TICKS(3000)
#define CALIBRATION_ENABLE_PERIOD_MILLISECONDS pdMS_TO_TICKS(3000)
#define PEDALS_FAULT_TIMEOUT_MILLISECONDS pdMS_TO_TICKS(100)

#define BRAKES_ENGAGED_THRESHOLD_PERCENTAGE 5

#endif /* INC_CONFIG_VCU_CONFIG_H_ */
