/*
 * log_vehicle_data.c
 *
 *  Created on: 6 Aug 2026
 *      Author: mehta
 */


#include <app/vehicle_fault_handler.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "config/amk_data_structs.h"
#include "config/vehicle_data_structs.h"
#include "app/state_control.h"
#include "config/vcu_config.h"
#include "cmt_utils.h"
#include "app/app_freertos.h"

void Task_VehicleFaultHandler(void *argument)
{
	uint32_t pedals_faults_flags;
	uint32_t apps_implausibility_flags;
	uint32_t screenshot_flag;

	SystemEvent_t system_event;
	SystemState_t current_state;

	for (;;)
	{
		current_state = Get_Current_State();

		pedals_faults_flags = osEventFlagsGet(PedalsOutOfRangeFault_EventHandle);
		apps_implausibility_flags = osEventFlagsGet(APPS_Implausibility_EventHandle);
		screenshot_flag = osEventFlagsGet(Screenshot_EventHandle);

		if (current_state == STATE_FAULTED)
		{
			INVERTER_DISABLE();

			if (pedals_faults_flags == NO_ERRORS
				&& apps_implausibility_flags == NO_ERRORS)
			{
				while (current_state != STATE_FAULTED)
				{
					system_event = EVENT_CLEAR_FAULT;
				    if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				    {
					    p_queue_errors_data->state_transition_errors++;
				    }

				    current_state = Get_Current_State();
				}
			}
		}
		else
		{
			if (pedals_faults_flags != NO_ERRORS
				|| apps_implausibility_flags != NO_ERRORS)
			{
				while (system_state != STATE_FAULTED)
				{
					system_event = EVENT_FAULT;
				    if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				    {
					    p_queue_errors_data->state_transition_errors++;
				    }

				    current_state = Get_Current_State();
				}
			}
		}

		if (current_state == STATE_SCREENSHOTED)
		{
			if (screenshot_flag == NO_ERRORS)
			{
				while (current_state != STATE_RTD)
				{
					system_event = EVENT_SCREENSHOT_OVER;
				    if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				    {
					    p_queue_errors_data->state_transition_errors++;
				    }

				    current_state = Get_Current_State();
				}
			}
		}
		else if (current_state == STATE_RTD)
		{
			if (screenshot_flag != NO_ERRORS)
			{
				while (system_state != STATE_SCREENSHOTED)
				{
					system_event = EVENT_SCREENSHOT;
				    if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				    {
					    p_queue_errors_data->state_transition_errors++;
				    }

				    current_state = Get_Current_State();
				}
			}
		}

		osDelay(1);
	}
}
