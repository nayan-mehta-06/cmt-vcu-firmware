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

// APPS1 - 90 degree sensor - voltage divided
#define APPS_1_OPEN_CIRCUIT_FAULT_THRESHOLD 4000
#define APPS_1_SHORT_CIRCUIT_FAULT_THRESHOLD 1200

// APPS2 - 45 degree sensor
#define APPS_2_OPEN_CIRCUIT_FAULT_THRESHOLD 4000
#define APPS_2_SHORT_CIRCUIT_FAULT_THRESHOLD 1200

#define BSE_OPEN_CIRCUIT_FAULT_THRESHOLD 4000
#define BSE_SHORT_CIRCUIT_FAULT_THRESHOLD 100

extern PedalsState_t* p_pedals_state_data;
extern PedalsFaults_t* p_pedals_faults_data;

void Task_Process_Pedals_Data(void *argument);
void APPS_1_OutOfRange_Timer_Callback(void *argument);
void APPS_2_OutOfRange_Timer_Callback(void *argument);
void BSE_OutOfRange_Timer_Callback(void *argument);
void APPS_Implausibility_Timer_Callback(void *argument);

uint8_t calc_adc_percentage(uint16_t val, uint16_t min, uint16_t max);
PedalStatus_t pedal_position_sensor_out_of_range_fault_check(uint16_t pedal_position_sensor_raw_value, uint16_t pedal_position_sensor_open_circuit_threshold, uint16_t pedal_position_sensor_short_circuit_threshold);
void Get_Previous_Pedals_Faults_Data(PedalsFaults_t* p_previous_faults);
void Set_New_Pedals_Faults_Data(PedalsFaults_t* p_new_faults);

#endif /* INC_APP_PROCESS_PEDALS_DATA_H_ */
