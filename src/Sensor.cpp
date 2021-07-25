#include <limits>

#include "Sensor.hpp"

Sensor::Sensor()
{
    spacing = 1.9f;
    position = 4.0f;

    samplePoints = makeSamplePoints();

    for (int i = 0; i < width; i++)
    {
        dots[i].setFillColor(sf::Color::Black);
        dots[i].setOutlineColor(sf::Color::Black);
        dots[i].setRadius(1.5f);
        dots[i].setOutlineThickness(-0.5f);
        dots[i].setOrigin(Vector2f(1.5f, 1.5f));
    }
}

void Sensor::setParentInfo(Vector2f position, float rotation)
{
    parentPos = position;
    parentRot = rotation;
}

array<Vector2f, Sensor::width> Sensor::makeSamplePoints()
{
    array<Vector2f, width> result;
    bool isOdd = (width % 2 == 1);
    float posX;

    if (isOdd)
    {
        posX = (width - 1) / 2 * -spacing;
    }
    else
    {
        posX = width / 2 * -spacing + spacing * 0.5f;
    }

    for (Vector2f &v : result)
    {
        v.x = posX;
        v.y = position;
        posX += spacing;
    }

    return result;
}

float Sensor::sense(float last)
{
    bool overLine = false;
    int seen = 0;
    float result = 0.0f;
    for (int i = 0; i < width; i++)
    {
        Vector2f dotPos = parentPos + help::rotateByDeg(samplePoints[i], parentRot);
        dots[i].setFillColor(sf::Color::Black);

        float distance = std::numeric_limits<float>::max();
        for (auto &df : distance_fields)
        {
            distance = std::min(distance, df->distance(dotPos));
        }
        if (distance <= 1.9f * 0.5f)
        {
            result += samplePoints[i].x;
            dots[i].setFillColor(sf::Color::White);
            overLine = true;
            seen += 1;
        }
    }

    if (abs(last) < 0.01f)
    {
        last = 0.0f;
    }
    else
    {
        last = std::copysignf(5.5f, last);
    }

    return overLine ? result / seen : last;
}
