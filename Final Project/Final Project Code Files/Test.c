//#include <Final Project Code Files/cyBot_Scan.h>  // For scan sensors
//#include <Final Project Code Files/lcd.h>
//#include <Final Project Code Files/movement.h>
//#include <Final Project Code Files/open_interface.h>
//#include <Final Project Code Files/Timer.h>
//#include <Final Project Code Files/uart-interrupt.h>
//#include <Final Project Code Files/adc.h>
//#include <math.h>
//
////#warning "Possible unimplemented functions"
////#define UART1_IM_R 0
//int main(void)
//{
//
//    timer_init(); // Must be called before lcd_init(), which uses timer functions
//    lcd_init();
//    uint16_t IR_raw_val;
//
//    adc_init();
//
//        char str[10];
//        int i;
//
//
//
//        sprintf(str, "SCANS:\n\r");
//       uart_sendStr(str);
//        for(i = 0; i < 10; i++) {
//            uint16_t scan = adc_read();
//            int dist = raw_to_digital(scan);
//            sprintf(str, "Raw: %d,Calc'd: %d", scan, dist);
//           uart_sendStr(str);
//            lcd_printf(str);
//        }
//
//}
