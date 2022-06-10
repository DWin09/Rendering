
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include "MatrixMath.hpp"
#include "Triangle.hpp"
#include <iostream>

class Camera
{

private:
	std::vector<std::vector<double>> cameraMatrix, projectionMatrix, rotMatrix, tmpMatrix1, tmpMatrix2;
	bool recalc = false;
public:
	Camera(double fov, double aspect,double near, double far);
	void toCameraSpace(Triangle& tri, Triangle &result);
	void toScreenSpace(Triangle& tri, Triangle& result);
	void move(double forward = 0, double right = 0, double up = 0);
	void move(Point& vel);
	void rotateX(double degree);
	void rotateY(double degree);
	void rotateZ(double degree);
	void rotate(std::vector<std::vector<double>>& matrix);
};

#endif