/*
 * can_driver.c
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */


#include "drivers/can_driver.h"
#include "app/app_freertos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can.h"
#include <math.h>
#include "main.h"


// If there is a new message, store in the important parts and send it to the process_can_msg task
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CanMsg_t msg;

	// Read CAN Message
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, msg.data) != HAL_OK)
	{
		Error_Handler();
	}

	msg.StdId = RxHeader.StdId;
	msg.ExtId = RxHeader.ExtId;
	msg.DLC = RxHeader.DLC;
	msg.can_line = (hcan->Instance == CAN1) ? 1 : 2;

	if (osMessageQueuePut(CanMsgQueueHandle, &msg, QUEUE_MESSAGE_PRIORITY, CAN_MSG_QUEUE_TIMEOUT_MILLISECONDS) != osOK)
	{
		p_queue_errors_data->can_msg_errors++;
	}
}


// Code below has been taken from 2025 Firmware

// Primary CANbus error flag = 8 error counts
// Threshold for error passive is 120 error count
// Threshold for bus off state is 255 errors counts
// Passive state threshold: 120/8 = 15 errors
// Bus off state threshold: 255/8 = 31 errors
// 20 errors seems reasonable
#define CAN_1_TRANSMISSION_ERRORS_FAULT_THRESHOLD 20
#define CAN_2_TRANSMISSION_ERRORS_FAULT_THRESHOLD 20

// 0 - no pending transmission
// 1 - pending transmission
#define CAN_TRANSMISSION_NOT_PENDING 0
#define CAN_TRANSMISSION_PENDING 1

int CAN_1_transmission_error_counter = 0;
int CAN_2_transmission_error_counter = 0;
bool CAN_1_disabled = false;
bool CAN_2_disabled = false;

uint32_t number_of_free_CAN_1_Tx_mailboxes = 0;
uint32_t CAN_1_transmission_pending_status = CAN_TRANSMISSION_NOT_PENDING;

uint32_t number_of_free_CAN_2_Tx_mailboxes = 0;
uint32_t CAN_2_transmission_pending_status = CAN_TRANSMISSION_NOT_PENDING;

// To transmit CAN messages
bool CAN_transmit(uint8_t* data, int dlc, int id, int can_line, int CAN_ID_format)
{

	bool CAN_transmission_sucessful = false;

	CAN_TxHeaderTypeDef CAN_TxHeader = {
				.RTR = CAN_RTR_DATA,
				.DLC = dlc

	};

	// Standard ID
	if (CAN_ID_format == 0)
	{

		CAN_TxHeader.IDE = CAN_ID_STD;
		CAN_TxHeader.StdId = id;

	}

	// Extended ID
	else
	{

		CAN_TxHeader.IDE = CAN_ID_EXT;
		CAN_TxHeader.ExtId = id;

	}

	// Create buffer with size of the dlc
	uint8_t txData[dlc];

	// Fill the transmit buffer with the data
	for (int i = 0; i < dlc; i++)
	{

		txData[i] = data[i];

	}

	// Select CAN Line
	if(can_line == 1)
	{


			// Check if any mailboxes are free
			number_of_free_CAN_1_Tx_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);

			// Check if a transmission is pending
			CAN_1_transmission_pending_status = HAL_CAN_IsTxMessagePending(&hcan1, TxMailbox);

			// Only attempt to transmit a CAN message if at least 1 mailbox is free
			if(number_of_free_CAN_1_Tx_mailboxes > 0)
			{

				/*** Transmit CAN 1 Message ***/

				// Transmit CAN message
				if(HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeader, txData, &TxMailbox) != HAL_OK)
				{

					// Increment error fault counter for every failed CAN transmission
					CAN_1_transmission_error_counter += 1;
					CAN_transmission_sucessful = false;

				}

				else
				{

					CAN_transmission_sucessful = true;

					if(CAN_1_transmission_error_counter > 0)
					{

						// Decrement error fault counter for every sucessful CAN transmission
						CAN_1_transmission_error_counter -= 1;

					}

				}

			}

			// If no mailboxes are free and a CAN transmission is pending, abort the most recent transmission request
			// Objective of this code is to make sure that if no mailboxes are free, we can abort the most recent CAN transmission
			// This might allow stuff to get transmitted again
			else if(CAN_1_transmission_pending_status == CAN_TRANSMISSION_PENDING)
			{

				// Abort CAN transmission
				// Frees space in the mailbox
				HAL_CAN_AbortTxRequest(&hcan1, TxMailbox);

			}

			// For logging CAN error status to dash
			if(CAN_1_transmission_error_counter > CAN_1_TRANSMISSION_ERRORS_FAULT_THRESHOLD)
			{

				// Handle error by disabling and re-enabling CAN
				CAN_1_Error_Handler();

			}

			else
			{


			}

	}

	else if(can_line == 2)
	{


			// Check if any mailboxes are free
			number_of_free_CAN_2_Tx_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);

			// Check if a transmission is pending
			CAN_2_transmission_pending_status = HAL_CAN_IsTxMessagePending(&hcan2, TxMailbox_CAN2);

			// Only attempt to transmit a CAN message if at least 1 mailbox is free
			if(number_of_free_CAN_2_Tx_mailboxes > 0)
			{

				/*** Transmit CAN 2 Message ***/

				// Transmit CAN message
				if(HAL_CAN_AddTxMessage(&hcan2, &CAN_TxHeader, txData, &TxMailbox_CAN2) != HAL_OK)
				{

					// Increment error fault counter for every failed CAN transmission
					CAN_2_transmission_error_counter += 1;
					CAN_transmission_sucessful = false;

				}

				else
				{

					CAN_transmission_sucessful = true;

					if(CAN_2_transmission_error_counter > 0)
					{

						// Decrement error fault counter for every sucessful CAN transmission
						CAN_2_transmission_error_counter -= 1;

					}

				}

			}

			// If no mailboxes are free and a CAN transmission is pending, abort the most recent transmission request
			// Objective of this code is to make sure that if no mailboxes are free, we can abort the most recent CAN transmission
			// This might allow stuff to get transmitted again
			else if(CAN_2_transmission_pending_status == CAN_TRANSMISSION_PENDING)
			{

				// Abort CAN transmission
				// Frees space in the mailbox
				HAL_CAN_AbortTxRequest(&hcan2, TxMailbox_CAN2);

			}

			// For logging CAN error status to dash
			if(CAN_2_transmission_error_counter > CAN_2_TRANSMISSION_ERRORS_FAULT_THRESHOLD)
			{

				// Handle error by disabling and re-enabling CAN
				CAN_2_Error_Handler();

			}

			else
			{

			}

	}

	return CAN_transmission_sucessful;

}

// Error Handler to handle with CAN 1 Errors
void CAN_1_Error_Handler()
{

	if((CAN_1_transmission_error_counter > CAN_1_TRANSMISSION_ERRORS_FAULT_THRESHOLD) && (CAN_1_disabled == false))
	{

		// Disable CAN
		HAL_CAN_Stop(&hcan1);
		HAL_CAN_DeInit(&hcan1);

		CAN_1_disabled = true;

		// Reset variable
		CAN_1_transmission_error_counter = 0;

		// Delay before enabling CAN 1 again
		osDelay(2000);

	}

	else if(CAN_1_disabled == true)
	{

		// Re-enable CAN
		HAL_CAN_Init(&hcan1);
		HAL_CAN_Start(&hcan1);
		CAN_1_disabled = false;

	}

}

// Error Handler to handle with CAN 2 Errors
void CAN_2_Error_Handler()

{

	if((CAN_2_transmission_error_counter > CAN_2_TRANSMISSION_ERRORS_FAULT_THRESHOLD) && (CAN_2_disabled == false))
	{

		// Disable CAN
		HAL_CAN_Stop(&hcan2);
		HAL_CAN_DeInit(&hcan2);

		CAN_2_disabled = true;

		// Reset variable
		CAN_2_transmission_error_counter = 0;

		// Delay before enabling CAN 2 again
		osDelay(2000);

	}

	else if(CAN_2_disabled == true)
	{

		// Re-enable CAN
		HAL_CAN_Init(&hcan2);
		HAL_CAN_Start(&hcan2);
		CAN_2_disabled = false;

	}

}

