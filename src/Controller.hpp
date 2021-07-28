#pragma once
#include <cmath>
#include <vector>

struct ControllerInput
{
    float error;
    std::vector<float> error_memory;
    float last_output;
    float wheel_angle;
};

class Controller
{
public:
    virtual float update(ControllerInput const, float const) = 0;
};
