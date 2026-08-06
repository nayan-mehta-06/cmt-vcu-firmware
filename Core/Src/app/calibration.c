/*
 * calibration.c
 *
 *  Created on: 18 Jul 2026
 *      Author: mehta
 */


#include <app/calibration.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "config/amk_data_structs.h"
#include "config/vehicle_data_structs.h"
#include "app/process_adc.h"
#include "config/vcu_config.h"
#include "cmt_utils.h"
#include "drivers/cmt_M95512.h"
#include "app/app_freertos.h"
#include "app/state_control.h"

void Task_Car_Calibration(void *argument)
{
	SystemEvent_t system_event;
	SystemState_t current_state;

	PedalsADC_t pedals_adc_data;
	CalibrationData_t calibration_data;

	uint8_t accelerator_pedal_calibration_values[8] = {0};
	uint8_t brake_pedal_calibration_values[8] = {0};

	uint32_t calibration_data_flag;

  for(;;)
  {
	  osMutexAcquire(SystemState_MutexHandle, osWaitForever);
	  	  current_state = system_state;
	  osMutexRelease(SystemState_MutexHandle);


	  if (current_state == STATE_CALIBRATION)
	  {

		  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
		  p_vehicle_state_data->calibrating = true;
		  p_vehicle_state_data->calibration_complete = false;
		  osMutexRelease(VehicleState_MutexHandle);

		  osDelay(300);
		  RTD_BUTTON_LIGHT_OFF();

		  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
		  {
			  p_queue_errors_data->pedals_adc_errors++;
		  }

		  calibration_data.apps_1_min_value = pedals_adc_data.apps_1_adc;
		  calibration_data.apps_2_min_value = pedals_adc_data.apps_2_adc;
		  calibration_data.bse_min_value = pedals_adc_data.bse_adc;

		osDelay(1000);

		RTD_BUTTON_LIGHT_ON();
		do
		{
			  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
			  {
				  p_queue_errors_data->pedals_adc_errors++;
			  }

			  calibration_data.bse_max_value = pedals_adc_data.bse_adc;
		}
		while (!CHECK_RTD_BUTTON_STATUS());
		RTD_BUTTON_LIGHT_OFF();

		osDelay(1000);

		RTD_BUTTON_LIGHT_ON();
		do
		{
			  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
			  {
				  p_queue_errors_data->pedals_adc_errors++;
			  }

			  calibration_data.apps_1_max_value = pedals_adc_data.apps_1_adc;
			  calibration_data.apps_2_max_value = pedals_adc_data.apps_2_adc;
		}
		while (!CHECK_RTD_BUTTON_STATUS());
		RTD_BUTTON_LIGHT_OFF();

		accelerator_pedal_calibration_values[0] = (calibration_data.apps_1_min_value % 256);
		accelerator_pedal_calibration_values[1] = (calibration_data.apps_1_min_value / 256);
		accelerator_pedal_calibration_values[4] = (calibration_data.apps_2_max_value % 256);
		accelerator_pedal_calibration_values[5] = (calibration_data.apps_2_max_value / 256);

		brake_pedal_calibration_values[0] = (calibration_data.bse_min_value % 256);
		brake_pedal_calibration_values[1] = (calibration_data.bse_min_value / 256);

		accelerator_pedal_calibration_values[2] = (calibration_data.apps_1_max_value % 256);
		accelerator_pedal_calibration_values[3] = (calibration_data.apps_1_max_value / 256);
		accelerator_pedal_calibration_values[6] = (calibration_data.apps_2_max_value % 256);
		accelerator_pedal_calibration_values[7] = (calibration_data.apps_2_max_value / 256);

		brake_pedal_calibration_values[2] = (calibration_data.bse_max_value % 256);
		brake_pedal_calibration_values[3] = (calibration_data.bse_max_value / 256);

		// Save data to EEPROM
		vTaskSuspendAll();

		// Enable writing to EEPROM
		EEPROM_WriteEnable();

		// Write to eeprom
		// Page size refers to number of bytes
		EEPROM_WritePage(0x20, accelerator_pedal_calibration_values, 8);
		//EEPROM_WritePage(0x28, accelerator_pedal_deadzone_offset_values, FOUR_BYTES);
		EEPROM_WritePage(0x2C, brake_pedal_calibration_values, 8);
		//EEPROM_WritePage(0x34, brake_pedal_deadzone_offset_values, FOUR_BYTES);

		EEPROM_WriteDisable();

		xTaskResumeAll();

		  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
		  p_vehicle_state_data->calibrating = false;
		  p_vehicle_state_data->calibration_complete = true;
		  osMutexRelease(VehicleState_MutexHandle);

	        osEventFlagsSet(
				CalibrationDataReceived_EventHandle,
				CALIBRATION_DATA_READY
	        );

		  do
		  {
			  if (osMessageQueuePut(CalibrationDataQueueHandle, &calibration_data, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
			  {
				  p_queue_errors_data->calibration_data_errors++;
			  }

			  calibration_data_flag = osEventFlagsGet(CalibrationDataReceived_EventHandle);
		  }
		  while (!(calibration_data_flag & CALIBRATION_DATA_RECEIVED));


		  osMutexAcquire(SystemState_MutexHandle, osWaitForever);
		  	  current_state = system_state;
		  osMutexRelease(SystemState_MutexHandle);

		  while (current_state == STATE_CALIBRATION)
		  {
			  system_event = EVENT_CALIBRATION_COMPLETE;

			  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
			  {
				  p_queue_errors_data->state_transition_errors++;
			  }

			  osMutexAcquire(SystemState_MutexHandle, osWaitForever);
			  	  current_state = system_state;
			  osMutexRelease(SystemState_MutexHandle);
		  }


	  }

    osDelay(1);
  }
}
