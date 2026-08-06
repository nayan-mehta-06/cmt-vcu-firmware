/*
 * log_vehicle_data.h
 *
 *  Created on: 6 Aug 2026
 *      Author: mehta
 */

#ifndef INC_APP_LOG_VEHICLE_DATA_H_
#define INC_APP_LOG_VEHICLE_DATA_H_

#include <config/vehicle_data_structs.h>

typedef struct ConvertedADCs
{

	float brake_pressure_front_bar;
	float brake_pressure_rear_bar;
	int BSPD_current_sensor_current;
	float steering_angle;
	uint16_t spare_adc;

} ConvertedADCs_t;

#define BPPS_MIN_VOLTAGE 0.5
#define BPPS_MAX_VOLTAGE 4.5

#define PSI_TO_BAR_CONVERSION_FACTOR 0.0689476

#define ADC_SUPPLY_VOLTAGE 3.3
#define BPPS_ADC_VOLTAGE_DIVIDER_CONVERSION_RATIO 0.6

void Task_LogVehicleData(void *argument);

float calcBrakePressure(uint16_t bpps_adc);
float calcBspdCurrentSensorCurrent(uint16_t bspd_cs_adc);

#endif /* INC_APP_LOG_VEHICLE_DATA_H_ */
