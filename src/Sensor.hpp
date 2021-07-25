#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <memory>

#include "Help.hpp"
#include "DistanceField.hpp"

using sf::CircleShape;
using sf::RectangleShape;
using sf::Vector2f;
using std::array;
using std::vector;

class Sensor
{
public:
  static const int width = 3;

private:
  float spacing;
  float position;
  Vector2f parentPos;
  float parentRot;
  array<Vector2f, width> makeSamplePoints();

public:
  Sensor();

  CircleShape dots[width];
  vector<std::shared_ptr<DistanceField>> distance_fields;
  array<Vector2f, width> samplePoints;

  float sense(float last);
  void setParentInfo(Vector2f postition, float rotation);
};