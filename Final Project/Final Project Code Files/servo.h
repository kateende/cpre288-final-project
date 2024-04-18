/*
 * servo.h
 *
 *  Created on: Apr 2, 2024
 *      Author: edcolsch
 */

#ifndef LAB10_CODE_FILES_SERVO_H_
#define LAB10_CODE_FILES_SERVO_H_
#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void Servo_init(void);
void Servo_move(uint16_t degrees);


#endif /* LAB10_CODE_FILES_SERVO_H_ */
