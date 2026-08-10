/*
 * can_ids.h
 *
 *  Created on: 14 Jul 2026
 *      Author: mehta
 */

#ifndef INC_CONFIG_CAN_IDS_H_
#define INC_CONFIG_CAN_IDS_H_

#define BMS_ID 0xF2
#define IVTS_U1 0x522
#define IVTS_U2 0x523

#define AMK_ACTUAL_VALUES_1 0x282
#define AMK_ACTUAL_VALUES_2 0x284
#define AMK_SETPOINTS_1 0x183

#define AMK_RECEIVE_MSG_1 0x284
#define AMK_RECEIVE_MSG_2 0x286
#define AMK_SEND_MSG_1 0x185

#define INVERTER_1_NODE_ADDRESS 0x0
#define INVERTER_2_NODE_ADDRESS 0x4

#define CAN_2_INV_DATA_1 0x1A4
#define CAN_2_INV_DATA_2 0X1A5
#define CAN_2_VCU_DATA_1 0x1AE
#define CAN_2_VCU_DATA_2 0xAEF
#define CAN_2_VCU_DATA_3 0x1B0

#endif /* INC_CONFIG_CAN_IDS_H_ */
