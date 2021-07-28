#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <iostream>

#include "Wheel.hpp"
#include "Help.hpp"
#include "Sensor.hpp"

using sf::RectangleShape;
using sf::Vector2f;
using std::remainder;

class Forklift
{
private:
  Vector2f position;
  float rotation;

  void turnWheel(float dT);

public:
  float steeringTarget;
  RectangleShape shape;
  RectangleShape dot;
  Wheel wheels[3];
  Sensor eyes;

  Forklift();

  Vector2f getPosition();
  float getRotation();
  void moveBy(Vector2f step);
  void rotateBy(float degrees);

  void drive(float power, float dT);
  float turnOffset();
  float turnRadius();
  float getWheelAngle();
};