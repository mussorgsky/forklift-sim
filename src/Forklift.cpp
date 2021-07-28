#include "Forklift.hpp"

Forklift::Forklift()
{
    position = Vector2f(0.0f, 0.0f);
    rotation = 0;
    steeringTarget = 0.0f;

    Vector2f frontSize = Vector2f(1.36f, 8.16f);
    Vector2f frontPos1 = Vector2f(6.4f, 0.0f);
    Vector2f frontPos2 = Vector2f(-6.4f, 0.0f);

    Vector2f rearSize = Vector2f(2.1f, 4.32f);
    Vector2f rearPos = Vector2f(0.0f, -16.8f * 0.75f);

    wheels[0] = Wheel(frontSize, frontPos1);
    wheels[1] = Wheel(frontSize, frontPos2);
    wheels[2] = Wheel(rearSize, rearPos);
    wheels[2].shape.setFillColor(sf::Color::Red);
    wheels[2].shape.setOutlineColor(sf::Color::Black);
    wheels[2].shape.setOutlineThickness(0.5f);

    shape = RectangleShape(Vector2f(12.8f, 20.0f));
    shape.setOrigin(Vector2f(6.4f, 20.0f));
    shape.setFillColor(sf::Color(101, 123, 131));

    dot = RectangleShape(Vector2f(2.0f, 2.0f));
    dot.setFillColor(sf::Color::Red);
    dot.setOrigin(dot.getSize() * 0.5f);

    eyes.setParentInfo(position, rotation);
}

float Forklift::getRotation()
{
    return rotation;
}

Vector2f Forklift::getPosition()
{
    return position;
}

void Forklift::moveBy(Vector2f step)
{
    position += step;

    eyes.setParentInfo(position, rotation);
}

void Forklift::rotateBy(float degrees)
{
    rotation += degrees;
    rotation = remainder(rotation, 360.0f);

    eyes.setParentInfo(position, rotation);
}

void Forklift::drive(float power, float dT)
{
    turnWheel(dT);

    float arclength = 23.5f * power * dT;

    if (wheels[2].rotation == 0.0f)
    {
        Vector2f forward = help::rotateByDeg(Vector2f(0.0f, 1.0f), rotation);
        moveBy(arclength * forward);
    }
    else
    {
        float circumference = 2.0f * 3.1415f * -turnOffset();
        float angle = 360.0f * arclength / circumference;
        float distance = 2.0f * -turnOffset() * help::sineDeg(angle * 0.5f);

        uint iter = 4;
        float mult = 1.0f / iter;
        for (uint i = 0; i < iter; i++)
        {
            rotateBy(angle * mult);
            Vector2f forward = help::rotateByDeg(Vector2f(0.0f, 1.0f), rotation);
            moveBy(distance * forward * mult);
        }
    }
}

void Forklift::turnWheel(float dT)
{
    const float difference = steeringTarget - wheels[2].rotation;

    if (difference != 0.0f)
    {
        const float degsPerSecond = 180.0f;
        const float maxRot = degsPerSecond * dT * copysignf(1.0f, difference);

        float change;

        if (abs(difference) > abs(maxRot))
        {
            change = maxRot;
        }
        else
        {
            change = difference;
        }

        wheels[2].rotation += change;
    }
}

float Forklift::turnOffset()
{
    float b = wheels[0].position.y - wheels[2].position.y;
    return b / help::tangentDeg(wheels[2].rotation);
}

float Forklift::turnRadius()
{
    float b = wheels[0].position.y - wheels[2].position.y;
    return b / help::sineDeg(wheels[2].rotation);
}

float Forklift::getWheelAngle()
{
    return wheels[2].rotation;
}
