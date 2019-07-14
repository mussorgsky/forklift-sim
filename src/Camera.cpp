#include "Camera.hpp"

Camera::Camera()
{
    zoom = 1.0f;
    position = Vector2f(0.0f, 0.0f);
    resolution = Vector2f(0.0f, 0.0f);
}

Camera::Camera(float zm, Vector2f pos, Vector2f res)
{
    zoom = zm;
    position = pos;
    resolution = res;
}

Vector2f Camera::projectToScreen(Vector2f worldPos)
{
    Vector2f result = (worldPos - position) * zoom + resolution * 0.5f;
    result.y *= -1.0f;
    return result;
}

Vector2f Camera::getZoom()
{
    return Vector2f(zoom, zoom);
}

void Camera::setZoom(float zm)
{
    zoom = zm;
}