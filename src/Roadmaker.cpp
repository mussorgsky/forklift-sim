#include "Roadmaker.hpp"

Roadmaker::Roadmaker()
{
    position = Vector2f(0.0f, 0.0f);
    rotation = 0.0f;
}

void Roadmaker::addSegment(RoadSegment newSegment)
{
    road_segments.push_back(newSegment);
}

void Roadmaker::createStraight(RoadSegment const &segment, vector<RectangleShape> &shapes)
{
    RectangleShape shape = RectangleShape(Vector2f(segment.length, 1.9f));
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(shape.getSize() * 0.5f);
    shape.setPosition(position + help::rotateByDeg(Vector2f(shape.getSize().x * 0.5f, 0.0f), rotation));
    shape.setRotation(rotation);
    position += help::rotateByDeg(Vector2f(shape.getSize().x, 0.0f), rotation);

    shapes.push_back(shape);
}

void Roadmaker::createSkip(RoadSegment const &segment)
{
    position += help::rotateByDeg(Vector2f(1.0f, 0.0f) * segment.length, rotation);
}

void Roadmaker::createTurn(RoadSegment const &segment, vector<RectangleShape> &shapes)
{
    float const sign = (segment.type == SegmentType::TURN_LEFT) ? -1.0f : 1.0f;

    float const step = 5.0f;
    int const count = segment.degrees / step;
    float const length = 2.0f * 3.1415f * segment.radius * (step / 360.0f);

    RectangleShape newOne = RectangleShape(Vector2f(length, 1.9f));
    newOne.setFillColor(sf::Color(16, 16, 16));
    newOne.setOrigin(newOne.getSize() * 0.5f);

    Vector2f const turnCenter = Vector2f(position + help::rotateByDeg(Vector2f(segment.radius, 0.0f), rotation + sign * 90.0f));

    for (int i = 0; i < count; i++)
    {
        float x = help::cosineDeg(rotation + sign * (i + 0.5f - 90.0f) * step) * segment.radius;
        float y = help::sineDeg(rotation + sign * (i + 0.5f - 90.0f) * step) * segment.radius;
        Vector2f offset = Vector2f(x, y);

        newOne.setPosition(turnCenter + offset);
        newOne.setRotation(rotation + sign * (i + 0.5f) * step);

        shapes.push_back(newOne);
    }

    position = turnCenter + help::rotateByDeg(position - turnCenter, sign * segment.degrees);
    rotation += sign * segment.degrees;
}

vector<RectangleShape> Roadmaker::createShapes()
{
    vector<RectangleShape> result;
    for (auto &segment : road_segments)
    {
        switch (segment.type)
        {
        case SegmentType::STRAIGHT:
            createStraight(segment, result);
            break;

        case SegmentType::SKIP:
            createSkip(segment);
            break;

        case SegmentType::TURN_RIGHT:
        case SegmentType::TURN_LEFT:
            createTurn(segment, result);
            break;
        }
    }
    return result;
}
