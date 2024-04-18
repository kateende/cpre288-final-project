#include <Final Project Code Files/cyBot_Scan.h>  // For scan sensors
#include <Final Project Code Files/lcd.h>
#include <Final Project Code Files/movement.h>
#include <Final Project Code Files/open_interface.h>
#include <Final Project Code Files/Timer.h>
#include <Final Project Code Files/uart-interrupt.h>
#include <Final Project Code Files/adc.h>
#include <Final Project Code Files/servo.h>
#include <math.h>

//#warning "Possible unimplemented functions"
//#define UART1_IM_R 0
void main()
{
    oi_t *senso_data = oi_alloc(); // do this only once at start of main()
    oi_init(senso_data);
    timer_init(); // Must be called before lcd_init(), which uses timer functions
   lcd_init();
//
    //uint16_t IR_raw_val;
   // servo_init();
   // adc_init();



   move_forward(senso_data, 1100);
    turn_right(senso_data, 90);
  move_forward(senso_data, 100);
  turn_left(senso_data, 90);


    oi_free(senso_data);
}
