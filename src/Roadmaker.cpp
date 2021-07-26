#include <random>
#include <algorithm>

#include "Roadmaker.hpp"
#include "SegmentDF.hpp"
#include "ArcDF.hpp"

void Roadmaker::addSegment(RoadSegment newSegment)
{
    road_segments.push_back(newSegment);
}

void Roadmaker::createStraight(RoadSegment const &segment)
{
    // Based on the length of the segment and the maximum path-point distance,
    // find the number of path points and the distance between them.
    // None of the segments adds their first point to the path, as it's the last
    // point of the previous segment.
    // The (0, 0) starting point of the path is a special case added elsewhere.

    int const fits_in_times = std::ceil(segment.length / max_path_distance);
    for (int i = 1; i <= fits_in_times; ++i)
    {
        float k = ((float)i / (float)fits_in_times);
        road_path.push_back(position + help::rotateByDeg({segment.length * k, 8.0f}, rotation));
        road_path.push_back(position + help::rotateByDeg({segment.length * k, -8.0f}, rotation));
    }

    Vector2f sdf_start = position;

    vector<Vertex> new_vertices;
    new_vertices.push_back(sf::Vertex(position + help::rotateByDeg({road_width * 0.5f, 0.0f}, rotation + 90.0f), sf::Color::Black));
    new_vertices.push_back(sf::Vertex(position + help::rotateByDeg({road_width * -0.5f, 0.0f}, rotation + 90.0f), sf::Color::Black));

    position += help::rotateByDeg({segment.length, 0.0f}, rotation);

    new_vertices.push_back(sf::Vertex(position + help::rotateByDeg({road_width * 0.5f, 0.0f}, rotation + 90.0f), sf::Color::Black));
    new_vertices.push_back(sf::Vertex(position + help::rotateByDeg({road_width * -0.5f, 0.0f}, rotation + 90.0f), sf::Color::Black));

    road_vertices.push_back(new_vertices);

    distance_fields.push_back(std::make_shared<SegmentDF>(sdf_start, position));
}

void Roadmaker::createSkip(RoadSegment const &segment)
{
    position += help::rotateByDeg(Vector2f(1.0f, 0.0f) * segment.length, rotation);
}

void Roadmaker::createTurn(RoadSegment const &segment)
{
    float const sign = (segment.type == SegmentType::TURN_LEFT) ? -1.0f : 1.0f;
    Vector2f const turnCenter = Vector2f(position + help::rotateByDeg(Vector2f(segment.radius, 0.0f), rotation + sign * 90.0f));

    int const fits_in_times = std::ceil(2.0f * 3.1415 * segment.radius * (segment.degrees / 360.0f) / max_path_distance);
    for (int i = 1; i <= fits_in_times; ++i)
    {
        float k = ((float)i / (float)fits_in_times);
        float angle = rotation + sign * (k * segment.degrees - 90.0f);
        road_path.push_back(turnCenter + help::rotateByDeg({segment.radius + 8.0f, 0.0f}, angle));
        road_path.push_back(turnCenter + help::rotateByDeg({segment.radius - 8.0f, 0.0f}, angle));
    }

    vector<Vertex> new_vertices;
    int const fits_in_times_visual = std::ceil(2.0f * 3.1415 * segment.radius * (segment.degrees / 360.0f) / max_visual_distance);
    for (int i = 0; i <= fits_in_times_visual; ++i)
    {
        float k = ((float)i / (float)fits_in_times_visual);
        float angle = rotation + sign * (k * segment.degrees - 90.0f);
        new_vertices.push_back({turnCenter + help::rotateByDeg({segment.radius + road_width * 0.5f, 0.0f}, angle), sf::Color::Black});
        new_vertices.push_back({turnCenter + help::rotateByDeg({segment.radius - road_width * 0.5f, 0.0f}, angle), sf::Color::Black});
    }

    if (segment.type == SegmentType::TURN_RIGHT)
    {
        distance_fields.push_back(std::make_shared<ArcDF>(turnCenter, segment.radius, rotation, segment.degrees));
    }

    position = turnCenter + help::rotateByDeg(position - turnCenter, sign * segment.degrees);
    rotation += sign * segment.degrees;

    road_vertices.push_back(new_vertices);

    if (segment.type == SegmentType::TURN_LEFT)
    {
        distance_fields.push_back(std::make_shared<ArcDF>(turnCenter, segment.radius, rotation + 180.0f, segment.degrees));
    }
}

void Roadmaker::createShapes()
{
    road_path.push_back(position + help::rotateByDeg({0.0f, 8.0f}, rotation));
    road_path.push_back(position + help::rotateByDeg({0.0f, -8.0f}, rotation));
    for (auto &segment : road_segments)
    {
        switch (segment.type)
        {
        case SegmentType::STRAIGHT:
            createStraight(segment);
            break;

        case SegmentType::SKIP:
            createSkip(segment);
            break;

        case SegmentType::TURN_RIGHT:
        case SegmentType::TURN_LEFT:
            createTurn(segment);
            break;
        }
    }
    finish_point = position;
}

void Roadmaker::generateSegments(RoadConfig const config)
{
    reset();

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

Vector2f Roadmaker::getFinishPoint()
{
    return finish_point;
}

vector<std::shared_ptr<DistanceField>> const &Roadmaker::getDistanceFields()
{
    return distance_fields;
}

vector<vector<Vertex>> const &Roadmaker::getRoadVertices()
{
    return road_vertices;
}

vector<Vector2f> const &Roadmaker::getRoadPath()
{
    return road_path;
}

void Roadmaker::reset()
{
    road_segments.clear();
    distance_fields.clear();
    for (auto &v : road_vertices)
    {
        v.clear();
    }
    road_vertices.clear();
    road_path.clear();
    position = Vector2f(0.0f, 0.0f);
    rotation = 0.0f;
}
