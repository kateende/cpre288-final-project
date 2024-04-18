/*
 * movement.c
 *
 *  Created on: Jan 30, 2024
 *      Author: edcolsch
 */
#include <Final Project Code Files/open_interface.h>
double move_forward(oi_t *sensor_data, double distance_mm)
{
    double sum = 0; // distance member in oi_t struct is type double
    double angsum = 0;
    oi_setWheels(100, 100); //move forward at full speed
    int adjusted = 0;
    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance;// use -> notation since pointer
        lcd_printf("%f", sensor_data->angle);
        angsum += sensor_data->angle;
       //If the roomba slides to the right
        if(angsum > 2.5) { //slightly
            oi_setWheels(50, 100);
            adjusted = 1;
        }
        else if(angsum > 5) { // not so slightly
            oi_setWheels(25, 100); //go left
            adjusted = 1;
        }
        //slides to the left
        else if (angsum < -2.5) { //slightly
            oi_setWheels(100, 25);
            adjusted = 1;
        }
        else if(angsum < -5) { //not so slightly
            oi_setWheels(100, 25); // go right
            adjusted = 1;
        }
        //if you adjusted and it's corrects, go back to straight

        else if(adjusted == 1) {
            adjusted = 0;
            oi_setWheels(100, 100);
        }

    }
    oi_setWheels(0, 0); //stop

    return 0;
}

double move_backward(oi_t *sensor_data, double distance_mm)
{
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-100, -100); //move forward at full speed
    while (sum > -distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; // use -> notation since pointer
    }
    oi_setWheels(0, 0); //stop

    return 0;
}



double turn_right(oi_t *sensor_data, double degrees)
{
    double sum = 0;
    oi_setWheels(-50,50);
    oi_update(sensor_data);
    while (sum >= -degrees)
    {
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }
    oi_setWheels(0, 0); //stop

    return 0;
}

double turn_left(oi_t *sensor_data, double degrees)
{
    double sum = 0;
    oi_setWheels(50,-50);
    oi_update(sensor_data);
    while (sum <= degrees)
    {
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }
    oi_setWheels(0, 0); //stop

    return 0;
}

double move_forward_avoid(oi_t *sensor_data, double distance_mm) {
    double sum = 0;
    oi_setWheels(100, 100);
    while(sum < distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;

        //straighten out
        angsum += sensor_data->angle;
        //If the roomba slides to the right
        if(angsum > 2.5) { //slightly
            oi_setWheels(50, 100);
            adjusted = 1;
        }
        else if(angsum > 5) { // not so slightly
            oi_setWheels(25, 100); //go left
            adjusted = 1;
        }
        //slides to the left
        else if (angsum < -2.5) { //slightly
            oi_setWheels(100, 25);
            adjusted = 1;
        }
        else if(angsum < -5) { //not so slightly
            oi_setWheels(100, 25); // go right
            adjusted = 1;
        }
        //if you adjusted and it's corrects, go back to straight

        else if(adjusted == 1) {
            adjusted = 0;
            oi_setWheels(100, 100);
        }

        //object detect
        if(sensor_data -> bumpRight) {
            oi_setWheels(0, 0);
            move_backward(sensor_data, 150);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_right(sensor_data, 90);

        }
        if(sensor_data -> bumpLeft) {
            oi_setWheels(0, 0);
            move_backward(sensor_data, 150);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_left(sensor_data, 90);
        }

        if ((sensor_data->cliffRightSignal) > 2600) {
            turn_left(sensor_data, 20);
            move_forward(sensor_data, 10);
            break;
        }

        if (sensor_data->cliffLeftSignal > 2600) {
            turn_right(sensor_data, 20);
            move_forward(sensor_data, 10);
            break;
        }
        if (sensor_data->cliffFrontRightSignal > 2600) {
            turn_left(sensor_data, 40);
            move_forward(sensor_data, 10);
            break;
        }

        if (sensor_data->cliffFrontLeftSignal > 2600) {
            turn_right(sensor_data, 40);
            move_forward(sensor_data, 10);
            break;
        }

        oi_setWheels(100,100);

    }
    oi_setWheels(0, 0);
    return sum;
}
