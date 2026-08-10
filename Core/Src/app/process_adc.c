/*
 * process_adc.c
 *
 *  Created on: 15 Jul 2026
 *      Author: mehta
 */

#include <app/process_adc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/app_freertos.h"
#include "config/vehicle_data_structs.h"
#include "config/vcu_config.h"
#include "drivers/adc_driver.h"


void Task_Process_ADC(void *argument)
{
	/* USER CODE BEGIN start_task */
	uint16_t apps_1_adc, apps_2_adc, bse_adc;
	uint16_t bpps_1_adc, bpps_2_adc, bspd_cs_adc, sas_adc, spare_adc;

	PedalsADC_t pedals_adc_data;
	ADCsToLog_t adcs_to_log;

	/* Infinite loop */
	for(;;)
	{
		apps_1_adc = Get_ADC_Channel(APPS1_ADC_CHANNEL);
		apps_2_adc = Get_ADC_Channel(APPS2_ADC_CHANNEL);
		bse_adc = Get_ADC_Channel(BSE_ADC_CHANNEL);

		bpps_1_adc = Get_ADC_Channel(BPPS1_ADC_CHANNEL);
		bpps_2_adc = Get_ADC_Channel(BPPS2_ADC_CHANNEL);
		bspd_cs_adc = Get_ADC_Channel(BSPD_CS_ADC_CHANNEL);
		sas_adc = Get_ADC_Channel(SAS_ADC_CHANNEL);
		spare_adc = Get_ADC_Channel(SPARE_ADC_CHANNEL);

		//Clamp_ADC(&apps_1_adc);

		pedals_adc_data.apps_1_adc = apps_1_adc;
		pedals_adc_data.apps_2_adc = apps_2_adc;
		pedals_adc_data.bse_adc = bse_adc;

		adcs_to_log.bpps_1_adc = bpps_1_adc;
		adcs_to_log.bpps_2_adc = bpps_2_adc;
		adcs_to_log.bspd_cs_adc = bspd_cs_adc;
		adcs_to_log.sas_adc = sas_adc;
		adcs_to_log.spare_adc = spare_adc;

		if (osMessageQueuePut(PedalsADCQueueHandle, &pedals_adc_data, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
		{
			p_queue_errors_data->pedals_adc_errors++;
		}

		if (osMessageQueuePut(ADCsToLogQueueHandle, &adcs_to_log, QUEUE_MESSAGE_PRIORITY, ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
		{
			p_queue_errors_data->logging_adc_errors++;
		}

		osDelay(1);
	}
	/* USER CODE END start_task */
}

void Clamp_ADC(uint16_t* adc_val)
{
	if (*adc_val < MIN_ADC_VAL)
	{
		*adc_val = MIN_ADC_VAL;
	}
	if (*adc_val > MAX_ADC_VAL)
	{
		*adc_val = MAX_ADC_VAL;
	}
}
