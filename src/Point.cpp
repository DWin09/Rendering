

#include "Point.hpp"

Point::Point(double x, double y, double z)
{
	this->position.at(0) = x;
	this->position.at(1) = y;
	this->position.at(2) = z;
}
Point::Point(std::vector<double> &_position)
{
	for (int i = 0; i < 3; i++)
	{
		this->position.at(i) = _position.at(i);
	}
}
void Point::setCoords(double x, double y, double z)
{
	this->position.at(0) = x;
	this->position.at(1) = y;
	this->position.at(2) = z;
	this->__inverseLength = -1;
	this->__length = -1;
}
double Point::length()
{
	if (this->__length < 0)
	{
		double len = 0;
		for (int i = 0; i < 3; i++)
		{
			len += this->position.at(i);
		}
		this->__length = sqrt(len);
	}
	return this->__length;
}
double Point::inverseLength()
{
	if (this->__inverseLength < 0)
	{
		if (this->length() != 0)
		{
			this->__inverseLength = 1 / this->length();
		}
		else
		{
			this->__inverseLength = 0;
		}
	}
	return this->__inverseLength;
}

Point* Point::addPoint(Point &p, double times)
{
	if (times == 1)
	{
		this->position.at(0) += p.position.at(0);
		this->position.at(1) += p.position.at(1);
		this->position.at(2) += p.position.at(2);
	}
	else
	{
		this->position.at(0) += p.position.at(0) * times;
		this->position.at(1) += p.position.at(1) * times;
		this->position.at(2) += p.position.at(2) * times;
	}
	this->__length = -1;
	this->__inverseLength = -1;
	return this;
}
Point* Point::subPoint(Point &p)
{
	this->position.at(0) -= p.position.at(0);
	this->position.at(1) -= p.position.at(1);
	this->position.at(2) -= p.position.at(2);
	this->__length = -1;
	this->__inverseLength = -1;
	return this;
}
void Point::subPointNew(Point &p, Point &result)
{
	/*double x, y, z;
	x = this->position.at(0) - p.position.at(0);
	y = this->position.at(1) - p.position.at(1);
	z = this->position.at(2) - p.position.at(2);
	result.setCoords( x, y, z);*/
	result.setCoords(this->position.at(0) - p.position.at(0),
					 this->position.at(1) - p.position.at(1),
					 this->position.at(2) - p.position.at(2));

}
Point* Point::scale(double factor)
{
	this->position.at(0) *= factor;
	this->position.at(1) *= factor;
	this->position.at(2) *= factor;
	this->__length = -1;
	this->__inverseLength = -1;
	return this;
}
void Point::scaleNew(double factor, Point &result)
{
	double x, y, z;
	x = this->position.at(0) * factor;
	y = this->position.at(1) * factor;
	z = this->position.at(2) * factor;
	result.setCoords(x, y, z);
}
Point* Point::normalize()
{
	double factor = this->inverseLength();
	this->scale(factor);
	this->__length = 1;
	this->__inverseLength = 1;
	return this;
}

double Point::dotProduct(Point &p)
{
	return this->position.at(0) * p.position.at(0) +
		this->position.at(1) * p.position.at(1) +
		this->position.at(2) * p.position.at(2);
}

void Point::crossProduct(Point &p, Point &result)
{
	result.setCoords(this->position.at(1) * p.position.at(2) - this->position.at(2) * p.position.at(1),
					this->position.at(2) * p.position.at(0) - this->position.at(0) * p.position.at(2),
					this->position.at(0) * p.position.at(1) - this->position.at(1) * p.position.at(0));
}

void Point::rotX(double degree)
{
	double rad = TO_RAD(degree);
	double cosVal = cos(rad);
	double sinVal = sin(rad);
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < 3; i++)
	{
		matrix.push_back(std::vector<double>());
		for (int j = 0; j < 3; j++)
		{
			matrix.at(i).push_back(0);
		}
	}
	matrix.at(0).at(0) = 1;
	matrix.at(1).at(1) = cosVal;
	matrix.at(1).at(2) = -sinVal;
	matrix.at(2).at(1) = sinVal;
	matrix.at(2).at(2) = cosVal;
	this->rot(matrix);
}
void Point::rotY(double degree)
{
	double rad = TO_RAD(degree);
	double cosVal = cos(rad);
	double sinVal = sin(rad);
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < 3; i++)
	{
		matrix.push_back(std::vector<double>());
		for (int j = 0; j < 3; j++)
		{
			matrix.at(i).push_back(0);
		}
	}
	matrix.at(1).at(1) = 1;
	matrix.at(0).at(0) = cosVal;
	matrix.at(2).at(0) = -sinVal;
	matrix.at(0).at(2) = sinVal;
	matrix.at(2).at(2) = cosVal;
	this->rot(matrix);
}
void Point::rotZ(double degree)
{
	double rad = TO_RAD(degree);
	double cosVal = cos(rad);
	double sinVal = sin(rad);
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < 3; i++)
	{
		matrix.push_back(std::vector<double>());
		for (int j = 0; j < 3; j++)
		{
			matrix.at(i).push_back(0);
		}
	}
	matrix.at(2).at(2) = 1;
	matrix.at(0).at(0) = cosVal;
	matrix.at(0).at(1) = -sinVal;
	matrix.at(1).at(0) = sinVal;
	matrix.at(1).at(1) = cosVal;
	this->rot(matrix);
}

void Point::rot(std::vector<std::vector<double>>& matrix)
{
	std::vector<double> pos = std::vector<double>(4,0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pos.at(i) += matrix.at(i).at(j) * this->position.at(j);
		}
	}
	pos.at(3) = 1;
	for (int i = 0; i < 4; i++)
	{
		this->position.at(i) = pos.at(i);
	}
}

void Point::project(std::vector<std::vector<double>>& matrix, Point& result)
{
	std::vector<double> pos = std::vector<double>(4, 0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pos.at(i) += matrix.at(i).at(j) * this->position.at(j);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		pos.at(i) /= pos.at(3);
	}
	result.setCoords(pos.at(0), pos.at(1), pos.at(2));
}