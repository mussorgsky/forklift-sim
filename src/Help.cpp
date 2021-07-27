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
    float distance(Vector2f const a, Vector2f const b)
    {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }

    bool checkNiceTrack(std::vector<Vector2f> const &road_path, std::vector<std::shared_ptr<DistanceField>> const &distance_fields)
    {
        for (auto &p : road_path)
        {
            for (auto &df : distance_fields)
            {
                if (df->distance(p) < 7.0f)
                {
                    return false;
                }
            }
        }
        return true;
    }

    float dot(Vector2f const a, Vector2f const b)
    {
        return (a.x * b.x) + (a.y * b.y);
    }

    Vector2f sincosDeg(float angle)
    {
        return Vector2f(sineDeg(angle), cosineDeg(angle));
    }

    bool saveToFile(std::vector<std::vector<RoadSegment>> &road_segment_vectors, std::string const &filename)
    {
        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
        if (file.good())
        {
            uint16_t segment_vector_count = road_segment_vectors.size();
            file.write((char *)&segment_vector_count, sizeof(uint16_t));
            for (auto &rsv : road_segment_vectors)
            {
                uint16_t size = rsv.size();
                file.write((char *)&size, sizeof(size));
                file.write((char *)rsv.data(), size * sizeof(RoadSegment));
            }
        }
        else
        {
            return false;
        }
        file.close();
        return true;
    }

    std::vector<std::vector<RoadSegment>> loadFromFile(std::string const &filename)
    {
        std::ifstream file;
        file.open(filename, std::ios::in | std::ios::binary);
        std::vector<std::vector<RoadSegment>> result;
        if (file.good())
        {
            uint16_t segment_vector_count;
            file.read((char *)&segment_vector_count, sizeof(uint16_t));
            for (int i = 0; i < segment_vector_count; ++i)
            {
                uint16_t size;
                file.read((char *)&size, sizeof(size));
                std::vector<RoadSegment> segments;
                for (int i = 0; i < size; ++i)
                {
                    RoadSegment rs;
                    file.read((char *)&rs, sizeof(rs));
                    segments.push_back(rs);
                }
                result.push_back(segments);
            }
        }
        return result;
    }

} // namespace help
