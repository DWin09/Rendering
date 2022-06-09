
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include "MatrixMath.hpp"
#include "Triangle.hpp"

#include <iostream>

class Object
{
private:
public:
	Object();
	Object(std::vector<Triangle> &tris);
	void initModelMatrix();
	void getTris(std::vector<Triangle>& result);
	void move(double x, double y, double z);
	void rotateX(double degree);
	void rotateY(double degree);
	void rotateZ(double degree);
	void rotate(std::vector<std::vector<double>>& matrix);
	void rotateLocalX(double degree);
	void rotateLocalY(double degree);
	void rotateLocalZ(double degree);

	std::vector<Triangle> tris;
	std::vector<Triangle> visibleTris;
	std::vector<std::vector<double>> modelMatrix;
};

#endif