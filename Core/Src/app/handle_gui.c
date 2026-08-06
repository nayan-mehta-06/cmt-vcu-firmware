/*
 * handle_gui.c
 *
 *  Created on: 29 Jul 2026
 *      Author: mehta
 */


#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/handle_gui.h"
#include "drivers/usart_driver.h"


void Task_HandleGUI(void *argument)
{
	uint8_t data[] = "Hello world\n";

  /* Infinite loop */
  for(;;)
  {
	  UART_Transmit(data, 12, 1000);
	  osDelay(1000);

    osDelay(1);
  }
}
