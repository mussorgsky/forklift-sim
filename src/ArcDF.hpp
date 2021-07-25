#pragma once

#include <algorithm>

#include "Help.hpp"
#include "DistanceField.hpp"

class ArcDF : public DistanceField
{
    float const ra;
    Vector2f const origin;
    Vector2f const sca;
    Vector2f const scb;

public:
    ArcDF(Vector2f position, float radius, float start_angle, float span)
        : ra(radius), origin(position),
          sca(help::sincosDeg(start_angle - 90.0f + span * 0.5f)),
          scb(help::sincosDeg(span * 0.5f)) {}
    float distance(Vector2f const p) override;
};
