#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "Help.hpp"
#include "RoadSegment.hpp"
#include "SegmentDF.hpp"

using sf::Vector2f;
using sf::Vertex;
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
	vector<vector<Vertex>> road_vertices;
	vector<std::shared_ptr<DistanceField>> distance_fields;
	vector<Vector2f> road_path;
	Vector2f position;
	Vector2f finish_point;
	float rotation;
	const float road_width;

	float const max_path_distance = 1.0f;
	float const max_visual_distance = 3.0f;

	void createStraight(RoadSegment const &);
	void createSkip(RoadSegment const &);
	void createTurn(RoadSegment const &);

public:
	Roadmaker() : position(Vector2f(0.0f, 0.0f)), rotation(0.0f), road_width(1.9f){};
	Roadmaker(float road_width) : position(Vector2f(0.0f, 0.0f)), rotation(0.0f), road_width(road_width){};
	bool addSegment(string newSegment);
	void addSegment(RoadSegment newSegment);
	void generateSegments(const RoadConfig);
	void createShapes();
	Vector2f getFinishPoint();
	vector<std::shared_ptr<DistanceField>> const &getDistanceFields();
	vector<vector<Vertex>> const &getRoadVertices();
	vector<Vector2f> const &getRoadPath();
	void reset();
};
