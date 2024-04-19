/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include <Final Project Code Files/lcd.h>
#include <Final Project Code Files/movement.h>
#include <Final Project Code Files/open_interface.h>
#include <Final Project Code Files/Timer.h>
#include <Final Project Code Files/uart-interrupt.h>
#include <math.h>
#include <Final Project Code Files/adc.h>
#include <Final Project Code Files/servo.h>
#include <Final Project Code Files/ping_template.h>
#include <Final Project Code Files/music.h>

typedef struct
{
    uint32_t sound_dist; // Distance from PING sensor (Scan returns -1.0 if PING is not enabled)
    uint16_t IR_raw_val; // Raw ADC value from IR sensor (Scan returns -1 if IR is not enabled)
} Scan_t;
Scan_t scan;
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
    getScan->sound_dist = ping_getDistance();
}
int main(void)
{

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    adc_init();
    ping_init();
    Servo_init();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);
    int right_calibration_value = 306250;
    int left_calibration_value = 1324750;
   // cyBOT_SERVO_cal();
    Scan_t scan;

    //Play the music
    load_songs(3);

    while (1)
    {
        char scanvalue[50];

        Scan(0, &scan);
        timer_waitMillis(2000);
        //sprintf(scanvalue, "%d\n", scan.IR_raw_val);
        //uart_sendStr(scanvalue);

        typedef struct
        {
            int angle;
            float distance;
            int width;
            float arcwidth;
        } cyBOT_Object_t;

        cyBOT_Object_t objects[50];

        int object_detected = 0;
        int current_object = 0;
        int start_angle = 0;
        int end_angle = 0;
        float min_dist = 0;
        double IR_cm;
        int i = 0;
        int m1 = 0;
        int m2 = 0;
        int m3 = 0;
        int m;
        char message[50];

        for (i = 0; i <= 180; i += 1)
        {
            Scan(i, &scan);
            m = scan.IR_raw_val;
            /*Scan(i, &scan);
             m2 = scan.IR_raw_val;
             Scan(i, &scan);
             m3 = scan.IR_raw_val;*/

            //m = (m1 + m2 + m3) / 3;
            //m = (m1 + m2) / 2;
            IR_cm = raw_to_digital(scan.IR_raw_val);
            sprintf(message, "IR distance: %f\n\r", IR_cm);
            uart_sendStr(message);
            if (!object_detected)
            {
                if (IR_cm < 80)
                {
                    object_detected = 1;
                    start_angle = i;
                    min_dist = IR_cm;

                }
            }
            else
            {
                if (IR_cm < min_dist)
                {
                    min_dist = IR_cm;
                }

                if (IR_cm > 80 || i == 180)
                {

                    object_detected = 0;
                    end_angle = i - 1;
                    objects[current_object].angle = (end_angle + start_angle)/2;
                    objects[current_object].distance = min_dist;
                    objects[current_object].width = end_angle - start_angle;
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
        int obstacle_left=0;
        int obstacle_right=0;
        float perp_distance=0;
        for (j = 0; j < current_object; j++)
        {
            if (objects[j].width > 5)
            {
                /*Scan(objects[j].angle, &scan);
                 timer_waitMillis(2000);
                 Scan(objects[j].angle, &scan);
                 objects[j].distance = scan.sound_dist;
                 Scan(objects[j].angle, &scan);
                 objects[j].distance += scan.sound_dist;
                 Scan(objects[j].angle, &scan);
                 objects[j].distance += scan.sound_dist;
                 objects[j].distance = objects[j].distance/3;
                 */
                objects[j].arcwidth = (2 * 3.1415926535 * objects[j].distance
                        * objects[j].width) / 360;
                perp_distance=cos((objects[j].angle)*3.1415926535/180)*objects[j].distance;


                if((0<=perp_distance)&&(perp_distance<30)) {
                    obstacle_right+=1;
                }
                else if((-30<perp_distance)&&(perp_distance<0)) {
                     obstacle_left+=1;
                }

                sprintf(scanvalue,
                        "Object %d is %f cm away with a width of %f cm and at an angle of %d\n\rPerp distance = %f cm\n\rObstsdcle left:%d\n\rObstsdcle right:%d\n\r",
                        number++, objects[j].distance, objects[j].arcwidth,
                        objects[j].angle, perp_distance,obstacle_left,obstacle_right);

                uart_sendStr(scanvalue);
                if (objects[j].arcwidth < min_width)
                {
                    min_width = objects[j].arcwidth;
                    min_width_index = j;
                }

            }
        }
        /*int final_angle = objects[min_width_index].angle;
         Scan(final_angle, &scan);

         if (final_angle < 90)
         {
         oi_update(sensor_data);
         turn_right(sensor_data, 90 - final_angle);
         }
         else
         {
         oi_update(sensor_data);
         turn_left(sensor_data, final_angle - 90);
         }
         int target = (objects[min_width_index].distance - 8) * 10;*/
        /*
        int distance_traveled = 0;
        if (min_width < 12)
        {
            int final_angle = objects[min_width_index].angle;
            Scan(final_angle, &scan);

            if (final_angle < 90)
            {
                oi_update(sensor_data);
                turn_right(sensor_data, 90 - final_angle);
            }
            else
            {
                oi_update(sensor_data);
                turn_left(sensor_data, final_angle - 90);
            }
            int target = (objects[min_width_index].distance - 8) * 10;
        }
        else if(min_width > 12)
        {
            int bad_angles[10];
            int o = 0;
            for(o = 0; o < current_object; o++)
            {
                bad_angles[j] = objects[j].angle;
            }

        }
        else
        {
            move_forward(sensor_data, 50);
        }
    int target = 100;
    while (distance_traveled < target)
    {
        char str[50];
        sprintf(str, "right: %d, left: %d, frontright: %d, frontleft: %d\n\r",
                sensor_data->cliffRightSignal, sensor_data->cliffLeftSignal,
                sensor_data->cliffFrontRightSignal,
                sensor_data->cliffFrontLeftSignal);
        uart_sendStr(str);

        if (sensor_data->bumpRight)
        {
            move_backward(sensor_data, 150);
            turn_left(sensor_data, 45);
            move_forward(sensor_data, 250);
            turn_right(sensor_data, 45);
            break;
        }

        if (sensor_data->bumpLeft)
        {
            move_backward(sensor_data, 150);
            turn_right(sensor_data, 45);
            move_forward(sensor_data, 250);
            turn_left(sensor_data, 45);
            break;
        }
        if (sensor_data->cliffRight)
        {
            turn_left(sensor_data, 20);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }
        if ((sensor_data->cliffRightSignal) > 2600)
        {
            turn_left(sensor_data, 20);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }

        if (sensor_data->cliffLeft)
        {
            turn_right(sensor_data, 20);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }
        if (sensor_data->cliffLeftSignal > 2600)
        {
            turn_right(sensor_data, 20);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }
        if (sensor_data->cliffFrontRight)
        {
            turn_left(sensor_data, 40);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }
        if (sensor_data->cliffFrontRightSignal > 2600)
        {
            turn_left(sensor_data, 40);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }

        if (sensor_data->cliffFrontLeft)
        {
            turn_right(sensor_data, 40);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }
        if (sensor_data->cliffFrontLeftSignal > 2600)
        {
            turn_right(sensor_data, 40);
            move_forward(sensor_data, 10);
            oi_update(sensor_data);
            break;
        }
        oi_setWheels(100, 100);
        oi_update(sensor_data);
        distance_traveled += sensor_data->distance;
    }
    if (distance_traveled >= target)
    {
        break;
    }
*/

        break;
}

oi_free(sensor_data); // do this once at end of main()
}

