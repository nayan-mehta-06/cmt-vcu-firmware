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

#define USE_INTEGRATION_THRESHOLDS // Prevents faulting on integration board

#ifdef USE_INTEGRATION_THRESHOLDS

// APPS1 - 90 degree sensor
#define APPS_1_OPEN_CIRCUIT_FAULT_THRESHOLD  3200
#define APPS_1_SHORT_CIRCUIT_FAULT_THRESHOLD 15

// APPS2 - 45 degree sensor
#define APPS_2_OPEN_CIRCUIT_FAULT_THRESHOLD  4150
#define APPS_2_SHORT_CIRCUIT_FAULT_THRESHOLD 15

#define BSE_OPEN_CIRCUIT_FAULT_THRESHOLD     4150
#define BSE_SHORT_CIRCUIT_FAULT_THRESHOLD    15

#else

// APPS1 - 90 degree sensor
#define APPS_1_OPEN_CIRCUIT_FAULT_THRESHOLD  3000
#define APPS_1_SHORT_CIRCUIT_FAULT_THRESHOLD 500

// APPS2 - 45 degree sensor
#define APPS_2_OPEN_CIRCUIT_FAULT_THRESHOLD  4000
#define APPS_2_SHORT_CIRCUIT_FAULT_THRESHOLD 1200

#define BSE_OPEN_CIRCUIT_FAULT_THRESHOLD     4000
#define BSE_SHORT_CIRCUIT_FAULT_THRESHOLD    90

#endif


// Event bits defined for all pedal-related flags
#define APPS_1_OUT_OF_RANGE (1U << 0)
#define APPS_2_OUT_OF_RANGE (1U << 1)
#define BSE_OUT_OF_RANGE (1U << 2)
#define APPS_IMPLAUSIBILITY (1U << 0)
#define SCREENSHOT (1U << 0)

extern PedalsState_t* p_pedals_state_data;
extern PedalsFaults_t* p_pedals_faults_data;

void Task_Process_Pedals_Data(void *argument);
void APPS_1_OutOfRange_Timer_Callback(void *argument);
void APPS_2_OutOfRange_Timer_Callback(void *argument);
void BSE_OutOfRange_Timer_Callback(void *argument);
void APPS_Implausibility_Timer_Callback(void *argument);

uint8_t calc_adc_percentage(uint16_t val, uint16_t min, uint16_t max);
PedalStatus_t pedal_position_sensor_out_of_range_fault_check(uint16_t pedal_position_sensor_raw_value,
															uint16_t pedal_position_sensor_open_circuit_threshold,
															uint16_t pedal_position_sensor_short_circuit_threshold);
bool APPS_Implausibility_Check(uint8_t APPS_1_percentage, uint8_t APPS_2_percentage);
bool Screenshot_Check(uint8_t APPS_1_percentage, uint8_t APPS_2_percentage,
					  uint8_t brake_pedal_position_percentage,
					  uint8_t brakes_engaged_threshold_percentage,
					  bool brake_pedal_implausibility_fault);

void Get_Previous_Pedals_Faults_Data(PedalsFaults_t* p_previous_faults);
void Set_New_Pedals_Faults_Data(PedalsFaults_t* p_new_faults);

#endif /* INC_APP_PROCESS_PEDALS_DATA_H_ */
