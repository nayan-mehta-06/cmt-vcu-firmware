/*
 * app_freertos.h
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#ifndef INC_APP_APP_FREERTOS_H_
#define INC_APP_APP_FREERTOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

extern osMessageQueueId_t CanMsgQueueHandle;
extern osMessageQueueId_t StateTransitionQueueHandle;
extern osMessageQueueId_t PedalsADCQueueHandle;

extern osMutexId_t SystemState_MutexHandle;

extern osMutexId_t SystemState_MutexHandle;
extern osMutexId_t VehicleState_MutexHandle;
extern osMutexId_t InverterData1_MutexHandle;
extern osMutexId_t InverterData2_MutexHandle;
extern osMutexId_t PedalsState_MutexHandle;
extern osMutexId_t InverterData1_MutexHandle;

extern osTimerId_t RTD_ButtonTimerHandle;


void Task_Process_CAN_Msgs(void *argument);


#endif /* INC_APP_APP_FREERTOS_H_ */
