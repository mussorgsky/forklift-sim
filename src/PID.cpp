#include "PID.hpp"

PID::PID(float kp, float ki, float kd)
{
    kP = kp;
    kI = ki;
    kD = kd;

    p = 0.0f;
    i = 0.0f;
    d = 0.0f;

    lastError = 0.0f;
}

float PID::update(float error, float dT)
{
    p = error;
    i += error * dT;
    d = (error - lastError) / dT;

    lastError = error;

    return (p * kP) + (i * kI) + (d * kD);
}