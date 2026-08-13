/*
 * state_control.h
 *
 *  Created on: 17 May 2026
 *      Author: mehta
 */

#ifndef INC_APP_STATE_CONTROL_H_
#define INC_APP_STATE_CONTROL_H_

#include <config/vehicle_data_structs.h>

// All VCU States with defined integer values to be logged to CAN
typedef enum
{
    STATE_IDLE				= 0,
	STATE_CALIBRATION		= 4,
	STATE_RTD				= 1,
	STATE_SCREENSHOTED		= 3,
	STATE_FAULTED			= 2
} SystemState_t;

// All VCU State Transitions
typedef enum
{
	EVENT_NONE,
	EVENT_ENTER_CALIBRATION,
	EVENT_CALIBRATION_COMPLETE,
	EVENT_ENTER_RTD,
	EVENT_EXIT_RTD,
	EVENT_SCREENSHOT,
	EVENT_SCREENSHOT_OVER,
	EVENT_FAULT,
	EVENT_CLEAR_FAULT
} SystemEvent_t;


extern SystemState_t system_state;
extern QueueErrors_t* p_queue_errors;

void Task_State_Ctrl(void *argument);

SystemState_t Get_Current_State(void);

#endif /* INC_APP_STATE_CONTROL_H_ */
