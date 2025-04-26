/*
 * battery.c
 *
 *  Created on: Apr 13, 2025
 *      Author: chili
 */

#include "battery.h"

#define ADC_BUFFER_SIZE 50

extern ADC_HandleTypeDef hadc1;
extern uint16_t ADC_value;
extern uint16_t ADC_value_f;
uint16_t adc_buffer[ADC_BUFFER_SIZE];
uint8_t index = 0;
float filteredValue = 0.0;
float alpha = 0.9;

float Convert_To_Voltage(uint16_t adc_val) {
	float v_adc = (adc_val * 3.3f) / 4095.0f;
	float v_batt = v_adc * ((10.0f + 4.7f) / 4.7f);
	return v_batt;
}

uint16_t Filter_ADC(uint16_t newValue) {
	adc_buffer[index++] = newValue;
	 if (index >= ADC_BUFFER_SIZE) index = 0;

	 uint32_t sum = 0;
	 for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
	 sum += adc_buffer[i];
	 }
	 return sum / ADC_BUFFER_SIZE;

	/*filteredValue = alpha * newValue + (1 - alpha) * filteredValue;
	return (uint16_t) filteredValue;*/
}

uint8_t Get_Battery_Charge(void) {
	const float MIN_VOLTAGE = 5.4;
	const float MAX_VOLTAGE = 8.2;
	float voltage = 0.0f;
	uint8_t percent = 0;

	voltage = Convert_To_Voltage(ADC_value_f);

	if (voltage <= MIN_VOLTAGE)
		percent = 0;
	else if (voltage >= MAX_VOLTAGE)
		percent = 100;
	else
		percent = (uint8_t) ((voltage - MIN_VOLTAGE)
				/ (MAX_VOLTAGE - MIN_VOLTAGE) * 100.0);

	return percent;
}
