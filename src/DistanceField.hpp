#pragma once

#include <SFML/System/Vector2.hpp>

using sf::Vector2f;

class DistanceField
{
public:
    virtual float distance(Vector2f const point) = 0;
};
