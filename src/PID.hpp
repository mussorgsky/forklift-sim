#pragma once
#include "Controller.hpp"

class PID : public Controller
{
private:
  float kP, kI, kD;
  float p, i, d;
  float lastError;

public:
  PID(float kp, float ki, float kd);
  float update(ControllerInput const data, float dT) override;
};
