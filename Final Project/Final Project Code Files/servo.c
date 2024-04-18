/*
 * servo.c
 *
 *  Created on: Apr 2, 2024
 *      Author: edcolsch
 */

#include "servo.h"
#include "Timer.h"

void Servo_init(void)
{
    SYSCTL_RCGCTIMER_R |= 0x2;
    while ((SYSCTL_PRTIMER_R & 0x2) == 0) {};
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;

    TIMER1_CTL_R &= ~(0x100);
    TIMER1_CFG_R |= 0x4;
    TIMER1_TBMR_R |= 0x8;
    TIMER1_TBMR_R &= ~(0x5);
    TIMER1_TBMR_R |= 0x2;
    TIMER1_CTL_R &= ~(0x4000);
    TIMER1_TBPR_R = 0x04;
    TIMER1_TBILR_R = 0xE200;
    TIMER1_TBPMR_R = 0x04;
    TIMER1_TBMATCHR_R = 0x5D30;
    TIMER1_CTL_R |= 0x100;
}

void Servo_move(uint16_t degrees)
{
    TIMER1_CTL_R &= ~(0x100);
    TIMER1_TBMATCHR_R = (312000 + (283000 - 312000)*degrees/180);
    TIMER1_CTL_R |= 0x100;


}
