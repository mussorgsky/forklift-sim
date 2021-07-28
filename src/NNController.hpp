#include "Controller.hpp"
#include "cppflow/cppflow.h"

class NNController : public Controller
{
    cppflow::model &model;

public:
    NNController(cppflow::model &model) : model(model){};
    float update(ControllerInput const data, float dT) override;
};
