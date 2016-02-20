#pragma once
#include <SFML\Graphics.hpp>
class ListenArea
{
public:
	ListenArea(int radius);
	~ListenArea();
	void setRadius(int radius);
	void setCenter(float x, float y);
	sf::CircleShape getShape();
private:
	int radius = 0;
	sf::CircleShape * shape;
};

