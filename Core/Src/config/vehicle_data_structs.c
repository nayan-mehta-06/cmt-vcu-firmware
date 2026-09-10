/*
 * system_data_structures.c
 *
 *  Created on: 18 May 2026
 *      Author: mehta
 */

#include <config/vehicle_data_structs.h>
#include "app/process_pedals_data.h"
#include <stdbool.h>
#include <stdint.h>

void initialise_vehicle_state_data(VehicleState_t* p_vehicle_state)
{
	p_vehicle_state->BMS_voltage = 0;
	p_vehicle_state->accy_voltage = 0;
	p_vehicle_state->inverter_voltage = 0;
	p_vehicle_state->calibrating = false;
	p_vehicle_state->calibration_complete = false;
	p_vehicle_state->precharge_signal_sent = false;
	p_vehicle_state->precharge_voltage_met = false;
	p_vehicle_state->air_aux_closed = false;
	p_vehicle_state->precharge_complete = false;
	p_vehicle_state->button_toggled = false;
	p_vehicle_state->inverter_enabled = false;
	p_vehicle_state->brakes_engaged = false;
	p_vehicle_state->bse_fault = false;
	p_vehicle_state->cs_fault = false;
	p_vehicle_state->plausibility_fault = false;
}

void initialise_queue_errors(QueueErrors_t* p_queue_errors)
{
	p_queue_errors->state_transition_errors = 0;
	p_queue_errors->can_msg_errors = 0;
	p_queue_errors->pedals_adc_errors = 0;
	p_queue_errors->calibration_data_errors = 0;
	p_queue_errors->logging_adc_errors = 0;
}

void initialise_pedals_state(PedalsState_t* p_pedals_state)
{
	p_pedals_state->apps_1_percentage = 0;
	p_pedals_state->apps_2_percentage = 0;
	p_pedals_state->accel_pedal_percentage = 0;
	p_pedals_state->brake_pedal_percentage = 0;
}

void initialise_pedals_faults(PedalsFaults_t* p_pedals_faults)
{
	p_pedals_faults->apps_1_status = NORMAL;
	p_pedals_faults->apps_2_status = NORMAL;
	p_pedals_faults->bse_status = NORMAL;
	p_pedals_faults->apps_implausibility = NORMAL;
	p_pedals_faults->screenshot = false;
}

void initialise_eint_data(EintData_t* p_eint_data)
{
	p_eint_data->power_limit = 0;
	p_eint_data->launch_control = false;
}

void initialise_testing_VCU_data(TestingVCU_t* testing_VCU_data)
{
	testing_VCU_data->AIR_digital = false;
	testing_VCU_data->APPS1 = 0;
	testing_VCU_data->APPS2 = 0;
	testing_VCU_data->BPS1 = 0;
	testing_VCU_data->BSE = 0;
	testing_VCU_data->BSE_Fault = false;
	testing_VCU_data->BSPD_CS = 0;
	testing_VCU_data->CS_fault = false;
	testing_VCU_data->PC_complete = false;
	testing_VCU_data->RTD_button_status = false;
	testing_VCU_data->SAI = 0;
	testing_VCU_data->SAS = 0;
	testing_VCU_data->plausibility_fault = false;
}

void initialise_freertos_task_stack_usage(FreertosTaskStackUsage_t* freertos_task_stack_usage)
{
	freertos_task_stack_usage->unused_words_LogTaskStackUsage = 0;
	freertos_task_stack_usage->unused_words_CalibrateTask = 0;
	freertos_task_stack_usage->unused_words_RTD_Task = 0;
	freertos_task_stack_usage->unused_words_ProcessADC_Task = 0;
	freertos_task_stack_usage->unused_words_PedalsDataTask = 0;
	freertos_task_stack_usage->unused_words_VehicleCtrlTask = 0;
	freertos_task_stack_usage->unused_words_StateCtrlTask = 0;
	freertos_task_stack_usage->unused_words_ProcessCAN_Task = 0;
	freertos_task_stack_usage->unused_words_HandleGUI_Task = 0;
	freertos_task_stack_usage->unused_words_LogVehicleData_Task = 0;
	freertos_task_stack_usage->unused_words_VehicleFaultHandler_Task = 0;
}
