#include "NNController.hpp"

float NNController::update(ControllerInput const data, float const dT)
{
    std::vector<float> input;
    input.insert(input.end(), data.error_memory.begin(), data.error_memory.end());
    input.push_back(data.last_output);
    input.push_back(data.wheel_angle);
    auto output = model(input);
    return output.get_data<float>().front();
}
