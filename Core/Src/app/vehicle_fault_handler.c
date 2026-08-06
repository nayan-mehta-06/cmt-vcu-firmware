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

	SystemEvent_t system_event;
	SystemState_t current_state;

	for (;;)
	{
		osMutexAcquire(SystemState_MutexHandle, osWaitForever);
			current_state = system_state;
		osMutexRelease(SystemState_MutexHandle);

		pedals_faults_flags = osEventFlagsGet(PedalsOutOfRangeFault_EventHandle);

		if (current_state == STATE_FAULTED)
		{
			if (pedals_faults_flags == NO_ERRORS)
			{
				osMutexAcquire(SystemState_MutexHandle, osWaitForever);

				while (system_state != STATE_FAULTED)
				{
					system_event = EVENT_CLEAR_FAULT;
				    if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				    {
					    p_queue_errors_data->state_transition_errors++;
				    }
				}

				osMutexRelease(SystemState_MutexHandle);
			}
		}
		else
		{
			if (pedals_faults_flags != NO_ERRORS)
			{
				osMutexAcquire(SystemState_MutexHandle, osWaitForever);

				while (system_state != STATE_FAULTED)
				{
					system_event = EVENT_FAULT;
				    if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				    {
					    p_queue_errors_data->state_transition_errors++;
				    }
				}

				osMutexRelease(SystemState_MutexHandle);
			}
		}



		osDelay(1);
	}
}
