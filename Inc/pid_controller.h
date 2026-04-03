/*
 * pid_controller.h
 *
 *  Created on: Mar 23, 2026
 *      Author: Austi
 */

#ifndef INC_PID_CONTROLLER_H_
#define INC_PID_CONTROLLER_H_

#include <stdint.h>

typedef struct {
    //PID vars
    float Kp;  //Proportional var
    float Ki;  //Integral var
    float Kd;  //Derivative var

    //State
    float setpoint;           //Desired temp var
    float integral;           //Error var
    float previous_error;     //Prev error var

    //output var
    float output_min;
    float output_max;

    //anti-windup
    float integral_max;

    //Time
    float sample_time;  //update time
} PID_Controller;

//prototypes for functions
void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float sample_time);
void PID_SetLimits(PID_Controller *pid, float min, float max);
void PID_SetSetpoint(PID_Controller *pid, float setpoint);
float PID_Update(PID_Controller *pid, float measured_value);
void PID_Reset(PID_Controller *pid);

#endif
