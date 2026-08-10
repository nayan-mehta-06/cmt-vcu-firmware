/*
 * log_vehicle_data.h
 *
 *  Created on: 6 Aug 2026
 *      Author: mehta
 */

#ifndef INC_APP_LOG_VEHICLE_DATA_H_
#define INC_APP_LOG_VEHICLE_DATA_H_

#include <config/vehicle_data_structs.h>
#include "config/amk_data_structs.h"
#include "app/process_adc.h"

typedef struct ConvertedADCs
{

	int16_t brake_pressure_front_bar;
	int16_t brake_pressure_rear_bar;
	int16_t BSPD_current_sensor_current;
	int16_t steering_angle;
	uint16_t spare_adc;

} ConvertedADCs_t;

typedef struct InverterDataToLog
{

	int16_t speed_val_rpm;
	int16_t torque_current_A;
	int16_t magnetizing_current_A;
	int16_t temp_motor;
	int16_t temp_inverter;
	int16_t temp_IGBT;
	int16_t torque_setpoint_percent;
	uint16_t diagnostic_number;

} InverterDataToLog_t;

typedef struct DataToLog{
    ConvertedADCs_t converted_adcs;
    InverterDataToLog_t inv_1_data;
    InverterDataToLog_t inv_2_data;
    uint8_t pedals_faults_flags;
    uint8_t apps_implausibility_flags;
    uint8_t screenshot_flag;
    PedalsState_t pedals_state;
    uint8_t vehicle_state;
    uint8_t pc_complete;
} DataToLog_t;

extern DataToLog_t* p_data_to_log;

#define BPPS_MIN_VOLTAGE 0.5
#define BPPS_MAX_VOLTAGE 4.5
#define BPPS_ADC_VOLTAGE_DIVIDER_CONVERSION_RATIO 0.6
#define PSI_TO_BAR_CONVERSION_FACTOR 0.0689476

#define ADC_SUPPLY_VOLTAGE 3.3f
#define ADC_MAX_READING 4095.0f
#define V_DIVIDER_GAIN 1.5f

void Task_LogVehicleData(void *argument);

float Calc_Brake_Pressure(uint16_t bpps_adc);
float Calc_BSPD_Sensor_Current(uint16_t bspd_cs_adc);
float Calc_Steering_Angle(uint16_t sas_cs_adc);
void Convert_ADCs(ConvertedADCs_t* converted_adcs, ADCsToLog_t raw_adcs);

void Get_Inverter_Data_To_Log(InverterDataToLog_t* inv_data_to_log,
							  InverterData_t* inv_data,
							  InverterSetpoints_t* inv_setpoints);


#endif /* INC_APP_LOG_VEHICLE_DATA_H_ */
