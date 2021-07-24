#pragma once

enum class SegmentType
{
    STRAIGHT,
    SKIP,
    TURN_RIGHT,
    TURN_LEFT
};

class RoadSegment
{
public:
    RoadSegment(SegmentType type, float length, float radius = 0.0, float degrees = 0.0)
        : type(type),
          length(length),
          radius(radius),
          degrees(degrees)
    {
    }

    const SegmentType type;
    const float length;
    const float radius;
    const float degrees;
};
