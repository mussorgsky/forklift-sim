#include <SFML/System/Vector2.hpp>

using sf::Vector2f;

class Camera
{
private:
  float zoom;
  Vector2f position;
  Vector2f resolution;

public:
  Camera();
  Camera(float zm, Vector2f pos, Vector2f res);

  Vector2f projectToScreen(Vector2f worldPos);
  Vector2f getZoom();
  void setZoom(float zm);
};