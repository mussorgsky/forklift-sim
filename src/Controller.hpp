#pragma once
#include <cmath>

struct ControllerInput
{
    float error;
};

class Controller
{
public:
    virtual float update(ControllerInput const, float const) = 0;
};
