/*
 * calibration.c
 *
 *  Created on: 18 Jul 2026
 *      Author: mehta
 */


#include <app/calibration.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void Task_Car_Calibration(void *argument)
{
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
}
