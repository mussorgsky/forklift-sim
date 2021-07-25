#include "SegmentDF.hpp"

float SegmentDF::distance(Vector2f const p)
{
    Vector2f const pa = p - a;
    float const h = std::clamp(dot(pa, ba) / dot_ba_ba, 0.0f, 1.0f);
    return help::distance({0.0f, 0.0f}, pa - ba * h);
}
