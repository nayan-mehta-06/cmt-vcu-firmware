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

	uint16_t apps_1_adc;
	uint16_t apps_2_adc;
	uint16_t bse_adc;

} PedalsADC_t;

typedef struct ADCsToLog
{

	uint16_t bpps_1_adc;
	uint16_t bpps_2_adc;
	uint16_t bspd_cs_adc;
	uint16_t sas_adc;
	uint16_t spare_adc;

} ADCsToLog_t;

#define MIN_ADC_VAL 30
#define MAX_ADC_VAL 3070

void Task_Process_ADC(void *argument);

void Clamp_ADC(uint16_t* adc_val);

#endif /* INC_APP_PROCESS_ADC_H_ */
