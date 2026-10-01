/*
 * RTD.c
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#include <app/RTD.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "config/amk_data_structs.h"
#include "config/vehicle_data_structs.h"
#include "config/vcu_config.h"
#include "cmt_utils.h"
#include "app/app_freertos.h"
#include "app/state_control.h"
#include "app/inverter_control.h"

// Debounce RTD button press to prevent rapid switching between IDLE and RTD states
static volatile bool RTD_to_IDLE_ready = false;
static volatile bool button_last_state = false;

static volatile bool precharge_timer_complete = false;

void Task_RTD(void *argument)
{
  /* USER CODE BEGIN StartReadyToDrive */

	SystemEvent_t system_event;
	bool precharge_complete_received = false;

	bool rtd_button_was_pressed = false;
	TickType_t button_start_tick;
	bool enable_calibration = false;

	SystemState_t current_state;

	bool precharge_complete_timer_started = false;

	INVERTER_DISABLE();

	osDelay(4000);

  /* Infinite loop */
  for(;;)
  {
	  bool button_current = CHECK_RTD_BUTTON_STATUS();
	  bool rtd_button_pressed = button_current && !button_last_state;

	  // Save for next loop
	  button_last_state = button_current;

	  //INVERTER_ENABLE();

	  //bool rtd_button_pressed = CHECK_RTD_BUTTON_STATUS();
	  //p_vehicle_state_data->button_toggled = CHECK_RTD_BUTTON_STATUS();

	  /* For HV Power Supply */

	  /*
	osMutexAcquire(mutex_Vehicle_StateHandle, osWaitForever);
	osMutexAcquire(mutex_Inverter_Setpoints_1Handle, osWaitForever);

	  p_vehicle_state_data -> inverter_enabled = true;
	  osEventFlagsSet(Inverter_EnabledHandle, EVT_INVERTER_ENABLED);
	  p_inverter_setpoints_1 -> control = 1;

	osMutexRelease(mutex_Vehicle_StateHandle);
	osMutexRelease(mutex_Inverter_Setpoints_1Handle);
	*/

	  current_state = Get_Current_State();

	  /* Only enter calibration if we aren't in RTD and if the inverter isn't fully enabled */
	  if (current_state != STATE_RTD && !enable_calibration && !p_inverter_status_1->quit_inverter_on)
	  {

		  /*
		   * Switched from the freeRTOS timer callback to enable calibration to checking within
		   * the task itself as it seemed better to keep all the mutex accessing and queue sending in
		   * the main loop instead of an interruptt
		   *
		   * Request to enter calibration is sent after the button is pressed for CALIBRATION_ENABLE_PERIOD_MILLISECONDS
		   */
		  if (button_current && !p_inverter_status_1->quit_inverter_on) {
		      if (!rtd_button_was_pressed) {
		          rtd_button_was_pressed = true;
		          button_start_tick = xTaskGetTickCount();
		      }
		      if (xTaskGetTickCount() - button_start_tick >= CALIBRATION_ENABLE_PERIOD_MILLISECONDS) {
		          enable_calibration = true;
		      }
		  } else {
		      rtd_button_was_pressed = false;
		  }
	  }

	  if (enable_calibration && current_state != STATE_CALIBRATION && current_state != STATE_FAULTED)
	  {
		  system_event = EVENT_ENTER_CALIBRATION;
		  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
		  {
			  p_queue_errors_data->state_transition_errors++;
		  }
	  }
	  else if (enable_calibration && current_state == STATE_CALIBRATION)
	  {
		  enable_calibration = false;
	  }


	  /* If we aren't calibrating, follow the inverter startup sequence from the amk datasheet
	   * (page 93)
	   */
	  if (current_state != STATE_CALIBRATION && current_state != STATE_FAULTED) {
		  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
		  osMutexAcquire(InverterData1_MutexHandle, osWaitForever);
		  osMutexAcquire(InverterData2_MutexHandle, osWaitForever);

		  if (p_vehicle_state_data->accy_voltage > 0)
		  {
			  p_vehicle_state_data->precharge_percentage = (p_vehicle_state_data->inverter_voltage * 100) / p_vehicle_state_data->accy_voltage;
		  }
		  else
		  {
			  p_vehicle_state_data->precharge_percentage = 0;
		  }

		  if (p_vehicle_state_data->precharge_percentage >= PRECHARGE_PERCENTAGE && p_vehicle_state_data->inverter_voltage > 350)
		  {
			  p_vehicle_state_data -> precharge_voltage_met = true;

			  if (!precharge_timer_complete && !precharge_complete_timer_started)
			  {
				  osTimerStart(Precharge_Complete_TimerHandle, PRECHARGE_COMPLETE_READY_MILLISECONDS);
				  precharge_complete_timer_started = true;
			  }
		  }
		  else
		  {
			  p_vehicle_state_data -> precharge_voltage_met = false;
			  osTimerStop(Precharge_Complete_TimerHandle);
			  precharge_complete_timer_started = false;
			  precharge_timer_complete = false;
		  }

		  if (precharge_timer_complete
		  	  && p_inverter_status_1->system_ready
		  	  && p_inverter_status_2->system_ready)
		  {
			  PRECHARGE_SIGNAL_ENABLE();
			  p_vehicle_state_data->precharge_signal_sent = true;
		  }
		  else
		  {
			  PRECHARGE_SIGNAL_DISABLE();
			  p_vehicle_state_data->precharge_signal_sent = false;
		  }

		  precharge_complete_received = CHECK_PRECHARGE_COMPLETE_STATUS();
		  //p_vehicle_state_data -> precharge_signal_sent = true;

		  if (precharge_complete_received && p_vehicle_state_data->precharge_signal_sent)
		  //if (p_vehicle_state_data -> precharge_signal_sent)
		  {
			  p_vehicle_state_data -> precharge_complete = true;
		  }
		  else
		  {
			  p_vehicle_state_data -> precharge_complete = false;
		  }


		  if (p_vehicle_state_data->precharge_percentage > 98 && current_state != STATE_FAULTED)
		  {
			  p_inverter_setpoints_1 -> control |= (1 << AMK_CONTROL_DC_ON);
			  p_inverter_setpoints_2 -> control |= (1 << AMK_CONTROL_DC_ON);
		  }
		  else
		  {
			  p_inverter_setpoints_1 -> control &= ~(1 << AMK_CONTROL_DC_ON);
			  p_inverter_setpoints_2 -> control &= ~(1 << AMK_CONTROL_DC_ON);
		  }

		  /*
		  if (p_inverter_status_1 -> dc_on && p_inverter_status_1 -> quit_dc_on
			  && p_inverter_status_2 -> dc_on && p_inverter_status_2 -> quit_dc_on
			  && current_state != STATE_FAULTED)
		  {
			  //INVERTER_ENABLE();

			  p_inverter_setpoints_1 -> control |= (1 << AMK_CONTROL_ENABLE);
			  p_inverter_setpoints_1 -> control |= (1 << AMK_CONTROL_INVERTER_ON);

			  p_inverter_setpoints_2 -> control |= (1 << AMK_CONTROL_ENABLE);
			  p_inverter_setpoints_2 -> control |= (1 << AMK_CONTROL_INVERTER_ON);
		  }
		  else
		  {
			  //INVERTER_DISABLE();

			  p_inverter_setpoints_1 -> control &= ~(1 << AMK_CONTROL_ENABLE);
			  p_inverter_setpoints_1 -> control &= ~(1 << AMK_CONTROL_INVERTER_ON);

			  p_inverter_setpoints_2 -> control &= ~(1 << AMK_CONTROL_ENABLE);
			  p_inverter_setpoints_2 -> control &= ~(1 << AMK_CONTROL_INVERTER_ON);
		  }
		  */

		  if (p_inverter_status_1->quit_dc_on &&
			  p_inverter_status_2->quit_dc_on &&
			  !p_vehicle_state_data->inverter_enabled &&
			  current_state == STATE_IDLE)
		  {
			  /* Driver can enter RTD only if the brakes are pressed */
			  if(p_vehicle_state_data->brakes_engaged == true) {
				  RTD_BUTTON_LIGHT_ON();
				  if (rtd_button_pressed)
				  {
					  RTD_BUZZER_ON();
					  osTimerStart(RTD_Button_TimerHandle, RTD_BUTTON_PRESS_MILLISECONDS);
					  osTimerStart(RTD_Buzzer_TimerHandle, RTD_BUZZER_TIME_MILLISECONDS);
					  system_event = EVENT_ENTER_RTD;
					  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
					  {
						  p_queue_errors_data->state_transition_errors++;
					  }

					  p_vehicle_state_data->inverter_enabled = true;

				  }
			  }
			  else
			  {
				  RTD_BUTTON_LIGHT_OFF();
			  }

		  }
		  /* RTD can be exited by pressing the RTD button
		   * mainly used for bench testing */
		  else if (p_vehicle_state_data->inverter_enabled
				   && current_state == STATE_RTD
				   && RTD_to_IDLE_ready)
		  {
			  if (rtd_button_pressed)
			  {
				  /*
				  p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_ENABLE);
				  p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_INVERTER_ON);

				  p_inverter_setpoints_2->control &= ~(1 << AMK_CONTROL_ENABLE);
				  p_inverter_setpoints_2->control &= ~(1 << AMK_CONTROL_INVERTER_ON);
				  */

				  //p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_DC_ON);
				  //p_inverter_setpoints_2->control &= ~(1 << AMK_CONTROL_DC_ON);



				  system_event = EVENT_EXIT_RTD;
				  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				  {
					  p_queue_errors_data->state_transition_errors++;
				  }

				  p_vehicle_state_data->inverter_enabled = false;

				  RTD_to_IDLE_ready = false;
			  }
		  }
			osMutexRelease(VehicleState_MutexHandle);
			osMutexRelease(InverterData1_MutexHandle);
			osMutexRelease(InverterData2_MutexHandle);
	  }

	  else if (current_state == STATE_FAULTED)
	  {
	      osMutexAcquire(InverterData1_MutexHandle, osWaitForever);
	      osMutexAcquire(InverterData2_MutexHandle, osWaitForever);
		  p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_DC_ON);
		  p_inverter_setpoints_2->control &= ~(1 << AMK_CONTROL_DC_ON);
	      osMutexRelease(InverterData2_MutexHandle);
	      osMutexRelease(InverterData1_MutexHandle);

	      osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
	      p_vehicle_state_data->inverter_enabled = false;
	      osMutexRelease(VehicleState_MutexHandle);

	      INVERTER_DISABLE();
	  }

    osDelay(1);
  }
  /* USER CODE END StartReadyToDrive */
}

void Precharge_Complete_Timer_Callback(void *argument)
{
	precharge_timer_complete = true;
}

void RTD_Button_Timer_Callback(void *argument)
{
	RTD_BUTTON_LIGHT_OFF();
	RTD_to_IDLE_ready = true;
}

void RTD_Buzzer_Timer_Callback(void *argument)
{
	RTD_BUZZER_OFF();
}
