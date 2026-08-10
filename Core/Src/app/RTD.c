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

void Task_RTD(void *argument)
{
  /* USER CODE BEGIN StartReadyToDrive */

	SystemEvent_t system_event;
	//bool is_pressed = false;
	bool precharge_complete_received = false;
	bool calibration_active = false;
	uint16_t inverter_voltage_percentage;

	bool rtd_button_was_pressed = false;
	TickType_t button_start_tick;
	bool enable_calibration = false;

	INVERTER_DISABLE();

  /* Infinite loop */
  for(;;)
  {
	  //static bool button_last_state = false;

	  //bool button_current = CHECK_RTD_BUTTON_STATUS();
	  //bool rtd_button_pressed = button_current && !button_last_state;

	  // Save for next loop
	  //button_last_state = button_current;

	  bool rtd_button_pressed = CHECK_RTD_BUTTON_STATUS();

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


	  osMutexAcquire(SystemState_MutexHandle, osWaitForever);

	  if (system_state != STATE_RTD && !calibration_active && !p_inverter_status_1->quit_inverter_on)
	  {
		  if (rtd_button_pressed) {
			  if (!rtd_button_was_pressed) {
				  rtd_button_was_pressed = true;
				  button_start_tick = xTaskGetTickCount();
			  }

			  if (xTaskGetTickCount() - button_start_tick >= CALIBRATION_ENABLE_PERIOD_MILLISECONDS)
			  {
				  enable_calibration = true;
			  }
		  }
		  else
		  {
			  rtd_button_was_pressed = false;
		  }
	  }

	  if (enable_calibration && system_state != STATE_CALIBRATION)
	  {
		  system_event = EVENT_ENTER_CALIBRATION;
		  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
		  {
			  p_queue_errors_data->state_transition_errors++;
		  }
	  }
	  else {
		  enable_calibration = false;
	  }


	  if (system_state != STATE_CALIBRATION) {
		  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
		  osMutexAcquire(InverterData1_MutexHandle, osWaitForever);

		  inverter_voltage_percentage = (p_vehicle_state_data->inverter_voltage * 100) / (p_vehicle_state_data->BMS_voltage);

		  if (inverter_voltage_percentage >= PRECHARGE_PERCENTAGE && p_vehicle_state_data->inverter_voltage > 40)
		  {
			  p_vehicle_state_data -> precharge_voltage_met = true;
		  }
		  else
		  {
			  p_vehicle_state_data -> precharge_voltage_met = false;
		  }

		  PRECHARGE_SIGNAL_ENABLE();
		  p_vehicle_state_data -> precharge_signal_sent = true;
		  /*
		  if (p_vehicle_state_data -> precharge_voltage_met && p_inverter_status_1->system_ready)
		  {
			  PRECHARGE_SIGNAL_ENABLE();
			  p_vehicle_state_data -> precharge_signal_sent = true;
		  }
		  else
		  {
			  PRECHARGE_SIGNAL_DISABLE();
			  p_vehicle_state_data -> precharge_signal_sent = false;
		  }
		  */

		  //precharge_complete_received = CHECK_PRECHARGE_COMPLETE_STATUS();

		  precharge_complete_received = true;

		  if (precharge_complete_received && p_vehicle_state_data -> precharge_signal_sent)
		  //if (p_vehicle_state_data -> precharge_signal_sent)
		  {
			  p_vehicle_state_data -> precharge_complete = true;
		  }
		  else
		  {
			  p_vehicle_state_data -> precharge_complete = false;
		  }

		  if (p_vehicle_state_data -> precharge_complete)
		  {
			  p_inverter_setpoints_1 -> control |= (1 << AMK_CONTROL_DC_ON);
		  }
		  else
		  {
			  p_inverter_setpoints_1 -> control &= ~(1 << AMK_CONTROL_DC_ON);
		  }

		  if (p_inverter_status_1 -> dc_on && p_inverter_status_1 -> quit_dc_on)
		  {
			  INVERTER_ENABLE();

			  p_inverter_setpoints_1 -> control |= (1 << AMK_CONTROL_ENABLE);
			  p_inverter_setpoints_1 -> control |= (1 << AMK_CONTROL_INVERTER_ON);
		  }
		  else
		  {
			  INVERTER_DISABLE();

			  p_inverter_setpoints_1 -> control &= ~(1 << AMK_CONTROL_ENABLE);
			  p_inverter_setpoints_1 -> control &= ~(1 << AMK_CONTROL_INVERTER_ON);
		  }


		  /*
		  if (p_inverter_status_1 -> quit_inverter_on) {
			  p_vehicle_state_data -> inverter_enabled = true;
			  osEventFlagsSet(Inverter_EnabledHandle, EVT_INVERTER_ENABLED);
		  }
		  else {
			  p_vehicle_state_data -> inverter_enabled = false;
			  osEventFlagsSet(Inverter_EnabledHandle, EVT_INVERTER_DISABLED);
		  }
		*/

		  if (p_inverter_status_1->quit_inverter_on &&
			  !p_vehicle_state_data->inverter_enabled &&
			  system_state == STATE_IDLE)
		  {
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
		  else if (p_vehicle_state_data->inverter_enabled &&
				   system_state == STATE_RTD)
		  {
			  if (rtd_button_pressed)
			  {
				  p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_ENABLE);
				  p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_INVERTER_ON);

				  system_event = EVENT_EXIT_RTD;
				  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
				  {
					  p_queue_errors_data->state_transition_errors++;
				  }

				  p_vehicle_state_data->inverter_enabled = false;
			  }
		  }

			osMutexRelease(VehicleState_MutexHandle);
			osMutexRelease(InverterData1_MutexHandle);
	  }

	osMutexRelease(SystemState_MutexHandle);

    osDelay(1);
  }
  /* USER CODE END StartReadyToDrive */
}



void RTD_Button_Timer_Callback(void *argument)
{
	RTD_BUTTON_LIGHT_OFF();
}

void RTD_Buzzer_Timer_Callback(void *argument)
{
	RTD_BUZZER_OFF();
}
