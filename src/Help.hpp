#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <cmath>
#include <memory>
#include <type_traits>
#include <string>
#include <vector>
#include <fstream>

#include "DistanceField.hpp"
#include "RoadSegment.hpp"

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
    bool checkNiceTrack(std::vector<Vector2f> const &road_path, std::vector<std::shared_ptr<DistanceField>> const &distance_fields);
    float dot(Vector2f const a, Vector2f const b);
    Vector2f sincosDeg(float angle);
    bool saveToFile(std::vector<std::vector<RoadSegment>> &road_segment, std::string const &filename);
    std::vector<std::vector<RoadSegment>> loadFromFile(std::string const &filename);

    template <class T>
    T getConfigProperty(std::string const &name, std::vector<std::string> const &lines, T const fallback, bool &failed)
    {
        std::string name_eq = name + "=";
        for (auto &line : lines)
        {
            if (size_t pos = line.find(name_eq) != std::string::npos)
            {
                pos += name_eq.length() - 1;
                if constexpr (std::is_same<T, unsigned int>::value)
                {
                    return std::stoi(line.substr(pos));
                }
                if constexpr (std::is_same<T, int>::value)
                {
                    return std::stoi(line.substr(pos));
                }
                if constexpr (std::is_same<T, float>::value)
                {
                    return std::stof(line.substr(pos));
                }
                if constexpr (std::is_same<T, std::string>::value)
                {
                    return line.substr(pos);
                }
                if constexpr (std::is_same<T, bool>::value)
                {
                    return (line.substr(pos) == "1");
                }
            }
        }
        failed = true;
        return fallback;
    }
} // namespace help
