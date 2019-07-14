#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Help.hpp"

using sf::RectangleShape;
using sf::Vector2f;

class Wheel
{
public:
  Wheel();
  Wheel(Vector2f s, Vector2f p);

  RectangleShape shape;
  Vector2f size;
  Vector2f position;
  float rotation;
};