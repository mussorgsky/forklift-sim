#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "Help.hpp"
#include "RoadSegment.hpp"

using sf::RectangleShape;
using sf::Vector2f;
using std::stof;

using std::string;
using std::vector;

struct RoadConfig
{
	unsigned int const turns_left, turns_right, straights, skips;
	float const radius_min, radius_max, length_min, length_max, angle_min, angle_max, lead_length;
	bool lead_in, lead_out;
};

class Roadmaker
{
private:
	vector<RoadSegment> road_segments;
	Vector2f position;
	float rotation;

	void createStraight(RoadSegment const &, vector<RectangleShape> &);
	void createSkip(RoadSegment const &);
	void createTurn(RoadSegment const &, vector<RectangleShape> &);

public:
	Roadmaker();
	bool addSegment(string newSegment);
	void addSegment(RoadSegment newSegment);
	void generateSegments(const RoadConfig);
	vector<RectangleShape> createShapes();
};
