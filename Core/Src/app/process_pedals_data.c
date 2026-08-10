/*
 * process_pedals_data.c
 *
 *  Created on: 17 Jul 2026
 *      Author: mehta
 */

#include <stdlib.h>
#include "app/process_pedals_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/app_freertos.h"
#include "config/vehicle_data_structs.h"
#include "app/process_adc.h"
#include "config/vcu_config.h"
#include "app/state_control.h"
#include "app/calibration.h"
#include "drivers/CMT_M95512.h"
#include "cmt_utils.h"

void Task_Process_Pedals_Data(void *argument)
{
	PedalsADC_t pedals_adc_data;
	CalibrationData_t calibration_data;

	uint16_t apps_1_percentage, apps_2_percentage;
	uint16_t accel_pedal_percentage, brake_pedal_percentage;

	uint8_t apps_1_min, apps_1_max;
	uint8_t apps_2_min, apps_2_max;
	uint8_t bse_min, bse_max;

	uint8_t accelerator_pedal_calibration_values[8] = {0};
	uint8_t brake_pedal_calibration_values[8] = {0};

	bool apps_1_fault_timer_started = false,
		 apps_2_fault_timer_started = false,
		 bse_fault_timer_started = false,
		 apps_implausibility_timer_started = false,
		 screenshot_timer_started = false;

	PedalsFaults_t previous_pedals_faults;
	PedalsFaults_t current_pedals_faults;

	SystemState_t current_state;

	vTaskSuspendAll();

	// Read most recent calibration data
	EEPROM_ReadData(0x20, accelerator_pedal_calibration_values, 8);
	EEPROM_ReadData(0x2C, brake_pedal_calibration_values, 8);

	xTaskResumeAll();

	apps_1_min = (accelerator_pedal_calibration_values[0] + (accelerator_pedal_calibration_values[1] * 256));
	apps_1_max = (accelerator_pedal_calibration_values[2] + (accelerator_pedal_calibration_values[3] * 256));
	apps_2_min = (accelerator_pedal_calibration_values[4] + (accelerator_pedal_calibration_values[5] * 256));
	apps_2_max = (accelerator_pedal_calibration_values[6] + (accelerator_pedal_calibration_values[7] * 256));

	bse_min = (brake_pedal_calibration_values[0] + (brake_pedal_calibration_values[1] * 256));
	bse_max = (brake_pedal_calibration_values[2] + (brake_pedal_calibration_values[3] * 256));

  /* Infinite loop */
  for(;;)
  {
	  osMutexAcquire(SystemState_MutexHandle, osWaitForever);
	  	  current_state = system_state;
	  osMutexRelease(SystemState_MutexHandle);


	  if (current_state != STATE_CALIBRATION)
	  {
		  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
		  {
			  p_queue_errors_data->pedals_adc_errors++;
		  }

		  current_pedals_faults.apps_1_status = pedal_position_sensor_out_of_range_fault_check(pedals_adc_data.apps_1_adc,
																	  APPS_1_OPEN_CIRCUIT_FAULT_THRESHOLD,
																	  APPS_1_SHORT_CIRCUIT_FAULT_THRESHOLD);

		  current_pedals_faults.apps_2_status = pedal_position_sensor_out_of_range_fault_check(pedals_adc_data.apps_2_adc,
																	  APPS_2_OPEN_CIRCUIT_FAULT_THRESHOLD,
																	  APPS_2_SHORT_CIRCUIT_FAULT_THRESHOLD);

		  current_pedals_faults.bse_status = pedal_position_sensor_out_of_range_fault_check(pedals_adc_data.bse_adc,
																	  BSE_OPEN_CIRCUIT_FAULT_THRESHOLD,
																	  BSE_SHORT_CIRCUIT_FAULT_THRESHOLD);

		  osMutexAcquire(PedalsFaults_MutexHandle, osWaitForever);
		  	  Get_Previous_Pedals_Faults_Data(&previous_pedals_faults);
		  osMutexRelease(PedalsFaults_MutexHandle);

		  if (current_pedals_faults.apps_1_status != NORMAL
			  && previous_pedals_faults.apps_1_status == NORMAL
			  && !apps_1_fault_timer_started)
		  {
			  osTimerStart(APPS_1_OutOfRange_TimerHandle, PEDALS_FAULT_TIMEOUT_MILLISECONDS);
			  apps_1_fault_timer_started = true;
		  }
		  else if (current_pedals_faults.apps_1_status == NORMAL
			  && previous_pedals_faults.apps_1_status == NORMAL
			  && apps_1_fault_timer_started)
		  {
			  osTimerStop(APPS_1_OutOfRange_TimerHandle);
			  apps_1_fault_timer_started = false;
		  }
		  else if (current_pedals_faults.apps_1_status == NORMAL
			  && !(previous_pedals_faults.apps_1_status == NORMAL))
		  {
			osEventFlagsClear(
				PedalsOutOfRangeFault_EventHandle,
				APPS_1_OUT_OF_RANGE
			);
		  }

		  if (current_pedals_faults.apps_2_status != NORMAL
			  && previous_pedals_faults.apps_2_status == NORMAL
			  && !apps_2_fault_timer_started)
		  {
			  osTimerStart(APPS_2_OutOfRange_TimerHandle, PEDALS_FAULT_TIMEOUT_MILLISECONDS);
			  apps_2_fault_timer_started = true;
		  }
		  else if (current_pedals_faults.apps_2_status == NORMAL
			  && previous_pedals_faults.apps_2_status == NORMAL
			  && apps_2_fault_timer_started)
		  {
			  osTimerStop(APPS_2_OutOfRange_TimerHandle);
			  apps_2_fault_timer_started = false;
		  }
		  else if (current_pedals_faults.apps_2_status == NORMAL
			  && !(previous_pedals_faults.apps_2_status == NORMAL))
		  {
			osEventFlagsClear(
				PedalsOutOfRangeFault_EventHandle,
				APPS_2_OUT_OF_RANGE
			);
		  }

		  if (current_pedals_faults.bse_status != NORMAL
			  && previous_pedals_faults.bse_status == NORMAL
			  && !bse_fault_timer_started)
		  {
			  osTimerStart(BSE_OutOfRange_TimerHandle, PEDALS_FAULT_TIMEOUT_MILLISECONDS);
			  bse_fault_timer_started = true;
		  }
		  else if (current_pedals_faults.bse_status == NORMAL
			  && previous_pedals_faults.bse_status == NORMAL
			  && bse_fault_timer_started)
		  {
			  osTimerStop(BSE_OutOfRange_TimerHandle);
			  bse_fault_timer_started = false;
		  }
		  else if (current_pedals_faults.bse_status == NORMAL
			  && !(previous_pedals_faults.bse_status == NORMAL))
		  {
			osEventFlagsClear(
				PedalsOutOfRangeFault_EventHandle,
				BSE_OUT_OF_RANGE
			);
		  }

		  apps_1_percentage = calc_adc_percentage(pedals_adc_data.apps_1_adc, apps_1_min, apps_1_max);
		  apps_2_percentage = calc_adc_percentage(pedals_adc_data.apps_2_adc, apps_2_min, apps_2_max);
		  accel_pedal_percentage = (apps_1_percentage + apps_2_percentage) / 2;

		  brake_pedal_percentage = calc_adc_percentage(pedals_adc_data.bse_adc, bse_min, bse_max);

		  current_pedals_faults.apps_implausibility = APPS_Implausibility_Check(apps_1_percentage,
				  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	apps_2_percentage);

		  if (current_pedals_faults.apps_implausibility == TRUE
			  && previous_pedals_faults.apps_implausibility == FALSE
			  && !apps_implausibility_timer_started)
		  {
			  osTimerStart(APPS_Implausibility_TimerHandle, PEDALS_FAULT_TIMEOUT_MILLISECONDS);
			  apps_implausibility_timer_started = true;
		  }
		  else if (current_pedals_faults.apps_implausibility == FALSE
			  && previous_pedals_faults.apps_implausibility == FALSE
			  && apps_implausibility_timer_started)
		  {
			  osTimerStop(APPS_Implausibility_TimerHandle);
			  apps_implausibility_timer_started = false;
		  }
		  else if (current_pedals_faults.apps_implausibility == FALSE
			  && !(previous_pedals_faults.apps_implausibility == TRUE))
		  {
			osEventFlagsClear(
				APPS_Implausibility_EventHandle,
				APPS_IMPLAUSIBILITY
			);
		  }

		  current_pedals_faults.screenshot = Screenshot_Check(apps_1_percentage, apps_2_percentage,
				  	  	  	  	  	  	  	  	  	  	  	   brake_pedal_percentage,
															   BRAKES_ENGAGED_THRESHOLD_PERCENTAGE,
															   current_pedals_faults.screenshot);

		  if (current_pedals_faults.screenshot == TRUE
			  && previous_pedals_faults.screenshot == FALSE
			  && !screenshot_timer_started)
		  {
			  osTimerStart(Screenshot_TimerHandle, PEDALS_FAULT_TIMEOUT_MILLISECONDS);
			  screenshot_timer_started = true;
		  }
		  else if (current_pedals_faults.screenshot == FALSE
			  && previous_pedals_faults.screenshot == FALSE
			  && screenshot_timer_started)
		  {
			  osTimerStop(Screenshot_TimerHandle);
			  screenshot_timer_started = false;
		  }
		  else if (current_pedals_faults.screenshot == FALSE
			  && !(previous_pedals_faults.screenshot == TRUE))
		  {
			osEventFlagsClear(
				Screenshot_EventHandle,
				SCREENSHOT
			);
		  }

		  osMutexAcquire(PedalsState_MutexHandle, osWaitForever);

			  p_pedals_state_data->apps_1_percentage = apps_1_percentage;
			  p_pedals_state_data->apps_2_percentage = apps_2_percentage;
			  p_pedals_state_data->accel_pedal_percentage = accel_pedal_percentage;
			  p_pedals_state_data->brake_pedal_percentage = brake_pedal_percentage;

		  osMutexRelease(PedalsState_MutexHandle);


		  osMutexAcquire(PedalsFaults_MutexHandle, osWaitForever);
		  	  Set_New_Pedals_Faults_Data(&current_pedals_faults);
		  osMutexRelease(PedalsFaults_MutexHandle);
	  }
	  else
	  {
		  osEventFlagsWait(
			  CalibrationDataReceived_EventHandle,
			  CALIBRATION_DATA_READY,
		      osFlagsWaitAny | osFlagsNoClear,
		      osWaitForever
		  );

		  if (osMessageQueueGet(CalibrationDataQueueHandle, &calibration_data, NULL, osWaitForever) != osOK)
		  {
			  p_queue_errors_data->calibration_data_errors++;
		  }

		  apps_1_min = calibration_data.apps_1_min_value;
		  apps_1_max = calibration_data.apps_1_max_value;
		  apps_2_min = calibration_data.apps_2_min_value;
		  apps_2_max = calibration_data.apps_2_max_value;
		  bse_min = calibration_data.bse_min_value;
		  bse_max = calibration_data.bse_max_value;

	        osEventFlagsSet(
				CalibrationDataReceived_EventHandle,
				CALIBRATION_DATA_RECEIVED
	        );
	  }

    osDelay(1);
  }
}

void APPS_1_OutOfRange_Timer_Callback(void *argument)
{
    osEventFlagsSet(
		PedalsOutOfRangeFault_EventHandle,
		APPS_1_OUT_OF_RANGE
    );
}

void APPS_2_OutOfRange_Timer_Callback(void *argument)
{
    osEventFlagsSet(
		PedalsOutOfRangeFault_EventHandle,
		APPS_2_OUT_OF_RANGE
    );
}

void BSE_OutOfRange_Timer_Callback(void *argument)
{
    osEventFlagsSet(
		PedalsOutOfRangeFault_EventHandle,
		BSE_OUT_OF_RANGE
    );
}

void APPS_Implausibility_Timer_Callback(void *argument)
{
    osEventFlagsSet(
		APPS_Implausibility_EventHandle,
		APPS_IMPLAUSIBILITY
    );
}

void Screenshot_Timer_Callback(void *argument)
{
    osEventFlagsSet(
		Screenshot_EventHandle,
		SCREENSHOT
    );
}

uint8_t calc_adc_percentage(uint16_t val, uint16_t min, uint16_t max)
{
	if (val <= min) return 0;

	if (val >= max) return 100;

	return (uint8_t)(
			((uint32_t)(val - min) * 100U) /
				(max - min)
			);
}


// Check if APPS / BSE sensor values are outside expected working range
PedalStatus_t pedal_position_sensor_out_of_range_fault_check(uint16_t pedal_position_sensor_raw_value,
															 uint16_t pedal_position_sensor_open_circuit_threshold,
															 uint16_t pedal_position_sensor_short_circuit_threshold)
{
	int pedal_position_sensor_out_of_range_fault_status = NORMAL;

	// Check for a short to GND
	if (pedal_position_sensor_raw_value <= pedal_position_sensor_short_circuit_threshold)
	{
		pedal_position_sensor_out_of_range_fault_status = SHORT_CIRCUIT;
	}

	// Check for a short to Vcc
	else if(pedal_position_sensor_raw_value >= pedal_position_sensor_open_circuit_threshold)
	{
		pedal_position_sensor_out_of_range_fault_status = OPEN_CIRCUIT;
	}

	return pedal_position_sensor_out_of_range_fault_status;
}


bool APPS_Implausibility_Check(uint8_t APPS_1_percentage, uint8_t APPS_2_percentage)
{
	bool APPS_implausibility_fault = false;

	// Define fault threshold at 10% as per Rules
	uint8_t APPS_pedal_travel_deviation_fault_threshold = 10;

	// Calculate pedal travel deviation
	int APPS_pedal_travel_deviation = abs(APPS_1_percentage - APPS_2_percentage);
	if(APPS_pedal_travel_deviation > APPS_pedal_travel_deviation_fault_threshold)
	{
		APPS_implausibility_fault = true;
	}

	return APPS_implausibility_fault;
}


// Check for brake implausbility
// Occurs if the brakes are engaged and more than 25% accelerator pedal travel is detected simultaneously
bool Screenshot_Check(uint8_t APPS_1_percentage, uint8_t APPS_2_percentage,
					  uint8_t brake_pedal_position_percentage,
					  uint8_t brakes_engaged_threshold_percentage,
					  bool current_screenshot_status)
{
	// Set APPS pedal travel fault threshold to 25%
	uint8_t APPS_pedal_travel_fault_threshold_percentage = 25;

	// Set APPS pedal travel fault reset threshold to 5%
	uint8_t APPS_pedal_travel_fault_reset_threshold_percentage = 5;

	if((current_screenshot_status == false)
		&& (brake_pedal_position_percentage > brakes_engaged_threshold_percentage)
		&& (APPS_1_percentage > APPS_pedal_travel_fault_threshold_percentage)
		&& (APPS_2_percentage > APPS_pedal_travel_fault_threshold_percentage))
	{
		current_screenshot_status = true;
	}

	else if((current_screenshot_status == true)
			  && (APPS_1_percentage <= APPS_pedal_travel_fault_reset_threshold_percentage)
			  && (APPS_2_percentage <= APPS_pedal_travel_fault_reset_threshold_percentage))
	{
		current_screenshot_status = false;
	}

	return current_screenshot_status;
}


void Get_Previous_Pedals_Faults_Data(PedalsFaults_t* p_previous_faults)
{
	p_previous_faults->apps_1_status 		= p_pedals_faults_data->apps_1_status;
	p_previous_faults->apps_2_status		= p_pedals_faults_data->apps_2_status;
	p_previous_faults->bse_status 			= p_pedals_faults_data->bse_status;
	p_previous_faults->apps_implausibility 	= p_pedals_faults_data->apps_implausibility;
	p_previous_faults->screenshot 			= p_pedals_faults_data->screenshot;
}

void Set_New_Pedals_Faults_Data(PedalsFaults_t* p_new_faults)
{
    p_pedals_faults_data->apps_1_status        = p_new_faults->apps_1_status;
    p_pedals_faults_data->apps_2_status        = p_new_faults->apps_2_status;
    p_pedals_faults_data->bse_status           = p_new_faults->bse_status;
    p_pedals_faults_data->apps_implausibility  = p_new_faults->apps_implausibility;
    p_pedals_faults_data->screenshot           = p_new_faults->screenshot;
}


