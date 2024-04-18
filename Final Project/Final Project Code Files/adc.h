/*
 * adc.h
 *
 *  Created on: Mar 19, 2024
 *      Author: edcolsch
 */

#ifndef LAB10_CODE_FILES_ADC_H_
#define LAB10_CODE_FILES_ADC_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include <Final Project Code Files/Timer.h>

void adc_init(void);
uint16_t adc_read(void);



#endif /* LAB10_CODE_FILES_ADC_H_ */
