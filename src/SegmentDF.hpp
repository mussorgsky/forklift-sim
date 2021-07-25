#pragma once

#include <algorithm>

#include "Help.hpp"
#include "DistanceField.hpp"

using help::dot;

class SegmentDF : public DistanceField
{
    Vector2f const a;
    Vector2f const b;
    Vector2f const ba;
    float const dot_ba_ba;

public:
    SegmentDF(Vector2f a, Vector2f b) : a(a), b(b), ba(b - a), dot_ba_ba(dot(ba, ba)) {}
    float distance(Vector2f const p) override;
};
