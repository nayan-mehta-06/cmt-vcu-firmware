/*
 * inverter_control.h
 *
 *  Created on: Feb 2, 2026
 *      Author: mehta
 */

#ifndef INC_INVERTER_CONTROL_H_
#define INC_INVERTER_CONTROL_H_

#include <stdint.h>
#include <stdbool.h>
#include "config/amk_data_structs.h"

extern InverterSetpoints_t* p_inverter_setpoints_1;

#define AMK_STATUS_SYSTEM_READY_MASK       	(1U << AMK_STATUS_SYSTEM_READY)
#define AMK_STATUS_ERROR_ACTIVE_MASK        (1U << AMK_STATUS_ERROR)
#define AMK_STATUS_WARNING_ACTIVE_MASK      (1U << AMK_STATUS_WARN)
#define AMK_STATUS_QUIT_DC_ON_MASK          (1U << AMK_STATUS_QUIT_DC_ON)
#define AMK_STATUS_DC_ON_MASK               (1U << AMK_STATUS_DC_ON)
#define AMK_STATUS_QUIT_INVERTER_ON_MASK    (1U << AMK_STATUS_QUIT_INVERTER_ON)
#define AMK_STATUS_INVERTER_ON_MASK         (1U << AMK_STATUS_INVERTER_ON)
#define AMK_STATUS_DERATING_ACTIVE_MASK     (1U << AMK_STATUS_DERATING)

void setInverterData1(InverterData_t *inv_data, uint8_t *data);
void processInverterStatus(InverterStatus_t *inv_status, uint16_t status);
void setInverterData2(InverterData_t *inv_data, uint8_t *data);

bool transmit_inverter_command(InverterSetpoints_t *p_inverter_setpoints, uint8_t inverter_node_number);

#endif /* INC_INVERTER_CONTROL_H_ */
