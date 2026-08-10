/*
 * amk_data_structs.h
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#ifndef INC_CONFIG_AMK_DATA_STRUCTS_H_
#define INC_CONFIG_AMK_DATA_STRUCTS_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct InverterData
{

	uint16_t status;
	int16_t actual_speed_value;
	int16_t torque_current_raw;
	float torque_current;
	int16_t magnetizing_current_raw;
	float magnetizing_current;
	int16_t temp_motor;
	int16_t temp_inverter;
	uint16_t diagnostic_number;
	int16_t temp_IGBT;

} InverterData_t;

typedef struct InverterStatus
{

	bool system_ready;
	bool error;
	bool warn;
	bool quit_dc_on;
	bool dc_on;
	bool quit_inverter_on;
	bool inverter_on;
	bool derating;

} InverterStatus_t;

typedef struct InverterSetpoints
{

	uint16_t control;
	int16_t torque_setpoint;
	int16_t torque_limit_positive;
	int16_t torque_limit_negative;

} InverterSetpoints_t;


void initialise_inverter_data(InverterData_t* p_inverter_data);
extern InverterData_t* p_inverter_data_1;
extern InverterData_t* p_inverter_data_2;

void initialise_inverter_status(InverterStatus_t* p_inverter_status);
extern InverterStatus_t* p_inverter_status_1;
extern InverterStatus_t* p_inverter_status_2;

void initialise_inverter_setpoints(InverterSetpoints_t* p_inverter_setpoints);
extern InverterSetpoints_t* p_inverter_setpoints_1;
extern InverterSetpoints_t* p_inverter_setpoints_2;

#endif /* INC_CONFIG_AMK_DATA_STRUCTS_H_ */
