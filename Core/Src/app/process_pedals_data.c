/*
 * process_pedals_data.c
 *
 *  Created on: 17 Jul 2026
 *      Author: mehta
 */


#include <app/process_pedals_data.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/app_freertos.h"
#include "config/vehicle_data_structs.h"
#include "app/process_adc.h"
#include "config/vcu_config.h"

void Task_Process_Pedals_Data(void *argument)
{
	PedalsADC_t pedals_adc_data;

	uint16_t apps1_adc;
	uint16_t app2_adc;
	uint16_t bse_adc;

	uint8_t accel_pedal_percentage;
	uint8_t brake_pedal_percentage;

  /* Infinite loop */
  for(;;)
  {
	  if (osMessageQueueGet(PedalsADCQueueHandle, &pedals_adc_data, NULL, osWaitForever) != osOK)
	  {
		  p_queue_errors_data->pedals_adc_errors++;
	  }

	  apps1_adc = pedals_adc_data.apps1_adc;

	  accel_pedal_percentage = calc_adc_percentage(apps1_adc);


	  osMutexAcquire(PedalsState_MutexHandle, osWaitForever);

	  	  p_pedals_state_data->accel_pedal_percentage = accel_pedal_percentage;

	  osMutexRelease(PedalsState_MutexHandle);

    osDelay(1);
  }
}

uint8_t calc_adc_percentage(uint16_t val)
{
	if (val <= MIN_ADC_VAL) return 0;

	if (val >= MAX_ADC_VAL) return 100;


	return (uint8_t)(
			((uint32_t)(val - MIN_ADC_VAL) * 100U) /
				(MAX_ADC_VAL - MIN_ADC_VAL)
			);
}


