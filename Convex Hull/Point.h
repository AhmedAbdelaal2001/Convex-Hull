#pragma once

//This class represents a point on a 2 dimensional plane. Our task is to find the convex hull of many such points. 
class Point
{
private:

	int id;                      //A unique integer assigned to each point as an identifier.
	
	float x;                     //Cartesian Coordinates.
	float y;

public:

	//Constructor.
	Point(int = 0, float = 0, float = 0);

	//Setters and Getters.
	void setId(int);
	float getId() const;

	void setX(float);
	float getX() const;

	void setY(float);
	float getY() const;
};

