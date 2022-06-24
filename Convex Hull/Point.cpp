#include "Point.h"

//Constructor.
Point::Point(int id, float x, float y) { setId(id), setX(x), setY(y); }


//Setters and Getters.
void Point::setId(int id) { this->id = id; }
float Point::getId() const { return id; }

void Point::setX(float x) { this->x = x; }
float Point::getX() const { return x; }

void Point::setY(float y) { this->y = y; }
float Point::getY() const { return y; }
