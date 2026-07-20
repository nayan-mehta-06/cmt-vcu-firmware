/*
 * process_ADC.h
 *
 *  Created on: 15 Jul 2026
 *      Author: mehta
 */

#ifndef INC_APP_PROCESS_ADC_H_
#define INC_APP_PROCESS_ADC_H_

#include <stdint.h>

typedef struct PedalsADC
{

	uint16_t apps1_adc;
	uint16_t apps2_adc;
	uint16_t bse_adc;

} PedalsADC_t;

void Task_Process_ADC(void *argument);

void Clamp_ADC(uint16_t* adc_val);

#endif /* INC_APP_PROCESS_ADC_H_ */
