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
#include "drivers/adc_driver.h"

void Task_Car_Calibration(void *argument)
{
	SystemEvent_t system_event;
	SystemState_t current_state;

	PedalsADC_t pedals_adc_data;
	CalibrationData_t calibration_data;

	/*
	 * Only one array is written/read to the EEPROM, which has been changed from previous years where
	 * multiple arrays were written to it. Pointers to indexes in the big array have been defined so
	 * the code still thinks theres one array for accel pedal values and one for brakes
	 */
	uint8_t pedal_calibration_values[12] = {0};
	uint8_t *accelerator_pedal_calibration_values = &pedal_calibration_values[0];
	uint8_t *brake_pedal_calibration_values = &pedal_calibration_values[8];

	uint32_t calibration_data_flag;

  for(;;)
  {
	  current_state = Get_Current_State();

	  /*
	   * The task will only calibrate if the mode has been enabled (duh)
	   * */
	  if (current_state == STATE_CALIBRATION)
	  {

		  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
		  p_vehicle_state_data->calibrating = true;
		  p_vehicle_state_data->calibration_complete = false;
		  osMutexRelease(VehicleState_MutexHandle);

		  /*
		   * Calibration will only begin once the driver let goes of the RTD button, as calibration is enabled
		   * in the RTD task by just holding the button down for 3 seconds
		   * */
		  bool rtd_button_pressed = CHECK_RTD_BUTTON_STATUS();
		  while (rtd_button_pressed)
		  {
			  RTD_BUTTON_LIGHT_ON();
			  rtd_button_pressed = CHECK_RTD_BUTTON_STATUS();
		  }
		  RTD_BUTTON_LIGHT_OFF();

		  osDelay(500);

		  /*
		   * The minimum values of the accel and brake sensors are recorded first (driver should not press down
		   * on the pedals at all once they enter calibration at all
		   * */
		  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
		  {
			  p_queue_errors_data->pedals_adc_errors++;
		  }

		  //calibration_data.apps_1_min_value = pedals_adc_data.apps_1_adc;
		  //calibration_data.apps_2_min_value = pedals_adc_data.apps_2_adc;
		  //calibration_data.bse_min_value = pedals_adc_data.bse_adc;

		  osMutexAcquire(RawADC_Values_MutexHandle, osWaitForever);
			  calibration_data.apps_1_min_value = Get_ADC_Channel(APPS1_ADC_CHANNEL);
			  calibration_data.apps_2_min_value = Get_ADC_Channel(APPS2_ADC_CHANNEL);
			  calibration_data.bse_min_value = Get_ADC_Channel(BSE_ADC_CHANNEL);
		  osMutexRelease(RawADC_Values_MutexHandle);

		osDelay(500);


		/*
		 * Record the maximum sensor values when the brake pedal is fully pressed down by continually receiving
		 * values from the process_adc task until the driver presses the RTD button
		 * */
		RTD_BUTTON_LIGHT_ON();
		do
		{
			  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
			  {
				  p_queue_errors_data->pedals_adc_errors++;
			  }

			  //calibration_data.bse_max_value = pedals_adc_data.bse_adc;

			  osMutexAcquire(RawADC_Values_MutexHandle, osWaitForever);
			  calibration_data.bse_max_value = Get_ADC_Channel(BSE_ADC_CHANNEL);
			  osMutexRelease(RawADC_Values_MutexHandle);
		}
		while (!CHECK_RTD_BUTTON_STATUS());
		RTD_BUTTON_LIGHT_OFF();

		osDelay(500);


		/*
		 * Record the maximum sensor values when the accel pedal is fully pressed down by continually receiving
		 * values from the process_adc task until the driver presses the RTD button
		 * */
		RTD_BUTTON_LIGHT_ON();
		do
		{
			  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
			  {
				  p_queue_errors_data->pedals_adc_errors++;
			  }
			  //calibration_data.apps_1_max_value = pedals_adc_data.apps_1_adc;
			  //calibration_data.apps_2_max_value = pedals_adc_data.apps_2_adc;

			  osMutexAcquire(RawADC_Values_MutexHandle, osWaitForever);
			  calibration_data.apps_1_max_value = Get_ADC_Channel(APPS1_ADC_CHANNEL);
			  calibration_data.apps_2_max_value = Get_ADC_Channel(APPS2_ADC_CHANNEL);
			  osMutexRelease(RawADC_Values_MutexHandle);
		}
		while (!CHECK_RTD_BUTTON_STATUS());
		RTD_BUTTON_LIGHT_OFF();

		/*
		 * Store all values into the main array that will be written to the eeprom
		 * */
		accelerator_pedal_calibration_values[0] = (calibration_data.apps_1_min_value % 256);
		accelerator_pedal_calibration_values[1] = (calibration_data.apps_1_min_value / 256);
		accelerator_pedal_calibration_values[4] = (calibration_data.apps_2_min_value % 256);
		accelerator_pedal_calibration_values[5] = (calibration_data.apps_2_min_value / 256);

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
		EEPROM_WritePage(0x420, pedal_calibration_values, 12);

		//EEPROM_WritePage(0x28, accelerator_pedal_deadzone_offset_values, FOUR_BYTES);

		//EEPROM_WritePage(0x34, brake_pedal_deadzone_offset_values, FOUR_BYTES);

		EEPROM_WriteDisable();

		xTaskResumeAll();

		  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
		  p_vehicle_state_data->calibrating = false;
		  p_vehicle_state_data->calibration_complete = true;
		  osMutexRelease(VehicleState_MutexHandle);

		  /* Tell the process pedals data task that there is calibration data ready to be received */
		  osEventFlagsClear(CalibrationDataReceived_EventHandle, CALIBRATION_DATA_RECEIVED);
		  osEventFlagsSet(CalibrationDataReceived_EventHandle, CALIBRATION_DATA_READY);

		  /* Send the new calibration data again and again to the process pedals data task until it receives it and sets the received event bit
		   * in the event flag*/
		  do
		  {
			  if (osMessageQueuePut(CalibrationDataQueueHandle, &calibration_data, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
			  {
				  p_queue_errors_data->calibration_data_errors++;
			  }

			  calibration_data_flag = osEventFlagsGet(CalibrationDataReceived_EventHandle);
		  }
		  while (!(calibration_data_flag & CALIBRATION_DATA_RECEIVED));


		  current_state = Get_Current_State();

		  /* Exit calibration mode by checking if the current state has changed after sending the corresponding event */
		  while (current_state == STATE_CALIBRATION)
		  {
			  system_event = EVENT_CALIBRATION_COMPLETE;

			  if (osMessageQueuePut(StateTransitionQueueHandle, &system_event, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
			  {
				  p_queue_errors_data->state_transition_errors++;
			  }

			  current_state = Get_Current_State();
		  }

	  }

    osDelay(1);
  }
}
