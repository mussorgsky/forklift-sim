#include "Wheel.hpp"

Wheel::Wheel()
{
    size = Vector2f(1.0f, 1.0f);
    position = Vector2f(0.0f, 0.0f);
    shape = RectangleShape(size);
    rotation = 0.0f;

    shape.setPosition(position);
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(size * 0.5f);
}

Wheel::Wheel(Vector2f s, Vector2f p)
{
    size = s;
    position = p;
    shape = RectangleShape(size);
    rotation = 0.0f;

    shape.setPosition(position);
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(size * 0.5f);
}