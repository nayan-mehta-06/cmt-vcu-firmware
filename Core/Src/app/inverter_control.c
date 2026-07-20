/*
 * inverter_control.c
 *
 *  Created on: Feb 2, 2026
 *      Author: mehta
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/inverter_control.h"
#include "app/app_freertos.h"
#include "drivers/can_driver.h"
#include "config/can_ids.h"


void setInverterData1(InverterData_t *inv_data, uint8_t *data)
{
	inv_data->status 					= (uint16_t)(data[0] | data[1] << 8);
	inv_data->actual_speed_value 		= (int16_t)(data[2] | data[3] << 8);
	inv_data->torque_current_raw 		= (int16_t)(data[4] | data[5] << 8);
	inv_data->magnetizing_current_raw 	= (int16_t)(data[6] | data[7] << 8);
}

void processInverterStatus(InverterStatus_t *inv_status, uint16_t status)
{
    inv_status->system_ready    	= (status & AMK_STATUS_SYSTEM_READY_MASK) != 0U;
    inv_status->error 				= (status & AMK_STATUS_ERROR_ACTIVE_MASK) != 0U;
    inv_status->warn    			= (status & AMK_STATUS_WARNING_ACTIVE_MASK) != 0U;
    inv_status->quit_dc_on      	= (status & AMK_STATUS_QUIT_DC_ON_MASK) != 0U;
    inv_status->dc_on             	= (status & AMK_STATUS_DC_ON_MASK) != 0U;
    inv_status->quit_inverter_on  	= (status & AMK_STATUS_QUIT_INVERTER_ON_MASK) != 0U;
    inv_status->inverter_on       	= (status & AMK_STATUS_INVERTER_ON_MASK) != 0U;
    inv_status->derating   			= (status & AMK_STATUS_DERATING_ACTIVE_MASK) != 0U;
}

void setInverterData2(InverterData_t *inv_data, uint8_t *data)
{
	inv_data->temp_motor 		= (int16_t)(data[0] | data[1] << 8);
	inv_data->temp_inverter 	= (int16_t)(data[2] | data[3] << 8);
	inv_data->diagnostic_number = (uint16_t)(data[4] | data[5] << 8);
	inv_data->temp_IGBT 		= (int16_t)(data[6] | data[7] << 8);
}


bool transmit_inverter_command(InverterSetpoints_t *p_inverter_setpoints, uint8_t inverter_node_number)
{
	bool torque_command_transmission_sucessful = false;

	// Initialise buffer for CAN data
	uint8_t txData[8] = {0};
	uint16_t can_address = AMK_SEND_MSG_1 + inverter_node_number;


	osMutexAcquire(InverterData1_MutexHandle, osWaitForever);

		txData[0] = (uint8_t)(p_inverter_setpoints_1 -> control & 0xFF);
		txData[1] = (uint8_t)(p_inverter_setpoints_1 -> control >> 8);

		txData[2] = (uint8_t)(p_inverter_setpoints_1 -> torque_setpoint & 0xFF);
		txData[3] = (uint8_t)(p_inverter_setpoints_1 -> torque_setpoint >> 8);

		txData[4] = (uint8_t)(p_inverter_setpoints_1 -> torque_limit_positive & 0xFF);
		txData[5] = (uint8_t)(p_inverter_setpoints_1 -> torque_limit_positive >> 8);

		txData[6] = (uint8_t)(p_inverter_setpoints_1 -> torque_limit_negative & 0xFF);
		txData[7] = (uint8_t)(p_inverter_setpoints_1 -> torque_limit_negative >> 8);

    osMutexRelease(InverterData1_MutexHandle);

	if ( CAN_transmit(txData, 8, can_address, CAN_1, CAN_STD_ID_FORMAT) )
	{
		torque_command_transmission_sucessful = true;
	}
	else
	{
		torque_command_transmission_sucessful = false;
	}

	return torque_command_transmission_sucessful;
}


