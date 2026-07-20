/*
 * RTD.h
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#ifndef INC_APP_RTD_H_
#define INC_APP_RTD_H_

#include <config/vehicle_data_structs.h>
#include <app/state_control.h>


void Task_RTD(void *argument);
void RTD_button_timer_callback(void *argument);

extern QueueErrors_t* p_queue_errors;
extern SystemState_t system_state;


#endif /* INC_APP_RTD_H_ */
