/*
 * system_data_structures.h
 *
 *  Created on: 18 May 2026
 *      Author: mehta
 */

#ifndef INC_CONFIG_VEHICLE_DATA_STRUCTS_H_
#define INC_CONFIG_VEHICLE_DATA_STRUCTS_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct QueueErrors
{

	uint16_t state_transition_errors;
	uint16_t can_msg_errors;
	uint16_t pedals_adc_errors;

} QueueErrors_t;

typedef struct VehicleState
{

	float BMS_voltage;
	float accy_voltage;
	float inverter_voltage;
	bool precharge_signal_sent;
	bool precharge_voltage_met;
	bool air_aux_closed;
	bool precharge_complete;
	bool button_toggled;
	bool inverter_enabled;

} VehicleState_t;

typedef struct PedalsState
{

	uint8_t accel_pedal_percentage;
	uint8_t brake_pedal_percentage;

} PedalsState_t;


typedef struct TestingVCU
{

	uint16_t APPS1;
	uint16_t APPS2;
	uint16_t BPS1;
	uint16_t BPS2;
	uint16_t BSPD_CS;
	uint16_t BSE;
	uint16_t SAS;
	uint16_t SAI;

	bool RTD_button_status;
	bool AIR_digital;
	bool PC_complete;

	bool BSE_Fault;
	bool plausibility_fault;
	bool CS_fault;

} TestingVCU_t;


void initialise_queue_errors(QueueErrors_t* p_queue_errors);
extern QueueErrors_t* p_queue_errors_data;

void initialise_vehicle_state_data(VehicleState_t* p_vehicle_state);
extern VehicleState_t* p_vehicle_state_data;

void initialise_pedals_state(PedalsState_t* p_pedals_state);
extern PedalsState_t* p_pedals_state;

void initialise_testing_VCU_data(TestingVCU_t* testing_VCU_data);
extern TestingVCU_t* p_testing_vcu_data;

#endif /* INC_CONFIG_VEHICLE_DATA_STRUCTS_H_ */
