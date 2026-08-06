/*
 * log_vehicle_data.c
 *
 *  Created on: 6 Aug 2026
 *      Author: mehta
 */


#include <app/log_vehicle_data.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/process_adc.h"
#include "config/amk_data_structs.h"
#include "config/vehicle_data_structs.h"
#include "config/vcu_config.h"
#include "cmt_utils.h"
#include "app/app_freertos.h"

void Task_LogVehicleData(void *argument)
{
	ADCsToLog_t adcs_to_log;
	ConvertedADCs_t converted_adcs;
	uint16_t bpps_1_adc, bpps_2_adc, bspd_cs_adc, sas_adc, spare_adc;

	/* Infinite Loop */
	for(;;)
	{
		  if (osMessageQueueGet(PedalsADCQueueHandle, &adcs_to_log, NULL, osWaitForever) != osOK)
		  {
			  p_queue_errors_data->logging_adc_errors++;
		  }

		  bpps_1_adc = adcs_to_log.bpps_1_adc;
		  bpps_2_adc = adcs_to_log.bpps_2_adc;
		  bspd_cs_adc = adcs_to_log.bspd_cs_adc;
		  sas_adc = adcs_to_log.sas_adc;
		  spare_adc = adcs_to_log.spare_adc;

		  converted_adcs.brake_pressure_front_bar = calcBrakePressure(bpps_1_adc);
		  converted_adcs.brake_pressure_rear_bar = calcBrakePressure(bpps_2_adc);
		  converted_adcs.BSPD_current_sensor_current = calcBspdCurrentSensorCurrent(bspd_cs_adc);
		  converted_adcs.steering_angle = sas_adc;
		  converted_adcs.spare_adc = spare_adc;


		osDelay(1);
	}
}

float calcBrakePressure(uint16_t bpps_adc)
{
	float BPPS_voltage_output = ((bpps_adc / 4096) * ADC_SUPPLY_VOLTAGE * (2.5/1.5));

	if (BPPS_voltage_output < BPPS_MIN_VOLTAGE)
	{
		BPPS_voltage_output = BPPS_MIN_VOLTAGE;
	}

	else if (BPPS_voltage_output > BPPS_MAX_VOLTAGE)
	{
		BPPS_voltage_output = BPPS_MAX_VOLTAGE;
	}

	// Equation from BPPS datasheet
	float brake_pressure_front_PSI = ((BPPS_voltage_output * 500) - 250);

	return (brake_pressure_front_PSI * PSI_TO_BAR_CONVERSION_FACTOR);
}

float calcBspdCurrentSensorCurrent(uint16_t bspd_cs_adc)
{
	/*
	p_vehicle_state_data->BSPD_current_sensor_current = (((int) ( 160.0 * ( (BSPD_current_sensor_voltage_output)))) - 5);

	// Zero the current
	if(p_vehicle_state_data->BSPD_current_sensor_current == -5)
	{

		p_vehicle_state_data->BSPD_current_sensor_current = 0;

	}
	*/

	return 67;
}
