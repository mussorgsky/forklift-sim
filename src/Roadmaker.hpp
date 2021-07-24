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
	vector<RectangleShape> createShapes();
};
