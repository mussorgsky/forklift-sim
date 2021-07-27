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
    RoadSegment() : type(SegmentType::SKIP), length(0.0f), radius(0.0f), degrees(0.0f) {}
    RoadSegment(SegmentType type, float length, float radius = 0.0, float degrees = 0.0)
        : type(type),
          length(length),
          radius(radius),
          degrees(degrees)
    {
    }

    SegmentType type;
    float length;
    float radius;
    float degrees;
};
