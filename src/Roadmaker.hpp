#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>

#include <iostream>

#include "Help.hpp"

using sf::RectangleShape;
using sf::Vector2f;
using std::stof;
using std::string;
using std::vector;

class Roadmaker
{
  private:
	vector<string> segments;
	Vector2f position;
	float rotation;

	bool checkValidity(string segment);
	void createStraight(string segment, vector<RectangleShape> *shapes);
	void createTurn(string segment, vector<RectangleShape> *shapes);
	void createSkip(string segment);

  public:
	Roadmaker();
	bool addSegment(string newSegment);
	vector<RectangleShape> createShapes();
};