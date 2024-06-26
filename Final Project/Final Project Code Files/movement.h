/*
 * movement.h
 *
 *  Created on: Jan 30, 2024
 *      Author: edcolsch
 */
#include <Final Project Code Files/open_interface.h>
#ifndef MOVEMENT_H_
#define MOVEMENT_H_
double move_forward(oi_t *sensor_data, double distance_mm);
double move_backward(oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor_data, double degrees);
double turn_left(oi_t *sensor_data, double degrees);
double move_forward_avoid(oi_t *sensor_data, double distance_mm);



#endif /* MOVEMENT_H_ */
