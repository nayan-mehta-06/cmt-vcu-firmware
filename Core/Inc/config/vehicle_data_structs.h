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
#include "cmsis_os2.h"


/*
 * Struct to hold the number of errors from putting items into queues
 * Used for debugging
 * */
typedef struct QueueErrors
{

	uint16_t state_transition_errors;
	uint16_t can_msg_errors;
	uint16_t pedals_adc_errors;
	uint16_t calibration_data_errors;
	uint16_t logging_adc_errors;

} QueueErrors_t;


/*
 * Struct to hold information about the car's current state
 * Primarily used for RTD sequence
 * */
typedef struct VehicleState
{

	float BMS_voltage;
	float accy_voltage;
	float inverter_voltage;
	bool calibrating;
	bool calibration_complete;
	bool precharge_signal_sent;
	bool precharge_voltage_met;
	bool air_aux_closed;
	bool precharge_complete;
	bool button_toggled;
	bool inverter_enabled;
	bool brakes_engaged;
	bool bse_fault;
	bool cs_fault;
	bool plausibility_fault;

} VehicleState_t;

/*
 * Struct to hold the current positions and %'s of the pedals
 * */
typedef struct PedalsState
{

	uint8_t apps_1_percentage;
	uint8_t apps_2_percentage;
	uint8_t accel_pedal_percentage;
	uint8_t brake_pedal_percentage;

} PedalsState_t;

/*
 * Struct to hold the calibration data sent from the calibration task
 * to the process pedals data task
 * */
typedef struct CalibrationData
{

	uint16_t apps_1_min_value;
	uint16_t apps_2_min_value;
	uint16_t bse_min_value;

	uint16_t apps_1_max_value;
	uint16_t apps_2_max_value;
	uint16_t bse_max_value;

} CalibrationData_t;

/*
 * Struct to show the status of a pedal for debugging
 * */
typedef enum
{
    NORMAL			= 0,
	SHORT_CIRCUIT	= 1,
	OPEN_CIRCUIT	= 2,
} PedalStatus_t;

/*
 * Struct to hold all pedal fault statuses
 * */
typedef struct PedalsFaults
{

	PedalStatus_t apps_1_status;
	PedalStatus_t apps_2_status;
	PedalStatus_t bse_status;
	bool apps_implausibility;
	bool screenshot;

} PedalsFaults_t;

/*
 * Struct to hold the data sent to VCU by E-interface
 * */
typedef struct EintData
{

	uint8_t power_limit;
	bool launch_control;

} EintData_t;

/*
 * Struct for testing all inputs/outputs of VCU
 * */
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

/*
 * Struct to determine the utilisation of stack size allocation
 * of each task in CubeMX for optimisaton
 * */
typedef struct FreertosTaskStackUsage {

	uint32_t unused_words_LogTaskStackUsage;
	uint32_t unused_words_CalibrateTask;
	uint32_t unused_words_RTD_Task;
	uint32_t unused_words_ProcessADC_Task;
	uint32_t unused_words_PedalsDataTask;
	uint32_t unused_words_VehicleCtrlTask;
	uint32_t unused_words_StateCtrlTask;
	uint32_t unused_words_ProcessCAN_Task;
	uint32_t unused_words_HandleGUI_Task;
	uint32_t unused_words_LogVehicleData_Task;
	uint32_t unused_words_VehicleFaultHandler_Task;

} FreertosTaskStackUsage_t;


void initialise_queue_errors(QueueErrors_t* p_queue_errors);
extern QueueErrors_t* p_queue_errors_data;

void initialise_vehicle_state_data(VehicleState_t* p_vehicle_state);
extern VehicleState_t* p_vehicle_state_data;

void initialise_pedals_state(PedalsState_t* p_pedals_state);
extern PedalsState_t* p_pedals_state;

void initialise_testing_VCU_data(TestingVCU_t* testing_VCU_data);
extern TestingVCU_t* p_testing_vcu_data;

void initialise_pedals_faults(PedalsFaults_t* p_pedals_faults);
extern PedalsFaults_t* p_pedals_faults;

void initialise_eint_data(EintData_t* p_eint_data);
extern EintData_t* p_eint_data;

void initialise_freertos_task_stack_usage(FreertosTaskStackUsage_t* p_freertos_task_stack_usage);
extern FreertosTaskStackUsage_t* p_freertos_task_stack_usage;

extern PedalsState_t* p_pedals_state_data;

#endif /* INC_CONFIG_VEHICLE_DATA_STRUCTS_H_ */
