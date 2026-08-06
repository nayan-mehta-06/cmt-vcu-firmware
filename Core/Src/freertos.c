/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "app/state_control.h"
#include "app/process_adc.h"
#include "app/calibration.h"
#include "app/handle_gui.h"
#include "app/process_pedals_data.h"
#include "app/RTD.h"
#include "app/vehicle_control.h"
#include "app/log_vehicle_data.h"
#include "app/vehicle_fault_handler.h"
#include "drivers/can_driver.h"

#include "config/vcu_config.h"

#include <stdlib.h>
#include "can.h"
#include "adc.h"
#include "tim.h"
#include "cmt_utils.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for LogTaskStackUsage */
osThreadId_t LogTaskStackUsageHandle;
const osThreadAttr_t LogTaskStackUsage_attributes = {
  .name = "LogTaskStackUsage",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CalibrateTask */
osThreadId_t CalibrateTaskHandle;
const osThreadAttr_t CalibrateTask_attributes = {
  .name = "CalibrateTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for RTD_Task */
osThreadId_t RTD_TaskHandle;
const osThreadAttr_t RTD_Task_attributes = {
  .name = "RTD_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProcessADC_Task */
osThreadId_t ProcessADC_TaskHandle;
const osThreadAttr_t ProcessADC_Task_attributes = {
  .name = "ProcessADC_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PedalsDataTask */
osThreadId_t PedalsDataTaskHandle;
const osThreadAttr_t PedalsDataTask_attributes = {
  .name = "PedalsDataTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for VehicleCtrlTask */
osThreadId_t VehicleCtrlTaskHandle;
const osThreadAttr_t VehicleCtrlTask_attributes = {
  .name = "VehicleCtrlTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for StateCtrlTask */
osThreadId_t StateCtrlTaskHandle;
const osThreadAttr_t StateCtrlTask_attributes = {
  .name = "StateCtrlTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProcessCAN_Task */
osThreadId_t ProcessCAN_TaskHandle;
const osThreadAttr_t ProcessCAN_Task_attributes = {
  .name = "ProcessCAN_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for HandleGUI_Task */
osThreadId_t HandleGUI_TaskHandle;
const osThreadAttr_t HandleGUI_Task_attributes = {
  .name = "HandleGUI_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for LogVehicleData_Task */
osThreadId_t LogVehicleData_TaskHandle;
const osThreadAttr_t LogVehicleData_Task_attributes = {
  .name = "LogVehicleData_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for VehicleFaultHandler_Task */
osThreadId_t VehicleFaultHandler_TaskHandle;
const osThreadAttr_t VehicleFaultHandler_Task_attributes = {
  .name = "VehicleFaultHandler_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for StateTransitionQueue */
osMessageQueueId_t StateTransitionQueueHandle;
const osMessageQueueAttr_t StateTransitionQueue_attributes = {
  .name = "StateTransitionQueue"
};
/* Definitions for CanMsgQueue */
osMessageQueueId_t CanMsgQueueHandle;
const osMessageQueueAttr_t CanMsgQueue_attributes = {
  .name = "CanMsgQueue"
};
/* Definitions for PedalsADCQueue */
osMessageQueueId_t PedalsADCQueueHandle;
const osMessageQueueAttr_t PedalsADCQueue_attributes = {
  .name = "PedalsADCQueue"
};
/* Definitions for CalibrationDataQueue */
osMessageQueueId_t CalibrationDataQueueHandle;
const osMessageQueueAttr_t CalibrationDataQueue_attributes = {
  .name = "CalibrationDataQueue"
};
/* Definitions for ADCsToLogQueue */
osMessageQueueId_t ADCsToLogQueueHandle;
const osMessageQueueAttr_t ADCsToLogQueue_attributes = {
  .name = "ADCsToLogQueue"
};
/* Definitions for RTD_Button_Timer */
osTimerId_t RTD_Button_TimerHandle;
const osTimerAttr_t RTD_Button_Timer_attributes = {
  .name = "RTD_Button_Timer"
};
/* Definitions for InverterCAN_Transmit_Timer */
osTimerId_t InverterCAN_Transmit_TimerHandle;
const osTimerAttr_t InverterCAN_Transmit_Timer_attributes = {
  .name = "InverterCAN_Transmit_Timer"
};
/* Definitions for RTD_Buzzer_Timer */
osTimerId_t RTD_Buzzer_TimerHandle;
const osTimerAttr_t RTD_Buzzer_Timer_attributes = {
  .name = "RTD_Buzzer_Timer"
};
/* Definitions for APPS_1_OutOfRange_Timer */
osTimerId_t APPS_1_OutOfRange_TimerHandle;
const osTimerAttr_t APPS_1_OutOfRange_Timer_attributes = {
  .name = "APPS_1_OutOfRange_Timer"
};
/* Definitions for BSE_OutOfRange_Timer */
osTimerId_t BSE_OutOfRange_TimerHandle;
const osTimerAttr_t BSE_OutOfRange_Timer_attributes = {
  .name = "BSE_OutOfRange_Timer"
};
/* Definitions for APPS_Implausibility_Timer */
osTimerId_t APPS_Implausibility_TimerHandle;
const osTimerAttr_t APPS_Implausibility_Timer_attributes = {
  .name = "APPS_Implausibility_Timer"
};
/* Definitions for APPS_2_OutOfRange_Timer */
osTimerId_t APPS_2_OutOfRange_TimerHandle;
const osTimerAttr_t APPS_2_OutOfRange_Timer_attributes = {
  .name = "APPS_2_OutOfRange_Timer"
};
/* Definitions for CAN1_Mutex */
osMutexId_t CAN1_MutexHandle;
const osMutexAttr_t CAN1_Mutex_attributes = {
  .name = "CAN1_Mutex"
};
/* Definitions for CAN2_Mutex */
osMutexId_t CAN2_MutexHandle;
const osMutexAttr_t CAN2_Mutex_attributes = {
  .name = "CAN2_Mutex"
};
/* Definitions for SystemState_Mutex */
osMutexId_t SystemState_MutexHandle;
const osMutexAttr_t SystemState_Mutex_attributes = {
  .name = "SystemState_Mutex"
};
/* Definitions for VehicleState_Mutex */
osMutexId_t VehicleState_MutexHandle;
const osMutexAttr_t VehicleState_Mutex_attributes = {
  .name = "VehicleState_Mutex"
};
/* Definitions for InverterData1_Mutex */
osMutexId_t InverterData1_MutexHandle;
const osMutexAttr_t InverterData1_Mutex_attributes = {
  .name = "InverterData1_Mutex"
};
/* Definitions for InverterData2_Mutex */
osMutexId_t InverterData2_MutexHandle;
const osMutexAttr_t InverterData2_Mutex_attributes = {
  .name = "InverterData2_Mutex"
};
/* Definitions for PedalsState_Mutex */
osMutexId_t PedalsState_MutexHandle;
const osMutexAttr_t PedalsState_Mutex_attributes = {
  .name = "PedalsState_Mutex"
};
/* Definitions for PedalsFaults_Mutex */
osMutexId_t PedalsFaults_MutexHandle;
const osMutexAttr_t PedalsFaults_Mutex_attributes = {
  .name = "PedalsFaults_Mutex"
};
/* Definitions for CalibrationDataReceived_Event */
osEventFlagsId_t CalibrationDataReceived_EventHandle;
const osEventFlagsAttr_t CalibrationDataReceived_Event_attributes = {
  .name = "CalibrationDataReceived_Event"
};
/* Definitions for PedalsOutOfRangeFault_Event */
osEventFlagsId_t PedalsOutOfRangeFault_EventHandle;
const osEventFlagsAttr_t PedalsOutOfRangeFault_Event_attributes = {
  .name = "PedalsOutOfRangeFault_Event"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Task_LogTaskStackUsage(void *argument);
extern void Task_Car_Calibration(void *argument);
extern void Task_RTD(void *argument);
extern void Task_Process_ADC(void *argument);
extern void Task_Process_Pedals_Data(void *argument);
extern void Task_Vehicle_Ctrl(void *argument);
extern void Task_State_Ctrl(void *argument);
extern void Task_Process_CAN_Msgs(void *argument);
extern void Task_HandleGUI(void *argument);
extern void Task_LogVehicleData(void *argument);
extern void Task_VehicleFaultHandler(void *argument);
extern void RTD_Button_Timer_Callback(void *argument);
extern void InverterCAN_Transmit_Callback(void *argument);
extern void RTD_Buzzer_Timer_Callback(void *argument);
extern void APPS_1_OutOfRange_Timer_Callback(void *argument);
extern void BSE_OutOfRange_Timer_Callback(void *argument);
extern void APPS_Implausibility_Timer_Callback(void *argument);
extern void APPS_2_OutOfRange_Timer_Callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of CAN1_Mutex */
  CAN1_MutexHandle = osMutexNew(&CAN1_Mutex_attributes);

  /* creation of CAN2_Mutex */
  CAN2_MutexHandle = osMutexNew(&CAN2_Mutex_attributes);

  /* creation of SystemState_Mutex */
  SystemState_MutexHandle = osMutexNew(&SystemState_Mutex_attributes);

  /* creation of VehicleState_Mutex */
  VehicleState_MutexHandle = osMutexNew(&VehicleState_Mutex_attributes);

  /* creation of InverterData1_Mutex */
  InverterData1_MutexHandle = osMutexNew(&InverterData1_Mutex_attributes);

  /* creation of InverterData2_Mutex */
  InverterData2_MutexHandle = osMutexNew(&InverterData2_Mutex_attributes);

  /* creation of PedalsState_Mutex */
  PedalsState_MutexHandle = osMutexNew(&PedalsState_Mutex_attributes);

  /* creation of PedalsFaults_Mutex */
  PedalsFaults_MutexHandle = osMutexNew(&PedalsFaults_Mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of RTD_Button_Timer */
  RTD_Button_TimerHandle = osTimerNew(RTD_Button_Timer_Callback, osTimerOnce, NULL, &RTD_Button_Timer_attributes);

  /* creation of InverterCAN_Transmit_Timer */
  InverterCAN_Transmit_TimerHandle = osTimerNew(InverterCAN_Transmit_Callback, osTimerPeriodic, NULL, &InverterCAN_Transmit_Timer_attributes);

  /* creation of RTD_Buzzer_Timer */
  RTD_Buzzer_TimerHandle = osTimerNew(RTD_Buzzer_Timer_Callback, osTimerOnce, NULL, &RTD_Buzzer_Timer_attributes);

  /* creation of APPS_1_OutOfRange_Timer */
  APPS_1_OutOfRange_TimerHandle = osTimerNew(APPS_1_OutOfRange_Timer_Callback, osTimerOnce, NULL, &APPS_1_OutOfRange_Timer_attributes);

  /* creation of BSE_OutOfRange_Timer */
  BSE_OutOfRange_TimerHandle = osTimerNew(BSE_OutOfRange_Timer_Callback, osTimerOnce, NULL, &BSE_OutOfRange_Timer_attributes);

  /* creation of APPS_Implausibility_Timer */
  APPS_Implausibility_TimerHandle = osTimerNew(APPS_Implausibility_Timer_Callback, osTimerOnce, NULL, &APPS_Implausibility_Timer_attributes);

  /* creation of APPS_2_OutOfRange_Timer */
  APPS_2_OutOfRange_TimerHandle = osTimerNew(APPS_2_OutOfRange_Timer_Callback, osTimerOnce, NULL, &APPS_2_OutOfRange_Timer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of StateTransitionQueue */
  StateTransitionQueueHandle = osMessageQueueNew (8, sizeof(SystemEvent_t), &StateTransitionQueue_attributes);

  /* creation of CanMsgQueue */
  CanMsgQueueHandle = osMessageQueueNew (32, sizeof(CanMsg_t), &CanMsgQueue_attributes);

  /* creation of PedalsADCQueue */
  PedalsADCQueueHandle = osMessageQueueNew (8, sizeof(PedalsADC_t), &PedalsADCQueue_attributes);

  /* creation of CalibrationDataQueue */
  CalibrationDataQueueHandle = osMessageQueueNew (8, sizeof(CalibrationData_t), &CalibrationDataQueue_attributes);

  /* creation of ADCsToLogQueue */
  ADCsToLogQueueHandle = osMessageQueueNew (8, sizeof(ADCsToLog_t), &ADCsToLogQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LogTaskStackUsage */
  LogTaskStackUsageHandle = osThreadNew(Task_LogTaskStackUsage, NULL, &LogTaskStackUsage_attributes);

  /* creation of CalibrateTask */
  CalibrateTaskHandle = osThreadNew(Task_Car_Calibration, NULL, &CalibrateTask_attributes);

  /* creation of RTD_Task */
  RTD_TaskHandle = osThreadNew(Task_RTD, NULL, &RTD_Task_attributes);

  /* creation of ProcessADC_Task */
  ProcessADC_TaskHandle = osThreadNew(Task_Process_ADC, NULL, &ProcessADC_Task_attributes);

  /* creation of PedalsDataTask */
  PedalsDataTaskHandle = osThreadNew(Task_Process_Pedals_Data, NULL, &PedalsDataTask_attributes);

  /* creation of VehicleCtrlTask */
  VehicleCtrlTaskHandle = osThreadNew(Task_Vehicle_Ctrl, NULL, &VehicleCtrlTask_attributes);

  /* creation of StateCtrlTask */
  StateCtrlTaskHandle = osThreadNew(Task_State_Ctrl, NULL, &StateCtrlTask_attributes);

  /* creation of ProcessCAN_Task */
  ProcessCAN_TaskHandle = osThreadNew(Task_Process_CAN_Msgs, NULL, &ProcessCAN_Task_attributes);

  /* creation of HandleGUI_Task */
  HandleGUI_TaskHandle = osThreadNew(Task_HandleGUI, NULL, &HandleGUI_Task_attributes);

  /* creation of LogVehicleData_Task */
  LogVehicleData_TaskHandle = osThreadNew(Task_LogVehicleData, NULL, &LogVehicleData_Task_attributes);

  /* creation of VehicleFaultHandler_Task */
  VehicleFaultHandler_TaskHandle = osThreadNew(Task_VehicleFaultHandler, NULL, &VehicleFaultHandler_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of CalibrationDataReceived_Event */
  CalibrationDataReceived_EventHandle = osEventFlagsNew(&CalibrationDataReceived_Event_attributes);

  /* creation of PedalsOutOfRangeFault_Event */
  PedalsOutOfRangeFault_EventHandle = osEventFlagsNew(&PedalsOutOfRangeFault_Event_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Task_LogTaskStackUsage */
/**
  * @brief  Function implementing the LogTaskStackUsage thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Task_LogTaskStackUsage */
void Task_LogTaskStackUsage(void *argument)
{
  /* USER CODE BEGIN Task_LogTaskStackUsage */
  /* Infinite loop */
  for(;;)
  {
	  p_freertos_task_stack_usage->used_bytes_LogTaskStackUsage =
	        LogTaskStackUsage_attributes.stack_size - osThreadGetStackSpace(LogTaskStackUsageHandle);

	  p_freertos_task_stack_usage->used_bytes_CalibrateTask =
	        CalibrateTask_attributes.stack_size - osThreadGetStackSpace(CalibrateTaskHandle);

	  p_freertos_task_stack_usage->used_bytes_RTD_Task =
	        RTD_Task_attributes.stack_size - osThreadGetStackSpace(RTD_TaskHandle);

	  p_freertos_task_stack_usage->used_bytes_ProcessADC_Task =
	        ProcessADC_Task_attributes.stack_size - osThreadGetStackSpace(ProcessADC_TaskHandle);

	  p_freertos_task_stack_usage->used_bytes_PedalsDataTask =
	        PedalsDataTask_attributes.stack_size - osThreadGetStackSpace(PedalsDataTaskHandle);

	  p_freertos_task_stack_usage->used_bytes_VehicleCtrlTask =
	        VehicleCtrlTask_attributes.stack_size - osThreadGetStackSpace(VehicleCtrlTaskHandle);

	  p_freertos_task_stack_usage->used_bytes_StateCtrlTask =
	        StateCtrlTask_attributes.stack_size - osThreadGetStackSpace(StateCtrlTaskHandle);

	  p_freertos_task_stack_usage->used_bytes_ProcessCAN_Task =
	        ProcessCAN_Task_attributes.stack_size - osThreadGetStackSpace(ProcessCAN_TaskHandle);

	  p_freertos_task_stack_usage->used_bytes_HandleGUI_Task =
	        HandleGUI_Task_attributes.stack_size - osThreadGetStackSpace(HandleGUI_TaskHandle);

	  p_freertos_task_stack_usage->used_bytes_LogVehicleData_Task =
	        LogVehicleData_Task_attributes.stack_size - osThreadGetStackSpace(LogVehicleData_TaskHandle);

	  p_freertos_task_stack_usage->used_bytes_VehicleFaultHandler_Task =
	        VehicleFaultHandler_Task_attributes.stack_size - osThreadGetStackSpace(VehicleFaultHandler_TaskHandle);

    osDelay(1);
  }
  /* USER CODE END Task_LogTaskStackUsage */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

