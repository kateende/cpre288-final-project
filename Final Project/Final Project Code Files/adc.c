/*
 * adc.c
 *
 *  Created on: Mar 19, 2024
 *      Author: edcolsch
 */

#include <Final Project Code Files/adc.h>
#include <math.h>

void adc_init(void)
{
    SYSCTL_RCGCADC_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_PRGPIO_R&0x02) != 0x02){};
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    ADC0_SSPRI_R = 0x0123;
    ADC0_ACTSS_R &= ~0x0008;
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10;
    ADC0_SSCTL3_R = 0x0006;
    ADC0_IM_R &= ~0x0008;
    ADC0_ACTSS_R |= 0x0008;
}

uint16_t adc_read(void)
{
    uint16_t result;
    ADC0_PSSI_R = 0x0008;
    while((ADC0_RIS_R&0x08)==0){};
    result = ADC0_SSFIFO3_R&0xFFF;
    ADC0_ISC_R = 0x0008;
    return result;
}

int raw_to_digital(uint16_t raw_ir) {

    //cybot 7: 2940000 * pow(raw_ir, -1.6);
    //b1's cybot: 20000000 * (pow(raw_ir, -1.882))
    //vybot 3: 8000000 * (pow(raw_ir, -1.71))
    return 8000000 * (pow(raw_ir, -1.71));


}
