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
extern osMessageQueueId_t CalibrationDataQueueHandle;
extern osMessageQueueId_t ADCsToLogQueueHandle;

extern osMutexId_t SystemState_MutexHandle;

extern osMutexId_t SystemState_MutexHandle;
extern osMutexId_t VehicleState_MutexHandle;
extern osMutexId_t InverterData1_MutexHandle;
extern osMutexId_t InverterData2_MutexHandle;
extern osMutexId_t PedalsState_MutexHandle;
extern osMutexId_t InverterData1_MutexHandle;
extern osMutexId_t PedalsFaults_MutexHandle;

extern osTimerId_t RTD_Button_TimerHandle;
extern osTimerId_t InverterCAN_Transmit_TimerHandle;
extern osTimerId_t RTD_Buzzer_TimerHandle;
extern osTimerId_t APPS_1_OutOfRange_TimerHandle;
extern osTimerId_t APPS_2_OutOfRange_TimerHandle;
extern osTimerId_t BSE_OutOfRange_TimerHandle;
extern osTimerId_t APPS_Implausibility_TimerHandle;


extern osEventFlagsId_t CalibrationDataReceived_EventHandle;
extern osEventFlagsId_t PedalsOutOfRangeFault_EventHandle;

#endif /* INC_APP_APP_FREERTOS_H_ */
