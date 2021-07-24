#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <cmath>

using sf::Color;
using sf::Image;
using sf::RectangleShape;
using sf::Vector2f;
using std::cos;
using std::sin;
using std::tan;

namespace help
{
    Vector2f rotateByDeg(Vector2f v, float angle);
    float tangentDeg(float angle);
    float sineDeg(float angle);
    float cosineDeg(float angle);
    bool isInsideRectangle(Vector2f point, RectangleShape rectangle);
    Image makeGridImage(int width, int height);
    float distance(Vector2f const a, Vector2f const b);
} // namespace help
