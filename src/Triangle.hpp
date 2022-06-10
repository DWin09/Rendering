

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Point.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>

class Triangle
{

private:
	double __minDistance = 1000000000;
	Point v0, v1, v2;
	double d00, d01, d11, d20, d21, inverseDenomentator;
public:
	Triangle() = default;
	Triangle(Point p1, Point p2, Point p3, std::vector<int> color = std::vector<int>(3,0));
	void calcNormal();
	double minDistance();
	void calcBoundingBox();
	bool withinBoundingBox(double x, double y);
	double minX();
	double maxX();
	double minY();
	double maxY();
	double minZ();
	double maxZ();
	void calcBarycentric();
	bool pointToBarycentric(Point &p);
	bool __rayPlaneIntersect(Point &rayOrigin, Point &rayDirection, double& currentDepth, double& depth);
	bool rayIntersect(Point &rayOrigin, Point &rayDirection, double &currentDepth, double &depth);
	void rotX(double degree);
	void rotY(double degree);
	void rotZ(double degree);
	void project(std::vector<std::vector<double>>& matrix, Triangle& result);


	std::vector<Point> points;
	std::vector<int> color;
	std::vector<Point> __boundingBox = std::vector<Point>(2);
	Point normal;
};

#endif
