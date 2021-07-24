#include "Help.hpp"

namespace help
{
    Vector2f rotateByDeg(Vector2f v, float angle)
    {
        float rad = angle * 3.1415f / 180.0f;
        float newX = v.x * cos(rad) - v.y * sin(rad);
        float newY = v.x * sin(rad) + v.y * cos(rad);

        return Vector2f(newX, newY);
    }

    float tangentDeg(float angle)
    {
        float rad = angle * 3.1415f / 180.0f;
        return tan(rad);
        // return rad + 0.333f * rad * rad * rad;
    }

    float sineDeg(float angle)
    {
        float rad = angle * 3.1415f / 180.0f;
        return sin(rad);
        // return rad - 0.166f * rad * rad * rad;
    }

    float cosineDeg(float angle)
    {
        float rad = angle * 3.1415f / 180.0f;
        return cos(rad);
        // return rad - 0.166f * rad * rad * rad;
    }

    bool isInsideRectangle(Vector2f point, RectangleShape rectangle)
    {
        point -= rectangle.getPosition();
        point = rotateByDeg(point, -rectangle.getRotation());
        Vector2f size = rectangle.getSize() * 0.5f;

        return (-size.x <= point.x && point.x <= size.x && -size.y <= point.y && point.y <= size.y);
    }

    Image makeGridImage(int width, int height)
    {
        Color background = sf::Color(0xee, 0xea, 0xea);
        Color border = sf::Color(0xff, 0xff, 0xff);
        Image img;
        img.create(width, height, background);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                {
                    img.setPixel(x, y, border);
                }
            }
        }

        return img;
    }

} // namespace help
