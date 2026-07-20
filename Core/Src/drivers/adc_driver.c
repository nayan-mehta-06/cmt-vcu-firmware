/*
 * adc_driver.c
 *
 *  Created on: 18 May 2026
 *      Author: mehta
 */

#include <stdint.h>

#include "drivers/adc_driver.h"

uint16_t Get_ADC_Channel(uint8_t channel)
{
    return raw_ADC_values[channel];
}
