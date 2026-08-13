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
#include "app/state_control.h"
#include "drivers/can_driver.h"
#include "config/can_ids.h"

void Task_LogVehicleData(void *argument)
{
	ADCsToLog_t adcs_to_log;
	ConvertedADCs_t converted_adcs_to_log;

	InverterDataToLog_t inv_1_data_to_log;
	InverterDataToLog_t inv_2_data_to_log;

	uint8_t pedals_faults_flags_to_log;
	uint8_t apps_implausibility_flags_to_log;
	uint8_t screenshot_flag_to_log;

	PedalsState_t pedals_state_to_log;

	uint8_t vehicle_state_to_log;

	uint8_t pc_complete_to_log;

	/* Start the periodic timers to start the CAN2 transmissions  */
	osTimerStart(CAN_2_Transmit_Timer_1Handle, CAN_2_TRANSMIT_RATE);
	osTimerStart(CAN_2_Transmit_Timer_2Handle, CAN_2_TRANSMIT_RATE);

	/* Infinite Loop */
	for(;;)
	{

		/* Receive the sensor values that should be logged from the process adc task */
		  if (osMessageQueueGet(ADCsToLogQueueHandle, &adcs_to_log, NULL, osWaitForever) != osOK)
		  {
			  //p_queue_errors_data->logging_adc_errors++;
		  }

		  Convert_ADCs(&converted_adcs_to_log, adcs_to_log);

		  osMutexAcquire(InverterData1_MutexHandle, osWaitForever);
			  Get_Inverter_Data_To_Log(&inv_1_data_to_log,
					  	  	  	  	   p_inverter_data_1,
									   p_inverter_setpoints_1);
		  osMutexRelease(InverterData1_MutexHandle);

		  osMutexAcquire(InverterData2_MutexHandle, osWaitForever);
			  Get_Inverter_Data_To_Log(&inv_2_data_to_log,
					  	  	  	  	   p_inverter_data_2,
									   p_inverter_setpoints_2);
		  osMutexRelease(InverterData2_MutexHandle);

		  /* The event flags can be directly logged as all event bits fit into the 8 bits of the flag
		   * so typecasting it to an 8bit unsigned integer wont lose any information  */
		pedals_faults_flags_to_log = (uint8_t)(osEventFlagsGet(PedalsOutOfRangeFault_EventHandle) & 0xFF);
		apps_implausibility_flags_to_log = (uint8_t)(osEventFlagsGet(APPS_Implausibility_EventHandle) & 0xFF);
		screenshot_flag_to_log = (uint8_t)(osEventFlagsGet(Screenshot_EventHandle) & 0xFF);

		/* Make copies of the global structs to log to prevent accessing them for very long */
		osMutexAcquire(PedalsState_MutexHandle, osWaitForever);
			pedals_state_to_log = *p_pedals_state_data;
		osMutexRelease(PedalsState_MutexHandle);

		osMutexAcquire(SystemState_MutexHandle, osWaitForever);
			vehicle_state_to_log = (uint8_t)system_state;
		osMutexRelease(SystemState_MutexHandle);

		osMutexAcquire(VehicleState_MutexHandle, osWaitForever);
			pc_complete_to_log = p_vehicle_state_data->precharge_complete;
		osMutexRelease(VehicleState_MutexHandle);

		/* Save all logging values to global struct so that only one mutex is required by the timer callbacks */
		osMutexAcquire(DataToLog_MutexHandle, osWaitForever);
			p_data_to_log->converted_adcs             = converted_adcs_to_log;
			p_data_to_log->inv_1_data                 = inv_1_data_to_log;
			p_data_to_log->inv_2_data                 = inv_2_data_to_log;
			p_data_to_log->pedals_faults_flags        = pedals_faults_flags_to_log;
			p_data_to_log->apps_implausibility_flags  = apps_implausibility_flags_to_log;
			p_data_to_log->screenshot_flag            = screenshot_flag_to_log;
			p_data_to_log->pedals_state               = pedals_state_to_log;
			p_data_to_log->vehicle_state              = vehicle_state_to_log;
			p_data_to_log->pc_complete                = pc_complete_to_log;
		osMutexRelease(DataToLog_MutexHandle);

		osDelay(1);
	}
}


/* Log non-inverter values over can over 2 messages */
void CAN_2_Transmit_Timer_1_Callback(void *argument)
{
	DataToLog_t all_data_to_log;
	uint8_t can_data[8] = {0};

	osMutexAcquire(DataToLog_MutexHandle, osWaitForever);
		all_data_to_log = *p_data_to_log;
	osMutexRelease(DataToLog_MutexHandle);

	/* 16 bit integers are logged over 2 bytes in big endian format */
	can_data[0] = (all_data_to_log.converted_adcs.BSPD_current_sensor_current >> 8) & 0xFF;
	can_data[1] = (all_data_to_log.converted_adcs.BSPD_current_sensor_current & 0xFF);

	can_data[2] = (all_data_to_log.converted_adcs.brake_pressure_front_bar >> 8) & 0xFF;
	can_data[3] = (all_data_to_log.converted_adcs.brake_pressure_front_bar & 0xFF);

	can_data[4] = (all_data_to_log.converted_adcs.brake_pressure_rear_bar >> 8) & 0xFF;
	can_data[5] = (all_data_to_log.converted_adcs.brake_pressure_rear_bar & 0xFF);

	can_data[6] = (all_data_to_log.converted_adcs.steering_angle >> 8) & 0xFF;
	can_data[7] = (all_data_to_log.converted_adcs.steering_angle & 0xFF);

	CAN_transmit(can_data, EIGHT_BYTES, CAN_2_VCU_DATA_1, CAN_2, CAN_STD_ID_FORMAT);

	can_data[0] = all_data_to_log.pedals_state.apps_1_percentage;
	can_data[1] = all_data_to_log.pedals_state.apps_2_percentage;
	can_data[2] = all_data_to_log.pedals_state.accel_pedal_percentage;
	can_data[3] = all_data_to_log.pedals_state.brake_pedal_percentage;
	can_data[4] = all_data_to_log.pedals_faults_flags;
	can_data[5] = all_data_to_log.apps_implausibility_flags;
	can_data[6] = all_data_to_log.screenshot_flag;
	can_data[7] = all_data_to_log.pc_complete;

	CAN_transmit(can_data, EIGHT_BYTES, CAN_2_VCU_DATA_2, CAN_2, CAN_STD_ID_FORMAT);

	can_data[0] = all_data_to_log.vehicle_state;

	can_data[1] = (all_data_to_log.converted_adcs.spare_adc >> 8) & 0xFF;
	can_data[2] = (all_data_to_log.converted_adcs.spare_adc & 0xFF);

	CAN_transmit(can_data, THREE_BYTES, CAN_2_VCU_DATA_3, CAN_2, CAN_STD_ID_FORMAT);

}

/* log inverter details over can over 2 messages */
void CAN_2_Transmit_Timer_2_Callback(void *argument)
{
	DataToLog_t all_data_to_log;
	uint8_t can_data[8] = {0};
	InverterDataToLog_t* inv_datas[2];
	int can_id_1 = CAN_2_INV_DATA_1;
	int can_id_2 = CAN_2_INV_DATA_2;
	int can_ids_offsets[2] = {INVERTER_1_NODE_ADDRESS, INVERTER_2_NODE_ADDRESS};
	int can_id;

	osMutexAcquire(DataToLog_MutexHandle, osWaitForever);
		all_data_to_log = *p_data_to_log;
	osMutexRelease(DataToLog_MutexHandle);

	/* Save each inverter struct into an array so they can be looped over instead of
	 * duplicating the code */
	inv_datas[0] = &all_data_to_log.inv_1_data;
	inv_datas[1] = &all_data_to_log.inv_2_data;

	for (uint8_t i=0; i<2; i++)
	{
		can_id = can_id_1 + can_ids_offsets[i];

		can_data[0] = (inv_datas[i]->speed_val_rpm >> 8) & 0xFF;
		can_data[1] = (inv_datas[i]->speed_val_rpm & 0xFF);

		can_data[2] = (inv_datas[i]->torque_current_A >> 8) & 0xFF;
		can_data[3] = (inv_datas[i]->torque_current_A & 0xFF);

		can_data[4] = (inv_datas[i]->magnetizing_current_A >> 8) & 0xFF;
		can_data[5] = (inv_datas[i]->magnetizing_current_A & 0xFF);

		can_data[6] = (inv_datas[i]->torque_setpoint_percent >> 8) & 0xFF;
		can_data[7] = (inv_datas[i]->torque_setpoint_percent & 0xFF);

		CAN_transmit(can_data, EIGHT_BYTES, can_id, CAN_2, CAN_STD_ID_FORMAT);

		can_id = can_id_2 + can_ids_offsets[i];

		can_data[0] = (inv_datas[i]->temp_motor >> 8) & 0xFF;
		can_data[1] = (inv_datas[i]->temp_motor & 0xFF);

		can_data[2] = (inv_datas[i]->temp_inverter >> 8) & 0xFF;
		can_data[3] = (inv_datas[i]->temp_inverter & 0xFF);

		can_data[4] = (inv_datas[i]->temp_IGBT >> 8) & 0xFF;
		can_data[5] = (inv_datas[i]->temp_IGBT & 0xFF);

		can_data[6] = (inv_datas[i]->diagnostic_number >> 8) & 0xFF;
		can_data[7] = (inv_datas[i]->diagnostic_number & 0xFF);

		CAN_transmit(can_data, EIGHT_BYTES, can_id, CAN_2, CAN_STD_ID_FORMAT);
	}
}


/* Taken from 2025 Firmware, modified to make it more consistent with 26 firmware */
float Calc_Brake_Pressure(uint16_t bpps_adc)
{

	float v_sensor = Calc_Sensor_Input_Voltage(bpps_adc);
	if (v_sensor < BPPS_MIN_VOLTAGE) v_sensor = BPPS_MIN_VOLTAGE;
	if (v_sensor > BPPS_MAX_VOLTAGE) v_sensor = BPPS_MAX_VOLTAGE;

	// Equation from BPPS datasheet
	float brake_pressure_front_PSI = ((v_sensor * 500) - 250);

	return (brake_pressure_front_PSI * PSI_TO_BAR_CONVERSION_FACTOR);
}

/* Adapted from the BSPD sensor datasheet */
// https://www.lem.com/sites/default/files/products_datasheets/htfs-200__800-p-v13.pdf
float Calc_BSPD_Sensor_Current(uint16_t bspd_cs_adc)
{
	float v_ref = 1.5f;
	uint16_t nominal_current = 200; // We use the HTFS-200, therefore nominal current is 200A
	float multiplier = 1.25f;

	float v_sensor = Calc_Sensor_Input_Voltage(bspd_cs_adc);

	float current = (v_sensor - v_ref) * (nominal_current / multiplier);

	return current;
}

/* Adapted from the Steering angle sensor datasheet */
// 981HE0B4WA https://www.vishay.com/docs/57103/model981he.pdf
float Calc_Steering_Angle(uint16_t sas_cs_adc) {

	uint8_t sensor_supply_v = 5;
	float sensor_min_percentage = 0.1f;
	float sensor_max_percentage = 0.9f;
	uint16_t sensor_electrical_angle = 360;

	float v_sensor = Calc_Sensor_Input_Voltage(sas_cs_adc);

	float v_percentage = v_sensor / sensor_supply_v;
	float angle = (((v_percentage - sensor_min_percentage)
					 /(sensor_max_percentage - sensor_min_percentage))
					 * sensor_electrical_angle);

	if (angle < 0.0f) angle = 0.0f;
	if (angle > 360.0f) angle = 360.0f;

	return angle;
}

/*
 * Convert the adc value back into the voltage coming into VCU as all the sensor
 * equations use their voltage
 */
float Calc_Sensor_Input_Voltage(uint16_t adc_val)
{
	return ((float)adc_val / ADC_MAX_READING) * ADC_SUPPLY_VOLTAGE * V_DIVIDER_GAIN;
}


/*
 * Scale calculated sensor values to fit them into 16 bits that can be sent over CAN
 * as floats take up 4 bytes and are not efficient to send
 */
void Convert_ADCs(ConvertedADCs_t* converted_adcs, ADCsToLog_t raw_adcs)
{
	converted_adcs->brake_pressure_front_bar 		= (int16_t)(Calc_Brake_Pressure(raw_adcs.bpps_1_adc) * 10.0f);
	converted_adcs->brake_pressure_rear_bar 		= (int16_t)(Calc_Brake_Pressure(raw_adcs.bpps_2_adc) * 10.0f);
	converted_adcs->BSPD_current_sensor_current		= (int16_t)(Calc_BSPD_Sensor_Current(raw_adcs.bspd_cs_adc) * 10.0f);
	converted_adcs->steering_angle 					= (int16_t)(Calc_Steering_Angle(raw_adcs.sas_adc) * 10.0f);
    converted_adcs->spare_adc 						= raw_adcs.spare_adc;
}

void Get_Inverter_Data_To_Log(InverterDataToLog_t* inv_data_to_log,
							  InverterData_t* inv_data,
							  InverterSetpoints_t* inv_setpoints)
{
	inv_data_to_log->speed_val_rpm 				= inv_data->actual_speed_value;
	inv_data_to_log->torque_current_A 			= (int16_t)(inv_data->torque_current * 100.0f);
	inv_data_to_log->magnetizing_current_A		= (int16_t)(inv_data->magnetizing_current * 100.0f);
	inv_data_to_log->temp_motor					= inv_data->temp_motor;
	inv_data_to_log->temp_inverter 				= inv_data->temp_inverter;
	inv_data_to_log->temp_IGBT 					= inv_data->temp_IGBT;
	inv_data_to_log->torque_setpoint_percent 	= inv_setpoints->torque_setpoint;
	inv_data_to_log->diagnostic_number 			= inv_data->diagnostic_number;
}

