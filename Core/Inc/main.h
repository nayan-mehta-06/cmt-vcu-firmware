/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern SPI_HandleTypeDef hspi2;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint32_t TxMailbox;
extern uint32_t TxMailbox_CAN2;

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BPS1_Pin GPIO_PIN_4
#define BPS1_GPIO_Port GPIOA
#define SPARE_ADC_Pin GPIO_PIN_5
#define SPARE_ADC_GPIO_Port GPIOA
#define BPS2_Pin GPIO_PIN_6
#define BPS2_GPIO_Port GPIOA
#define STEERING_ANGLE_ADC_Pin GPIO_PIN_7
#define STEERING_ANGLE_ADC_GPIO_Port GPIOA
#define APPS1_Pin GPIO_PIN_4
#define APPS1_GPIO_Port GPIOC
#define APPS2_Pin GPIO_PIN_5
#define APPS2_GPIO_Port GPIOC
#define BSE_ADC_Pin GPIO_PIN_0
#define BSE_ADC_GPIO_Port GPIOB
#define BSPD_CS_ADC_Pin GPIO_PIN_1
#define BSPD_CS_ADC_GPIO_Port GPIOB
#define RTD_Button_Status_IN_Pin GPIO_PIN_10
#define RTD_Button_Status_IN_GPIO_Port GPIOE
#define AIR__IN_Pin GPIO_PIN_11
#define AIR__IN_GPIO_Port GPIOE
#define PC_Complete_IN_Pin GPIO_PIN_12
#define PC_Complete_IN_GPIO_Port GPIOE
#define Spare_5V_OUT_Pin GPIO_PIN_13
#define Spare_5V_OUT_GPIO_Port GPIOE
#define PC_Enable_Pin GPIO_PIN_14
#define PC_Enable_GPIO_Port GPIOE
#define Precharge_Complete_Pin GPIO_PIN_11
#define Precharge_Complete_GPIO_Port GPIOB
#define SPI_HOLD_Pin GPIO_PIN_12
#define SPI_HOLD_GPIO_Port GPIOB
#define RTD_Buzzer_Enable_Pin GPIO_PIN_8
#define RTD_Buzzer_Enable_GPIO_Port GPIOD
#define RTD_Button_Light_Enable_Pin GPIO_PIN_9
#define RTD_Button_Light_Enable_GPIO_Port GPIOD
#define SPI_WriteProtect_Pin GPIO_PIN_10
#define SPI_WriteProtect_GPIO_Port GPIOD
#define SPI_ChipSelect_Pin GPIO_PIN_12
#define SPI_ChipSelect_GPIO_Port GPIOD
#define Brake_Light_Enable_Pin GPIO_PIN_4
#define Brake_Light_Enable_GPIO_Port GPIOG
#define Spare_24V_Out_Pin GPIO_PIN_7
#define Spare_24V_Out_GPIO_Port GPIOC
#define Inverter_24V_Enable_Pin GPIO_PIN_8
#define Inverter_24V_Enable_GPIO_Port GPIOC
#define BSE_Fault_IN_Pin GPIO_PIN_9
#define BSE_Fault_IN_GPIO_Port GPIOG
#define Plausibility_Fault_IN_Pin GPIO_PIN_11
#define Plausibility_Fault_IN_GPIO_Port GPIOG
#define CS_Fault_IN_Pin GPIO_PIN_13
#define CS_Fault_IN_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */
#define EVT_INVERTER_ENABLED   (1 << 0)
#define EVT_INVERTER_DISABLED  (1 << 1)

#define CAN_1 1
#define CAN_2 2

#define CAN_STD_ID_FORMAT 0
#define CAN_EXT_ID_FORMAT 1

#define AMK_STATUS_RESERVE 0
#define AMK_STATUS_SYSTEM_READY 8
#define AMK_STATUS_ERROR 9
#define AMK_STATUS_WARN 10
#define AMK_STATUS_QUIT_DC_ON 11
#define AMK_STATUS_DC_ON 12
#define AMK_STATUS_QUIT_INVERTER_ON 13
#define AMK_STATUS_INVERTER_ON 14
#define AMK_STATUS_DERATING 15

#define AMK_CONTROL_RESERVE 0
#define AMK_CONTROL_INVERTER_ON 8
#define AMK_CONTROL_DC_ON 9
#define AMK_CONTROL_ENABLE 10
#define AMK_CONTROL_ERROR_RESET 11
#define AMK_CONTROL_RESERVE_2 12

#define AMK_ID110 10

#define MAX_RPM 30000
#define MAX_RPM_NO_FIELD_WEAKENING 20000
#define MMAX_MN_RATIO 2.1

#define POS_VELOCITY_LIM 1000
#define NEG_VELOCITY_LIM -100

#define POS_TORQUE_LIM 100
#define NEG_TORQUE_LIM 0

/* #define MAX_SPEED 1500 */


#define QUEUE_MESSAGE_PRIORITY 0

#define CAN_MSG_QUEUE_TIMEOUT_MILLISECONDS pdMS_TO_TICKS(0)
#define ADC_INPUT_QUEUE_TIMEOUT_MILLISECONDS pdMS_TO_TICKS(10)

#define RTD_BUTTON_PRESS_MILLISECONDS pdMS_TO_TICKS(200)
// Try reduce to 10Hz for traction control


#define ADC_INDEX_USED 2
#define ADC_BUFFER_LENGTH 8
#define MAX_ADC_VALUE 4096

extern uint16_t raw_ADC_values[ADC_BUFFER_LENGTH];

#define PRECHARGE_PERCENTAGE 90

// Define commands for checking status of inputs
#define CHECK_RTD_BUTTON_STATUS() HAL_GPIO_ReadPin(RTD_Button_Status_IN_GPIO_Port, RTD_Button_Status_IN_Pin)
#define CHECK_PRECHARGE_COMPLETE_STATUS() HAL_GPIO_ReadPin(PC_Complete_IN_GPIO_Port, PC_Complete_IN_Pin)

// Define commands for controlling outputs
#define INVERTER_ENABLE() HAL_GPIO_WritePin(Inverter_24V_Enable_GPIO_Port, Inverter_24V_Enable_Pin, GPIO_PIN_SET)
#define INVERTER_DISABLE() HAL_GPIO_WritePin(Inverter_24V_Enable_GPIO_Port, Inverter_24V_Enable_Pin, GPIO_PIN_RESET)

#define PRECHARGE_SIGNAL_ENABLE() HAL_GPIO_WritePin(PC_Enable_GPIO_Port, PC_Enable_Pin, GPIO_PIN_SET)
#define PRECHARGE_SIGNAL_DISABLE() HAL_GPIO_WritePin(PC_Enable_GPIO_Port, PC_Enable_Pin, GPIO_PIN_RESET)

#define RTD_BUTTON_LIGHT_ON() HAL_GPIO_WritePin(RTD_Button_Light_Enable_GPIO_Port, RTD_Button_Light_Enable_Pin, GPIO_PIN_SET)
#define RTD_BUTTON_LIGHT_OFF() HAL_GPIO_WritePin(RTD_Button_Light_Enable_GPIO_Port, RTD_Button_Light_Enable_Pin, GPIO_PIN_RESET)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
