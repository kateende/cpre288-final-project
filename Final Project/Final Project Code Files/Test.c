#include <Final Project Code Files/cyBot_Scan.h>  // For scan sensors
#include <Final Project Code Files/lcd.h>
#include <Final Project Code Files/movement.h>
#include <Final Project Code Files/open_interface.h>
#include <Final Project Code Files/Timer.h>
#include <Final Project Code Files/uart-interrupt.h>
#include <Final Project Code Files/adc.h>
#include <Final Project Code Files/servo.h>
#include <Final Project Code Files/music.h>
#include <Final Project Code Files/ping_template.h>
#include <math.h>

//#warning "Possible unimplemented functions"
//#define UART1_IM_R 0

typedef struct
{
    float sound_dist; // Distance from PING sensor (Scan returns -1.0 if PING is not enabled)
    uint16_t IR_raw_val; // Raw ADC value from IR sensor (Scan returns -1 if IR is not enabled)
} Scan_t;


void main()

{
    //Network password?; cpre288bsk
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data);
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    uart_interrupt_init();
    //
    //uint16_t IR_raw_val;
    //    servo_init();
    adc_init();
    Servo_init();



    char str[10];
    int i;


    while(1) {

        //do things based on what key was typed
        if(enteredKey == 1) {
            uart_sendStr("Scan\n\r");
            runScan(); //this is not the best updated scan version, but it's proof of concept
            enteredKey = NONE;
        }
        else if(enteredKey == FORWARD) {
            uart_sendStr("Forward\n\r");
            enteredKey = NONE;
            move_forward_avoid(sensor_data, 200);
        }
        else if(enteredKey == BACKWARD) {
            uart_sendStr("Backward\n\r");
            enteredKey = NONE;
            move_backward(sensor_data, 200);
        }
        else if(enteredKey == RIGHT) {
            uart_sendStr("Right\n\r");
            enteredKey = NONE;
            turn_right(sensor_data, 5);
        }
        else if(enteredKey == LEFT) {
            uart_sendStr("Left\n\r");
            enteredKey = NONE;
            turn_left(sensor_data, 5);
        }
        else if(enteredKey == RIGHT90) {
            uart_sendStr("Right90\n\r");
            enteredKey = NONE;
            turn_right(sensor_data, 90);
        }
        else if(enteredKey == LEFT90) {
            uart_sendStr("Left90\n\r");
            enteredKey = NONE;
            turn_left(sensor_data, 90);
        }
        else if(enteredKey == FINISH) {
            uart_sendStr("Finish\n\r");
            enteredKey = NONE;
            break;
        }
    }


    oi_free(sensor_data);
}

void runScan() {



    Scan_t scan;



    double IR_cm;
    int i = 0;
    int m;
    char message[250];

    uart_sendStr("Angle(Degrees) Distance(m)\n\r");
    for (i = 0; i <= 180; i += 1)
    {

        Scan(i, &scan);
        m = scan.IR_raw_val;
        IR_cm = (1.63E7) * (pow(m, -1.91));
        sprintf(message, "%d\t%f\n\r", i, (IR_cm / 100.0));
        uart_sendStr(message);
    }


}

void Scan(uint16_t angle, Scan_t *getScan)
{
    Servo_move(angle);
    timer_waitMillis(50);
    uint16_t total=0;
    int n=3;
    int i=0;
    for(i=0;i<n;i++) {
        total+=adc_read();
    }

    getScan->IR_raw_val = total/n;
    //getScan->sound_dist = ping_getDistance();
}
