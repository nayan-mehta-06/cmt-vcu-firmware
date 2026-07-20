/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Spare_5V_OUT_Pin|PC_Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI_HOLD_GPIO_Port, SPI_HOLD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RTD_Buzzer_Enable_Pin|RTD_Button_Light_Enable_Pin|SPI_WriteProtect_Pin|SPI_ChipSelect_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Brake_Light_Enable_GPIO_Port, Brake_Light_Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Spare_24V_Out_Pin|Inverter_24V_Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RTD_Button_Status_IN_Pin AIR__IN_Pin */
  GPIO_InitStruct.Pin = RTD_Button_Status_IN_Pin|AIR__IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PC_Complete_IN_Pin */
  GPIO_InitStruct.Pin = PC_Complete_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PC_Complete_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Spare_5V_OUT_Pin PC_Enable_Pin */
  GPIO_InitStruct.Pin = Spare_5V_OUT_Pin|PC_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : Precharge_Complete_Pin */
  GPIO_InitStruct.Pin = Precharge_Complete_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Precharge_Complete_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_HOLD_Pin */
  GPIO_InitStruct.Pin = SPI_HOLD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI_HOLD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RTD_Buzzer_Enable_Pin RTD_Button_Light_Enable_Pin SPI_WriteProtect_Pin SPI_ChipSelect_Pin */
  GPIO_InitStruct.Pin = RTD_Buzzer_Enable_Pin|RTD_Button_Light_Enable_Pin|SPI_WriteProtect_Pin|SPI_ChipSelect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : Brake_Light_Enable_Pin */
  GPIO_InitStruct.Pin = Brake_Light_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Brake_Light_Enable_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Spare_24V_Out_Pin Inverter_24V_Enable_Pin */
  GPIO_InitStruct.Pin = Spare_24V_Out_Pin|Inverter_24V_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BSE_Fault_IN_Pin Plausibility_Fault_IN_Pin CS_Fault_IN_Pin */
  GPIO_InitStruct.Pin = BSE_Fault_IN_Pin|Plausibility_Fault_IN_Pin|CS_Fault_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
