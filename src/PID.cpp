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

float PID::update(ControllerInput const data, float dT)
{
    p = data.error;
    i += data.error * dT;
    d = (data.error - lastError) / dT;

    lastError = data.error;

    return (p * kP) + (i * kI) + (d * kD);
}