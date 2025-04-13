/*
 * battery.c
 *
 *  Created on: Apr 13, 2025
 *      Author: chili
 */

#include "battery.h"

extern ADC_HandleTypeDef hadc1;

uint16_t Read_ADC(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t val = HAL_ADC_GetValue(&hadc1);
    return val;
}

float Convert_To_Voltage(uint16_t adc_val)
{
    float v_adc = (adc_val * 3.3f) / 4095.0f;
    float v_batt = v_adc * ((10.0f + 4.7f) / 4.7f);
    return v_batt;
}

uint8_t Get_Battery_Charge(void)
{
	float voltage = 0.0f;
	uint8_t percent = 0;

	voltage = Convert_To_Voltage(Read_ADC());

	if (voltage <= 5.0)
		percent = 0;
	else if (voltage >= 8.0)
		percent = 100;
	else
		percent = (uint8_t)((voltage - 5.0) / (8.0 - 5.0) * 100.0);

	return percent;
}
