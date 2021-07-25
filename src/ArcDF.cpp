#include "ArcDF.hpp"

float ArcDF::distance(Vector2f const point)
{
    Vector2f p = point - origin;
    p = Vector2f(p.x * sca.x - p.y * sca.y, p.x * sca.y + p.y * sca.x);
    p.x = abs(p.x);
    float const k = (scb.y * p.x > scb.x * p.y) ? help::dot(p, scb) : help::distance({0.0f, 0.0f}, p);
    return sqrt(help::dot(p, p) + ra * ra - 2.0f * ra * k);
}
