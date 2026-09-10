/*
 * process_can_msgs.c
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */


#include <app/process_can_msgs.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include <config/vehicle_data_structs.h>
#include <config/amk_data_structs.h>
#include "config/can_ids.h"
#include "drivers/can_driver.h"
#include "app/app_freertos.h"
#include "app/inverter_control.h"
#include "cmt_utils.h"


void Task_Process_CAN_Msgs(void *argument)
{
	CanMsg_t msg;

	for(;;)
	{

		/* Only process the message if it was successfully received from the queue */
	  if (osMessageQueueGet(CanMsgQueueHandle, &msg, NULL, osWaitForever) == osOK)
	  {
		  if (msg.can_line == 1)
		  {
			  switch (msg.StdId)
			  {
				  case BMS_ID:
					osMutexAcquire(VehicleState_MutexHandle, osWaitForever);

					  uint32_t packVoltageRaw = CONVERT_TO_32BIT(msg.data[0], msg.data[1], msg.data[2], msg.data[3]);
					  p_vehicle_state_data->BMS_voltage = packVoltageRaw / 10000.0f;

					osMutexRelease(VehicleState_MutexHandle);
				  break;

				  case IVTS_U1:
					osMutexAcquire(VehicleState_MutexHandle, osWaitForever);

						uint32_t inverterVoltageRaw = CONVERT_TO_32BIT(msg.data[2], msg.data[3], msg.data[4], msg.data[5]);
						p_vehicle_state_data->inverter_voltage = inverterVoltageRaw / 1000.0f;

					osMutexRelease(VehicleState_MutexHandle);
				  break;

				  case IVTS_U2:
					osMutexAcquire(VehicleState_MutexHandle, osWaitForever);

						uint32_t accyVoltageRaw = CONVERT_TO_32BIT(msg.data[2], msg.data[3], msg.data[4], msg.data[5]);
						p_vehicle_state_data->accy_voltage = accyVoltageRaw / 1000.0f;

					osMutexRelease(VehicleState_MutexHandle);
				  break;

				  case (AMK_RECEIVE_MSG_1+INVERTER_1_NODE_ADDRESS):
					osMutexAcquire(InverterData1_MutexHandle, osWaitForever);

				  	  setInverterData1(p_inverter_data_1, msg.data);
					  processInverterStatus(p_inverter_status_1, p_inverter_data_1->status);

					osMutexRelease(InverterData1_MutexHandle);
				  break;

				  case (AMK_RECEIVE_MSG_1+INVERTER_2_NODE_ADDRESS):
					osMutexAcquire(InverterData2_MutexHandle, osWaitForever);

				  	  setInverterData1(p_inverter_data_2, msg.data);
					  processInverterStatus(p_inverter_status_2, p_inverter_data_2->status);

					osMutexRelease(InverterData2_MutexHandle);
				  break;

				  case (AMK_RECEIVE_MSG_2+INVERTER_1_NODE_ADDRESS):
					osMutexAcquire(InverterData1_MutexHandle, osWaitForever);

				  	  	  setInverterData2(p_inverter_data_1, msg.data);

					osMutexRelease(InverterData1_MutexHandle);
				  break;

				  case (AMK_RECEIVE_MSG_2+INVERTER_2_NODE_ADDRESS):
					osMutexAcquire(InverterData2_MutexHandle, osWaitForever);

				  	  	  setInverterData2(p_inverter_data_2, msg.data);

					osMutexRelease(InverterData2_MutexHandle);
				  break;

				  case (EINT_ID):
				    osMutexAcquire(EintData_MutexHandle, osWaitForever);

				  	  p_eint_data->power_limit = msg.data[0];
				  	  if (msg.data[3] == 0xFF)
				  	  {
				  		  p_eint_data->launch_control = true;
				  	  }
				  	  else
				  	  {
				  		  p_eint_data->launch_control = false;
				  	  }

				    osMutexRelease(EintData_MutexHandle);
				  break;
			  }
		  }
	  }
	osDelay(1);
  }
}
