#include <random>
#include <algorithm>

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
    unsigned int const pieces_count = segment.length / 1.0f;
    float const piece_length = segment.length / pieces_count;
    for (unsigned int i = 0; i < pieces_count; ++i)
    {
        RectangleShape shape = RectangleShape(Vector2f(piece_length / 2.0f, 1.9f));
        shape.setFillColor(sf::Color::Black);
        shape.setOrigin(shape.getSize() * 0.5f);
        shape.setPosition(position + help::rotateByDeg(Vector2f(shape.getSize().x * 0.5f, 0.0f), rotation));
        shape.setRotation(rotation);
        position += help::rotateByDeg(Vector2f(shape.getSize().x, 0.0f), rotation);

        shapes.push_back(shape);
    }
}

void Roadmaker::createSkip(RoadSegment const &segment)
{
    position += help::rotateByDeg(Vector2f(1.0f, 0.0f) * segment.length, rotation);
}

void Roadmaker::createTurn(RoadSegment const &segment, vector<RectangleShape> &shapes)
{
    float const sign = (segment.type == SegmentType::TURN_LEFT) ? -1.0f : 1.0f;

    float const step = 1.0f;
    int const count = segment.degrees / step;
    float const length = 2.0f * 3.1415f * segment.radius * (step / 360.0f);

    RectangleShape newOne = RectangleShape(Vector2f(length, 1.9f));
    newOne.setFillColor(sf::Color::Black);
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
    position = Vector2f(0.0f, 0.0f);
    rotation = 0.0f;
    road_segments.clear();

    return result;
}

void Roadmaker::generateSegments(RoadConfig const config)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> radius(config.radius_min, config.radius_max);
    std::uniform_real_distribution<float> length(config.length_min, config.length_max);
    std::uniform_real_distribution<float> angle(config.angle_min, config.angle_max);

    if (config.lead_in)
    {
        road_segments.push_back(RoadSegment(SegmentType::STRAIGHT, config.lead_length));
    }

    for (unsigned int i = 0; i < config.turns_left; ++i)
    {
        road_segments.push_back(RoadSegment(SegmentType::TURN_LEFT, 0.0f, radius(engine), angle(engine)));
    }
    for (unsigned int i = 0; i < config.turns_right; ++i)
    {
        road_segments.push_back(RoadSegment(SegmentType::TURN_RIGHT, 0.0f, radius(engine), angle(engine)));
    }
    for (unsigned int i = 0; i < config.straights; ++i)
    {
        road_segments.push_back(RoadSegment(SegmentType::STRAIGHT, length(engine)));
    }
    for (unsigned int i = 0; i < config.skips; ++i)
    {
        road_segments.push_back(RoadSegment(SegmentType::SKIP, length(engine)));
    }
    std::random_shuffle(road_segments.begin() + (config.lead_in ? 1 : 0), road_segments.end());

    if (config.lead_out)
    {
        road_segments.push_back(RoadSegment(SegmentType::STRAIGHT, config.lead_length));
    }
}
