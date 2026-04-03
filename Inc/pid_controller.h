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
    // PID gains
    float Kp;  // Proportional gain
    float Ki;  // Integral gain
    float Kd;  // Derivative gain

    // PID state
    float setpoint;           // Desired temperature (°C)
    float integral;           // Accumulated error
    float previous_error;     // Last error for derivative

    // Output limits
    float output_min;
    float output_max;

    // Anti-windup
    float integral_max;

    // Timing
    float sample_time;  // seconds between updates
} PID_Controller;

// Function prototypes
void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float sample_time);
void PID_SetLimits(PID_Controller *pid, float min, float max);
void PID_SetSetpoint(PID_Controller *pid, float setpoint);
float PID_Update(PID_Controller *pid, float measured_value);
void PID_Reset(PID_Controller *pid);

#endif
