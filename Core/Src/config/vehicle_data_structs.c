/*
 * system_data_structures.c
 *
 *  Created on: 18 May 2026
 *      Author: mehta
 */

#include <config/vehicle_data_structs.h>
#include <stdbool.h>
#include <stdint.h>

void initialise_vehicle_state_data(VehicleState_t* p_vehicle_state)
{
	p_vehicle_state->BMS_voltage = 0;
	p_vehicle_state->accy_voltage = 0;
	p_vehicle_state->inverter_voltage = 0;
	p_vehicle_state->precharge_signal_sent = false;
	p_vehicle_state->precharge_voltage_met = false;
	p_vehicle_state->air_aux_closed = false;
	p_vehicle_state->precharge_complete = false;
	p_vehicle_state->button_toggled = false;
	p_vehicle_state->inverter_enabled = false;
}

void initialise_queue_errors(QueueErrors_t* p_queue_errors)
{
	p_queue_errors->state_transition_errors = 0;
	p_queue_errors->can_msg_errors = 0;
	p_queue_errors->pedals_adc_errors = 0;
}

void initialise_pedals_state(PedalsState_t* p_pedals_state)
{
	p_pedals_state->accel_pedal_percentage = 0;
	p_pedals_state->brake_pedal_percentage = 0;
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
