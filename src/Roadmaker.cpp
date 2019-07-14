#include "Roadmaker.hpp"

Roadmaker::Roadmaker()
{
    position = Vector2f(0.0f, 0.0f);
    rotation = 0.0f;
}

bool Roadmaker::checkValidity(string segment)
{
    return true;
}

bool Roadmaker::addSegment(string newSegment)
{
    if (checkValidity(newSegment))
    {
        segments.push_back(newSegment);
        return true;
    }
    else
    {
        return false;
    }
}

void Roadmaker::createStraight(string segment, vector<RectangleShape> *shapes)
{
    // "s100"
    float length = stof(segment.substr(1));
    RectangleShape newOne = RectangleShape(Vector2f(length, 1.9f));

    newOne.setFillColor(sf::Color::Black);
    newOne.setOrigin(newOne.getSize() * 0.5f);

    newOne.setPosition(position + help::rotateByDeg(Vector2f(newOne.getSize().x * 0.5f, 0.0f), rotation));
    newOne.setRotation(rotation);

    position += help::rotateByDeg(Vector2f(newOne.getSize().x, 0.0f), rotation);

    shapes->push_back(newOne);
}

void Roadmaker::createTurn(string segment, vector<RectangleShape> *shapes)
{
    // "tl 50 90"
    char direction = segment[1];
    float sign = (direction == 'l' ? -1.0f : 1.0f);

    string::size_type degStart;

    segment = segment.substr(2);

    float radius = stof(segment, &degStart);
    float degrees = stof(segment.substr(degStart));

    const float step = 5.0f;
    int count = degrees / step;
    float length = 2.0f * 3.1415f * radius * (step / 360.0f);

    RectangleShape newOne = RectangleShape(Vector2f(length, 1.9f));
    newOne.setFillColor(sf::Color(16, 16, 16));
    newOne.setOrigin(newOne.getSize() * 0.5f);

    std::cout << rotation << '\t' << degrees << '\t';

    Vector2f turnCenter = Vector2f(position + help::rotateByDeg(Vector2f(radius, 0.0f), rotation + sign * 90.0f));

    for (int i = 0; i < count; i++)
    {
        float x = help::cosineDeg(rotation + sign * (i + 0.5f - 90.0f) * step) * radius;
        float y = help::sineDeg(rotation + sign * (i + 0.5f - 90.0f) * step) * radius;
        //     // float x = help::sineDeg(sign * rotation - sign * (i + 0.5f) * step);
        //     // float y = help::cosineDeg(sign * rotation - sign * (i + 0.5f) * step);
        Vector2f offset = Vector2f(x, y);

        newOne.setPosition(turnCenter + offset); // + offset);
        newOne.setRotation(rotation + sign * (i + 0.5f) * step);

        shapes->push_back(newOne);
    }

    position = turnCenter + help::rotateByDeg(position - turnCenter, sign * degrees);
    rotation += sign * degrees;

    std::cout << rotation << '\n';
}

void Roadmaker::createSkip(string segment)
{
    // "b 100"
    float length = stof(segment.substr(1));

    position = position + help::rotateByDeg(Vector2f(1.0f, 0.0f) * length, rotation);
}

vector<RectangleShape> Roadmaker::createShapes()
{
    vector<RectangleShape> result;

    for (string segment : segments)
    {
        switch (segment[0])
        {
        case 's':
            createStraight(segment, &result);
            break;
        case 't':
            createTurn(segment, &result);
            break;
        case 'b':
            createSkip(segment);
            break;
        }
    }

    return result;
}