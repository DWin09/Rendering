
#ifndef POINT_HPP
#define POINT_HPP

#include <vector>
#include "MatrixMath.hpp"


class Point
{
private:
	double __length = -1;
	double __inverseLength = -1;
public:
	Point() = default;
	Point(double x, double y, double z);
	Point(std::vector<double> &_position);
	void setCoords(double x, double y, double z);
	double length();
	double inverseLength();
	Point* addPoint(Point &p, double times = 1);
	Point* subPoint(Point &p);
	void subPointNew(Point &p,Point &result);
	Point* scale(double factor);
	void scaleNew(double factor, Point &result);
	Point* normalize();
	double dotProduct(Point &p);
	void crossProduct(Point &p, Point &result);
	void rotX(double degree);
	void rotY(double degree);
	void rotZ(double degree);
	void rot(std::vector<std::vector<double>> &matrix);
	void project(std::vector<std::vector<double>>& matrix, Point& result);


	std::vector<double> position = std::vector<double>(4,1);
};

#endif