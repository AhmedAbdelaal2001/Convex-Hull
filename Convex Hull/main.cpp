#include "Point.h"
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
using namespace std;

//Prompts the user for input.
void getUserInput(Point* pointsArr, int arrSize);

//Outputs the points on the convex hull after it is calculated.
void printConvexHull(Point* pointsArr, list<int>* convexHullPoints);

//Predicate that will be used to sort the points in ascending order of x-coordinates.
bool sortingPredicate(Point point1, Point point2) { return point1.getX() < point2.getX(); }

//Computes the convex hull of a set of points stored in pointsArr, from startIndex to endIndex.
list<int>* convexHull(Point* pointsArr, int startIndex, int endIndex);

int main() {

	int n;

	cout << "Enter the number of points: ";
	cin >> n;                                  //Get the number of points from the user.

	Point* pointsArr = new Point[n];          //Create an array to store those n points.
	getUserInput(pointsArr, n);              //Prompt the user to enter the n points.
	sort(pointsArr, pointsArr + n, sortingPredicate);            //Sort the array in ascending order of x.
	
	list<int>* convexHullPoints = convexHull(pointsArr, 0, n - 1);       //Compute the convex hull of all points within the array.
	printConvexHull(pointsArr, convexHullPoints);                       //Print the convex hull.

	delete convexHullPoints;

	return 0;
}

void getUserInput(Point* pointsArr, int arrSize) {

	float x, y;

	cout << "\nEnter the x and y coordinates of each point: " << endl;

	for (int i = 0; i < arrSize; i++) {

		cout << "\nPoint " << i + 1 << ": " << endl;
		cout << "X-Coordinate: ";
		cin >> x;
		cout << "Y-Coordinate: ";
		cin >> y;

		Point inputPoint(i + 1, x, y);
		pointsArr[i] = inputPoint;
	}

}

void printConvexHull(Point* pointsArr, list<int>* convexHullPoints) {

	cout << "\n------------------------------------------------------------------------------------------------------------\n" << endl;
	cout << "Printing the convex Hull: " << endl;

	list<int>::iterator it;
	for (it = convexHullPoints->begin(); it != convexHullPoints->end(); it++) {

		cout << "\nPoint " << pointsArr[*it].getId() << endl;
		cout << "X-Coordinate = " << pointsArr[*it].getX() << endl;
		cout << "Y-Coordinate = " << pointsArr[*it].getY() << endl;

	}

}

//Finds the y-coordinate of the point of intersection of the straight line connecting the two points at index positions point1Index 
// and point2Index with the line x = xLine.
float getIntersection(Point* pointsArr, int point1Index, int point2Index, float xLine) {

	Point point1 = pointsArr[point1Index];
	Point point2 = pointsArr[point2Index];

	float deltaX = point2.getX() - point1.getX();
	float deltaY = point2.getY() - point1.getY();

	return (deltaY / deltaX) * (xLine - point1.getX()) + point1.getY();

}

//Two functions allowing us to use std::list as a circular doubly linked list. Creates the same effect as connecting the first
//and last element in the chain.
list<int>::iterator rotateClockwise(list<int>* L, list<int>::iterator it) {

	it++;

	if (it == L->end()) return L->begin();
	else return it;

}
list<int>::iterator rotateAntiClockwise(list<int>* L, list<int>::iterator it) {

	if (it == L->begin()) return --L->end();
	else return --it;

}

//Rotates clockwise in the rightConvexHull and anticlockwise in the leftConvexHull, until it finds the 2 points which maximize the
//intersection with the line x = dividingLineX. Those 2 points must be on the upper tangent, and so they are returned by the function.
pair<list<int>::iterator, list<int>::iterator> getUpperTanget(Point* pointsArr, list<int>* leftConvexHull, list<int>::iterator leftHullIterator,
	list<int>* rightConvexHull, list<int>::iterator rightHullIterator, float dividingLineX) {

	float intersection1, intersection2, intersection3;

	do {
		intersection1 = getIntersection(pointsArr, *leftHullIterator, *rightHullIterator, dividingLineX);
		intersection2 = getIntersection(pointsArr, *leftHullIterator, *rotateClockwise(rightConvexHull, rightHullIterator), dividingLineX);
		intersection3 = getIntersection(pointsArr, *rotateAntiClockwise(leftConvexHull, leftHullIterator), *rightHullIterator, dividingLineX);

		if (intersection2 > intersection1) rightHullIterator = rotateClockwise(rightConvexHull, rightHullIterator);
		else if (intersection3 > intersection1) leftHullIterator = rotateAntiClockwise(leftConvexHull, leftHullIterator);

	} while (intersection2 > intersection1 || intersection3 > intersection1);

	return { leftHullIterator, rightHullIterator };

}

//Rotates anticlockwise in the rightConvexHull and clockwise in the leftConvexHull, until it finds the 2 points which minimize the
//intersection with the line x = dividingLineX. Those 2 points must be on the lower tangent, and so they are returned by the function.
pair<list<int>::iterator, list<int>::iterator> getLowerTanget(Point* pointsArr, list<int>* leftConvexHull, list<int>::iterator leftHullIterator,
	list<int>* rightConvexHull, list<int>::iterator rightHullIterator, float dividingLineX) {

	float intersection1, intersection2, intersection3;

	do {
		intersection1 = getIntersection(pointsArr, *leftHullIterator, *rightHullIterator, dividingLineX);
		intersection2 = getIntersection(pointsArr, *leftHullIterator, *rotateAntiClockwise(rightConvexHull, rightHullIterator), dividingLineX);
		intersection3 = getIntersection(pointsArr, *rotateClockwise(leftConvexHull, leftHullIterator), *rightHullIterator, dividingLineX);

		if (intersection2 < intersection1) rightHullIterator = rotateAntiClockwise(rightConvexHull, rightHullIterator);
		else if (intersection3 < intersection1) leftHullIterator = rotateClockwise(leftConvexHull, leftHullIterator);

	} while (intersection2 < intersection1 || intersection3 < intersection1);

	return { leftHullIterator, rightHullIterator };

}

//Cut and paste in time O(n) to merge the two hulls.
void merge(list<int>* convexHullPoints, pair<list<int>::iterator, list<int>::iterator> upperTanget, 
			pair<list<int>::iterator, list<int>::iterator> lowerTanget, list<int>* leftConvexHull, list<int>* rightConvexHull) {

	list<int>::iterator leftHullIterator;
	list<int>::iterator rightHullIterator;

	convexHullPoints->push_back(*upperTanget.first);
	rightHullIterator = upperTanget.second;                //Link the two points on the upper tangent.

	while (rightHullIterator != lowerTanget.second) {
		convexHullPoints->push_back(*rightHullIterator);        //go down the rightConvexHull list until a point on the lower tangent is reached.
		rightHullIterator = rotateClockwise(rightConvexHull, rightHullIterator);
	}

	convexHullPoints->push_back(*lowerTanget.second);
	leftHullIterator = lowerTanget.first;                        //Insert the points on the lower tangent into the convex hull.

	while (leftHullIterator != upperTanget.first) {
		convexHullPoints->push_back(*leftHullIterator);
		leftHullIterator = rotateClockwise(leftConvexHull, leftHullIterator);    //go up the leftConvexHull until a point on the upper tangent is reached.
	}

}

//Uses a divide and conquer strategy to compute the convex hull of a set of points.
//Runs in O(nlogn).
list<int>* convexHull(Point* pointsArr, int startIndex, int endIndex) {

	list<int>* convexHullPoints = new list<int>;                  //The list which will store the points on the convex hull.

	if (startIndex == endIndex) {                                //Base case: If only one point is being considered, then the convex
		convexHullPoints->push_back(startIndex);                //hull will only contain that point.
		return convexHullPoints;
	}
	else if (endIndex - startIndex == 1) {                     //Base case: If only 2 points are being considered, then create
		convexHullPoints->push_back(startIndex);              //a convex hull containing those 2 points.
		convexHullPoints->push_back(endIndex);
		return convexHullPoints;
	}

	int middle = (startIndex + endIndex) / 2;               //Compute the index of the middle element.

	//If the middle element has the same x value as the one directly after it, then search for the first element within the array
	//that is different from the one following it, and set middle to be the index of that element. This is done because if the elements
	//at positions middle and middle+1 have the same x-value, we would not be able to split the points into two half sets.
	if (middle < endIndex && pointsArr[middle].getX() == pointsArr[middle + 1].getX()) {

		middle = startIndex;
		while (middle < endIndex && pointsArr[middle].getX() == pointsArr[middle + 1].getX()) middle++;
		if (middle == endIndex) {

			//If all the elements have the same x-value, then the convex hull contains all of those points, and is a vertical line.
			for (int i = startIndex; i <= endIndex; i++) convexHullPoints->push_back(i);
			return convexHullPoints;

		}

	}

	//Now that we have the index position of the middle element, find a value k for which the vertical line x = k splits our points
	//into two sets.
	float dividingLineX = (pointsArr[middle].getX() + pointsArr[middle + 1].getX()) / 2;

	//Recursively compute the convex hull of the left and right halves of our original set of points.
	list<int>* leftConvexHull = convexHull(pointsArr, startIndex, middle);
	list<int>* rightConvexHull = convexHull(pointsArr, middle + 1, endIndex);

	//Set leftHullIterator to point to the value on the leftConvexHull with the largest x-value.
	list<int>::iterator leftHullIterator = find(leftConvexHull->begin(), leftConvexHull->end(), middle);
	//Set RightHullITerator to point to the value on the rightConvexHull with the smallest x-value.
	list<int>::iterator rightHullIterator = find(rightConvexHull->begin(), rightConvexHull->end(), middle + 1);
	
	//Compute the upper and lower tangents connecting the left and right convex hulls.
	pair<list<int>::iterator, list<int>::iterator> upperTanget = getUpperTanget(pointsArr, leftConvexHull, leftHullIterator, rightConvexHull, rightHullIterator, dividingLineX);
	pair<list<int>::iterator, list<int>::iterator> lowerTanget = getLowerTanget(pointsArr, leftConvexHull, leftHullIterator, rightConvexHull, rightHullIterator, dividingLineX);

	//Merge the left and right convex hulls.
	merge(convexHullPoints, upperTanget, lowerTanget, leftConvexHull, rightConvexHull);

	delete leftConvexHull;
	delete rightConvexHull;

	return convexHullPoints;

}