#include "stdafx.h"
#include "ListenArea.h"

ListenArea::ListenArea(int radius = 300)
{
	this->shape = new sf::CircleShape(radius);
	this->shape->setOutlineThickness(1);
	shape->setOutlineColor(sf::Color::White);
	shape->setFillColor(sf::Color::Cyan);
	this->setRadius(radius);
}


ListenArea::~ListenArea()
{

}

void ListenArea::setCenter(float x, float y) {
	shape->setPosition(x, y);
	shape->move(-radius, -radius);
}

void ListenArea::setRadius(int radius)
{
	this->radius = radius;
	ListenArea::shape->setRadius(radius);
}

sf::CircleShape ListenArea::getShape() {
	return *shape;
}
