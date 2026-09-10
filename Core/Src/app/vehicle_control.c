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
#include "config/amk_data_structs.h"
#include "app/state_control.h"
#include "config/can_ids.h"
#include "config/vcu_config.h"
#include "app/inverter_control.h"

// Shared between Task_Vehicle_Ctrl and the error reset timer callbacks
//static uint8_t inv_1_error_reset_attempts = 0;
//static uint8_t inv_2_error_reset_attempts = 0;
static volatile bool inv_1_error_reset_ready = true;
static volatile bool inv_2_error_reset_ready = true;

void Task_Vehicle_Ctrl(void *argument)
{
	uint8_t accel_pedal_percentage;
	uint8_t brakes_pedal_percentage;
	int16_t torque_setpoint;
	SystemState_t current_state;

	bool brakes_engaged;

	// Start the periodic timer to transmit messages to the inverters over CAN1
	osTimerStart(InverterCAN_Transmit_TimerHandle, INVERTER_TORQUE_COMMAND_CAN_TRANSMIT_RATE);

  /* Infinite loop */
  for(;;)
  {
	  current_state = Get_Current_State();

	  osMutexAcquire(PedalsState_MutexHandle, osWaitForever);
	  	  accel_pedal_percentage = p_pedals_state_data->accel_pedal_percentage;
	  	  brakes_pedal_percentage = p_pedals_state_data->brake_pedal_percentage;
	  osMutexRelease(PedalsState_MutexHandle);


	  brakes_engaged = BrakesEngagedCheck(brakes_pedal_percentage,
			  	  	  	  	  	  	  	  BRAKES_ENGAGED_THRESHOLD_PERCENTAGE);

	  osMutexAcquire(VehicleState_MutexHandle, osWaitForever);

	  if (brakes_engaged)
	  {
		  p_vehicle_state_data->brakes_engaged = true;
		  BRAKE_LIGHT_ON();
	  }
	  else
	  {
		  p_vehicle_state_data->brakes_engaged = false;
		  BRAKE_LIGHT_OFF();

	  }

	  osMutexRelease(VehicleState_MutexHandle);

	  // Inverter requires the torque setpoint to be in 0.1% units
	  /* Torque is a function of how much the pedal is pressed, 100% pedal press
	   * means you request 100% of the allowed torque
	   */
	  torque_setpoint = (int16_t)(accel_pedal_percentage * 10.0f);

	  osMutexAcquire(InverterData1_MutexHandle, osWaitForever);
	  osMutexAcquire(InverterData2_MutexHandle, osWaitForever);
	  /*
	   * By default, the inverter will be set no torque limits or setpoints as needed
	   * by its start-up sequence and to ensure the car can't move randomly
	   */
		  if (current_state == STATE_RTD)
		  {
			  p_inverter_setpoints_1->torque_setpoint = torque_setpoint;
			  p_inverter_setpoints_1->torque_limit_positive = TORQUE_LIM_POS;
			  p_inverter_setpoints_1->torque_limit_negative = TORQUE_LIM_NEG;

			  p_inverter_setpoints_2->torque_setpoint = torque_setpoint;
			  p_inverter_setpoints_2->torque_limit_positive = TORQUE_LIM_POS;
			  p_inverter_setpoints_2->torque_limit_negative = TORQUE_LIM_NEG;
		  }
		  else
		  {
			  p_inverter_setpoints_1->torque_setpoint = 0;
			  p_inverter_setpoints_1->torque_limit_positive = 0;
			  p_inverter_setpoints_1->torque_limit_negative = 0;

			  p_inverter_setpoints_2->torque_setpoint = 0;
			  p_inverter_setpoints_2->torque_limit_positive = 0;
			  p_inverter_setpoints_2->torque_limit_negative = 0;
		  }

		  /*
		  // Attempt to reset errors on Inverter 1
		  if (p_inverter_status_1->error)
		  {
		      if (p_inverter_setpoints_1->torque_setpoint == 0
		          && inv_1_error_reset_attempts < MAX_ERROR_RESET_ATTEMPTS
		          && inv_1_error_reset_ready)
		      {
		          p_inverter_setpoints_1->control |= (1 << AMK_CONTROL_ERROR_RESET);
		          inv_1_error_reset_attempts++;
		          inv_1_error_reset_ready = false;
		          osTimerStart(INV_1_Error_Reset_TimerHandle, INVERTER_ERROR_RESET_RATE);
		      }
		      else
		      {
		          // Either waiting on the retry timer, or out of attempts
		          p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_ERROR_RESET);
		      }
		  }
		  else
		  {
		      // Error cleared
		      p_inverter_setpoints_1->control &= ~(1 << AMK_CONTROL_ERROR_RESET);
		      inv_1_error_reset_attempts = 0;
		      inv_1_error_reset_ready = true;
		      osTimerStop(INV_1_Error_Reset_TimerHandle);
		  }

		  // Attempt to reset errors on Inverter 2
		  if (p_inverter_status_2->error)
		  {
		      if (p_inverter_setpoints_2->torque_setpoint == 0
		          && inv_2_error_reset_attempts < MAX_ERROR_RESET_ATTEMPTS
		          && inv_2_error_reset_ready)
		      {
		          p_inverter_setpoints_2->control |= (1 << AMK_CONTROL_ERROR_RESET);
		          inv_2_error_reset_attempts++;
		          inv_2_error_reset_ready = false;
		          osTimerStart(INV_2_Error_Reset_TimerHandle, INVERTER_ERROR_RESET_RATE);
		      }
		      else
		      {
		          p_inverter_setpoints_2->control &= ~(1 << AMK_CONTROL_ERROR_RESET);
		      }
		  }
		  else
		  {
		      p_inverter_setpoints_2->control &= ~(1 << AMK_CONTROL_ERROR_RESET);
		      inv_2_error_reset_attempts = 0;
		      inv_2_error_reset_ready = true;
		      osTimerStop(INV_2_Error_Reset_TimerHandle);
		  }

		  */

	  osMutexRelease(InverterData1_MutexHandle);
	  osMutexRelease(InverterData2_MutexHandle);

    osDelay(1);
  }
}

void InverterCAN_Transmit_Callback(void *argument)
{
	InverterSetpoints_t inv_setpoints_1_local;
	InverterSetpoints_t inv_setpoints_2_local;

	osMutexAcquire(InverterData1_MutexHandle, osWaitForever);
		inv_setpoints_1_local = *p_inverter_setpoints_1;
	osMutexRelease(InverterData1_MutexHandle);

	osMutexAcquire(InverterData2_MutexHandle, osWaitForever);
		inv_setpoints_2_local = *p_inverter_setpoints_2;
	osMutexRelease(InverterData2_MutexHandle);

	osMutexAcquire(CAN1_MutexHandle, osWaitForever);
		transmit_inverter_command(&inv_setpoints_1_local, INVERTER_1_NODE_ADDRESS);
		transmit_inverter_command(&inv_setpoints_2_local, INVERTER_2_NODE_ADDRESS);
	osMutexRelease(CAN1_MutexHandle);
}

void INV_1_Error_Reset_Timer_Callback(void *argument)
{
	inv_1_error_reset_ready = true;
}

void INV_2_Error_Reset_Timer_Callback(void *argument)
{
	inv_2_error_reset_ready = true;
}

void Eint_CAN_Transmit_Timer_Callback(void *argument)
{

}

bool BrakesEngagedCheck(uint8_t brake_pedal_position_percentage,
						uint8_t brakes_engaged_threshold_percentage)
{
	bool brakes_engaged = false;

	if(brake_pedal_position_percentage > brakes_engaged_threshold_percentage)
	{
		brakes_engaged = true;
	}

	return brakes_engaged;

}
