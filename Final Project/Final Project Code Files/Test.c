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
    uint32_t sound_dist; // Distance from PING sensor (Scan returns -1.0 if PING is not enabled)
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

    typedef struct
    {
        int angle;
        float distance;
        int width;
        float arcwidth;
    } cyBOT_Object_t;

    Scan_t scan;

    cyBOT_Object_t objects[50];
    memset(objects, 0, sizeof objects);

    int object_detected = 0;
    int current_object = 0;
    int start_angle = 0;
    int end_angle = 0;
    float min_dist = 0;
    double IR_cm;
    int i = 0;
    int m;
    char message[250];
    int cutoff = 0;
    char scanvalue[250];


    for (i = 0; i <= 180; i += 1)
    {

        Scan(i, &scan);
        m = scan.IR_raw_val;
        IR_cm = (1.63E7) * (pow(m, -1.91));
        sprintf(message, "$d\t%f\n\r", i, IR_cm);
        uart_sendStr(message);
        if (!object_detected)
        {
            if (IR_cm < 60)
            {
                if(i == 0){
                    cutoff == 1;
                }
                object_detected = 1;
                start_angle = i;
                min_dist = IR_cm;

            }
        }
        // Objects cutoff on edges are not being detected properly
        else
        {
            if (IR_cm < min_dist)
            {
                min_dist = IR_cm;
            }

            if (IR_cm > 60 || i == 180)
            {

                object_detected = 0;
                end_angle = i - 1;
                objects[current_object].angle = (end_angle + start_angle)
                                        / 2;
                objects[current_object].distance = min_dist;
                if(cutoff == 1){
                    objects[current_object].width = 50;
                }
                else{
                    objects[current_object].width = end_angle - start_angle;
                }
                if (i == 180)
                {
                    objects[current_object].width = 300;
                }
                current_object += 1;
            }
        }
    }

    int j = 0;
    int number = 1;
    int min_width = 999999;
    int min_width_index = 0;
    int obstacle_left = 0;
    int obstacle_right = 0;
    float perp_distance[50];
    memset(perp_distance, 0, sizeof perp_distance);
    for (j = 0; j < current_object; j++)
    {
        if (objects[j].width > 5)
        {
            objects[j].arcwidth = (2 * 3.1415926535 * objects[j].distance
                    * objects[j].width) / 360;
            // Width is not accurate atm, could be calibration issue
            // Test objects are not meeting specs given in problem statement (Problem statement specifies 6 in, currently at 4.5 in)
            perp_distance[j] = cos((objects[j].angle) * 3.1415926535 / 180)* objects[j].distance;

            sprintf(scanvalue,
                    "Object %d is %f cm away with a width of %f cm and at an angle of %d\nPerp distance = %f cm\nObstacle left:%d\nObstacle right:%d\n\r",
                    number++, objects[j].distance, objects[j].arcwidth,
                    objects[j].angle, perp_distance[j], obstacle_left,
                    obstacle_right);

            uart_sendStr(scanvalue);
            if ((objects[j].arcwidth) < min_width)
            {
                min_width = objects[j].arcwidth;
                min_width_index = j;
            }
        }
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
   // getScan->sound_dist = ping_getDistance();
}
