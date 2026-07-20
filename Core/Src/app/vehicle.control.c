/*
 * vehicle.control.c
 *
 *  Created on: 18 Jul 2026
 *      Author: mehta
 */


#include <app/vehicle_control.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/app_freertos.h"
#include "config/vehicle_data_structs.h"
#include "app/state_control.h"
#include "config/can_ids.h"
#include "config/vcu_config.h"
#include "app/inverter_control.h"


void Task_Vehicle_Ctrl(void *argument)
{
	float accel_pedal_percentage;
	int16_t torque_setpoint;
	SystemState_t current_state;

  /* Infinite loop */
  for(;;)
  {
	  osMutexAcquire(SystemState_MutexHandle, osWaitForever);
	  	  current_state = system_state;
	  osMutexRelease(SystemState_MutexHandle);

	  osMutexAcquire(PedalsState_MutexHandle, osWaitForever);
	  	  accel_pedal_percentage = (p_pedals_state_data->accel_pedal_percentage) / 100.0f;
	  osMutexRelease(PedalsState_MutexHandle);

	  torque_setpoint = (int16_t)(accel_pedal_percentage * 1000.0f);

	  osMutexAcquire(InverterData1_MutexHandle, osWaitForever);
		  if (current_state == STATE_RTD)
		  {
			  p_inverter_setpoints_1->torque_setpoint = torque_setpoint;
			  p_inverter_setpoints_1->torque_limit_positive = TORQUE_LIM_POS;
			  p_inverter_setpoints_1->torque_limit_negative = TORQUE_LIM_NEG;
		  }
		  else
		  {
			  p_inverter_setpoints_1->torque_setpoint = 0;
			  p_inverter_setpoints_1->torque_limit_positive = 0;
			  p_inverter_setpoints_1->torque_limit_negative = 0;
		  }
	  osMutexRelease(InverterData1_MutexHandle);



    osDelay(1);
  }
}

void InverterCAN_TransmitCallback(void *argument)
{
	osMutexAcquire(InverterData1_MutexHandle, osWaitForever);

		transmit_inverter_command(p_inverter_setpoints_1, INVERTER_1_NODE_ADDRESS);

	osMutexRelease(InverterData1_MutexHandle);
}
