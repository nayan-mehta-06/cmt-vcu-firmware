/*
 * state_control.c
 *
 *  Created on: 17 May 2026
 *      Author: mehta
 */

#include "main.h"
#include "freeRTOS.h"
#include "cmsis_os.h"
#include "app/state_control.h"
#include "app/app_freertos.h"
#include "config/vcu_config.h"

void Task_State_Ctrl(void *argument)
{
  /* USER CODE BEGIN Start_State_Ctrl_Task */
	SystemEvent_t most_recent_transition;
  /* Infinite loop */
  for(;;)
  {
			if (osMessageQueueGet(StateTransitionQueueHandle,
								  &most_recent_transition,
								  NULL,
								  0) != osOK)
			{
				// p_queue_errors_data->state_transition_errors++;
			}

	  osMutexAcquire(SystemState_MutexHandle, osWaitForever);
		  switch (system_state)
		  {
			  case STATE_IDLE:
				  if (most_recent_transition == EVENT_ENTER_CALIBRATION)
				  {
					  system_state = STATE_CALIBRATION;
				  }
				  else if (most_recent_transition == EVENT_ENTER_RTD)
				  {
					  system_state = STATE_RTD;
				  }
				  else if (most_recent_transition == EVENT_FAULT)
				  {
					  system_state = STATE_FAULTED;
				  }
				  break;

			  case STATE_CALIBRATION:
				  if (most_recent_transition == EVENT_ENTER_CALIBRATION)
				  {
					  system_state = STATE_CALIBRATION;
				  }
				  else if (most_recent_transition == EVENT_FAULT)
				  {
					  system_state = STATE_FAULTED;
				  }
				  break;

			  case STATE_RTD:
				  if (most_recent_transition == EVENT_EXIT_RTD)
				  {
					  system_state = STATE_IDLE;
				  }
				  else if (most_recent_transition == EVENT_SCREENSHOT)
				  {
					  system_state = STATE_SCREENSHOTED;
				  }
				  else if (most_recent_transition == EVENT_FAULT)
				  {
					  system_state = STATE_FAULTED;
				  }
				  break;

			  case STATE_SCREENSHOTED:
				  if (most_recent_transition == EVENT_SCREENSHOT_OVER)
				  {
					  system_state = STATE_RTD;
				  }
				  else if (most_recent_transition == EVENT_FAULT)
				  {
					  system_state = STATE_FAULTED;
				  }
				  break;

			  case STATE_FAULTED:
				  if (most_recent_transition == EVENT_CLEAR_FAULT)
				  {
					  system_state = STATE_IDLE;
				  }
				  break;
		  }
	  osMutexRelease(SystemState_MutexHandle);


    osDelay(1);
  }
  /* USER CODE END start_task */
}

SystemState_t Get_Current_State(void)
{
	SystemState_t current_state;

	osMutexAcquire(SystemState_MutexHandle, osWaitForever);
		current_state = system_state;
	osMutexRelease(SystemState_MutexHandle);

	return current_state;
}
