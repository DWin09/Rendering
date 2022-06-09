

#include "Triangle.hpp"




Triangle::Triangle(Point p1, Point p2, Point p3, std::vector<int> color)
{
	this->points.push_back(p1);
	this->points.push_back(p2);
	this->points.push_back(p3);
	this->calcBarycentric();
	this->color = color;
}
void Triangle::calcNormal()
{
	Point p1, p2;
	this->points.at(0).subPointNew(this->points.at(1), p1);
	this->points.at(0).subPointNew(this->points.at(2), p2);
	p1.crossProduct(p2, this->normal);
}
double Triangle::minDistance()
{
	if (this->__minDistance >= 1000000)
	{
		this->__minDistance = this->__boundingBox.at(0).position.at(2);
	}
	return this->__minDistance;
}
void Triangle::calcBoundingBox()
{
	double _x, _y, _z;
	_x = _y = _z = 1000000000;
	double __x, __y, __z;
	__x = __y = __z = -1000000000;
	for (int i = 0; i < 3; i++)
	{
		if (this->points.at(i).position.at(0) < _x)
		{
			_x = this->points.at(i).position.at(0);
		}
		if (this->points.at(i).position.at(0) > __x)
		{
			__x = this->points.at(i).position.at(0);
		}
		if (this->points.at(i).position.at(1) < _y)
		{
			_y = this->points.at(i).position.at(1);
		}
		if (this->points.at(i).position.at(1) > __y)
		{
			__y = this->points.at(i).position.at(1);
		}
		if (this->points.at(i).position.at(2) < _z)
		{
			_z = this->points.at(i).position.at(2);
		}
		if (this->points.at(i).position.at(2) > __z)
		{
			__z = this->points.at(i).position.at(2);
		}
	}
	this->__boundingBox.at(0).setCoords(_x, _y, _z);
	this->__boundingBox.at(1).setCoords(__x, __y, __z);
	this->__minDistance = 1000000;
}

bool Triangle::withinBoundingBox(double x, double y)
{
	if (x < this->__boundingBox.at(0).position.at(0) || x > this->__boundingBox.at(1).position.at(0))
	{
		return false;
	}
	if (y < this->__boundingBox.at(0).position.at(1) || y > this->__boundingBox.at(1).position.at(1))
	{
		return false;
	}
	return true;
}
double Triangle::minX()
{
	return this->__boundingBox.at(0).position.at(0);
}
double Triangle::maxX()
{
	return this->__boundingBox.at(1).position.at(0);
}
double Triangle::minY()
{
	return this->__boundingBox.at(0).position.at(1);
}
double Triangle::maxY()
{
	return this->__boundingBox.at(1).position.at(1);
}
double Triangle::minZ()
{
	return this->__boundingBox.at(0).position.at(2);
}
double Triangle::maxZ()
{
	return this->__boundingBox.at(1).position.at(2);
}
bool Triangle::pointToBarycentric(Point &p)
{
	double v, w, u;
	p.subPointNew(this->points.at(0),this->v2);
	this->d20 = this->v2.dotProduct(this->v0);
	this->d21 = this->v2.dotProduct(this->v1);

	v = (this->d11 * this->d20 - this->d01 * this->d21) * this->inverseDenomentator;
	if (v > 1 || v < 0)
	{
		return false;
	}
	w = (this->d00 * this->d21 - this->d01 * this->d20) * this->inverseDenomentator;
	if (w > 1 || w < 0)
	{
		return false;
	}
	u = 1 - v - w;
	if (u > 1 || u < 0)
	{
		return false;
	}
	return true;


}

void Triangle::calcBarycentric()
{
	this->calcBoundingBox();
	this->calcNormal();
	this->points.at(1).subPointNew(this->points.at(0), this->v0);
	this->points.at(2).subPointNew(this->points.at(0), this->v1);
	this->d00 = this->v0.dotProduct(this->v0);
	this->d01 = this->v0.dotProduct(this->v1);
	this->d11 = this->v1.dotProduct(this->v1);

	this->inverseDenomentator = 1 / (this->d00 * this->d11 - this->d01 * this->d01);
}

bool Triangle::__rayPlaneIntersect(Point &rayOrigin, Point &rayDirection, double &currentDepth, double &depth)
{
	double dot;
	dot = this->normal.dotProduct(rayDirection);
	if (dot <= 0.000001)
	{
		return false;
	}
	depth = (this->normal.dotProduct(this->points.at(0)) - this->normal.dotProduct(rayOrigin)) / dot;
	if (depth > currentDepth)
	{
		return false;
		
	}
	rayOrigin.addPoint(rayDirection, depth);
	return true;
}
bool Triangle::rayIntersect(Point &rayOrigin, Point &rayDirection, double &currentDepth, double &depth)
{
	if(!Triangle::__rayPlaneIntersect(rayOrigin, rayDirection, currentDepth, depth))
	{
		return false;
	}
	return this->pointToBarycentric(rayOrigin);
}

void Triangle::rotX(double degree)
{
	for (int i = 0; i < 3; i++)
	{
		this->points.at(i).rotX(degree);
	}
	this->calcBarycentric();
}
void Triangle::rotY(double degree)
{
	for (int i = 0; i < 3; i++)
	{
		this->points.at(i).rotY(degree);
	}
	this->calcBarycentric();
}
void Triangle::rotZ(double degree)
{
	for (int i = 0; i < 3; i++)
	{
		this->points.at(i).rotZ(degree);
	}
	this->calcBarycentric();
}

void Triangle::project(std::vector<std::vector<double>>& matrix, Triangle& result)
{
	std::vector<Point> pointBuffer = std::vector<Point>(3);
	for (int i = 0; i < 3; i++)
	{
		this->points.at(i).project(matrix, pointBuffer.at(i));
	}
	result = Triangle(pointBuffer.at(0), pointBuffer.at(1), pointBuffer.at(2), this->color);
}
