/*
 * vehicle_control.h
 *
 *  Created on: 18 Jul 2026
 *      Author: mehta
 */

#ifndef INC_APP_VEHICLE_CONTROL_H_
#define INC_APP_VEHICLE_CONTROL_H_

#include "config/vehicle_data_structs.h"
#include "config/amk_data_structs.h"
#include "app/state_control.h"

extern PedalsState_t* p_pedals_state_data;
extern InverterSetpoints_t* p_inverter_setpoints_1;
extern SystemState_t system_state;
extern VehicleState_t* p_vehicle_state_data;

void Task_Vehicle_Ctrl(void *argument);
void InverterCAN_Transmit_Callback(void *argument);

bool BrakesEngagedCheck(uint8_t brake_pedal_position_percentage, uint8_t brakes_engaged_threshold_percentage);

#endif /* INC_APP_VEHICLE_CONTROL_H_ */
