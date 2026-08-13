/*
 * adc_driver.h
 *
 *  Created on: 18 May 2026
 *      Author: mehta
 */

#ifndef INC_DRIVERS_ADC_DRIVER_H_
#define INC_DRIVERS_ADC_DRIVER_H_

#include <stdint.h>
#include "main.h"

uint16_t Get_ADC_Channel(uint8_t channel);

// Each channel corresponds to their index in the raw_adc_values array
#define APPS1_ADC_CHANNEL 6
#define APPS2_ADC_CHANNEL 7
#define BSE_ADC_CHANNEL 4
#define BPPS1_ADC_CHANNEL 0
#define BPPS2_ADC_CHANNEL 2
#define BSPD_CS_ADC_CHANNEL 3
#define SAS_ADC_CHANNEL 3
#define SPARE_ADC_CHANNEL 1

#endif /* INC_DRIVERS_ADC_DRIVER_H_ */
