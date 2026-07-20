/*
 * amk_data_structs.c
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#include <config/amk_data_structs.h>
#include <stdbool.h>
#include <stdint.h>

void initialise_inverter_data(InverterData_t* p_inverter_data)
{
    p_inverter_data->status = 0;
    p_inverter_data->actual_speed_value = 0;
    p_inverter_data->torque_current_raw = 0;
    p_inverter_data->magnetizing_current_raw = 0;
    p_inverter_data->temp_motor = 0;
    p_inverter_data->temp_inverter = 0;
    p_inverter_data->diagnostic_number = 0;
    p_inverter_data->temp_IGBT = 0;
}

void initialise_inverter_status(InverterStatus_t* p_inverter_status)
{
    p_inverter_status->system_ready = false;
    p_inverter_status->error = false;
    p_inverter_status->warn = false;
    p_inverter_status->quit_dc_on = false;
    p_inverter_status->dc_on = false;
    p_inverter_status->quit_inverter_on = false;
    p_inverter_status->inverter_on = false;
    p_inverter_status->derating = false;
}

void initialise_inverter_setpoints(InverterSetpoints_t* p_inverter_setpoints)
{
	p_inverter_setpoints->control = 0x00;
	p_inverter_setpoints->torque_setpoint = 0;
	p_inverter_setpoints->torque_limit_positive = 0;
	p_inverter_setpoints->torque_limit_negative = 0;
}
