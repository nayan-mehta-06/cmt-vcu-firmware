/*
 * calibration.h
 *
 *  Created on: 18 Jul 2026
 *      Author: mehta
 */

#ifndef INC_APP_CALIBRATION_H_
#define INC_APP_CALIBRATION_H_

#define CALIBRATION_DATA_READY (1U << 0)
#define CALIBRATION_DATA_RECEIVED (1U << 1)

void Task_Car_Calibration(void *argument);

#endif /* INC_APP_CALIBRATION_H_ */
