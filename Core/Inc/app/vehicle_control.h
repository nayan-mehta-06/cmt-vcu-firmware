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

void Task_Vehicle_Ctrl(void *argument);
void InverterCAN_TransmitCallback(void *argument);

#endif /* INC_APP_VEHICLE_CONTROL_H_ */
