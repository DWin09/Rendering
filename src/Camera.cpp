
#include "Camera.hpp"


Camera::Camera(double fov, double aspect, double near, double far)
{
	double s = 1 / tan(TO_RAD(fov / 2));
	for (int i = 0; i < 4; i++)
	{
		this->projectionMatrix.push_back(std::vector<double>(4, 0));
		this->rotMatrix.push_back(std::vector<double>(4, 0));
		this->cameraMatrix.push_back(std::vector<double>(4, 0));
		this->tmpMatrix1.push_back(std::vector<double>(4, 0));
		this->tmpMatrix2.push_back(std::vector<double>(4, 0));
		this->cameraMatrix.at(i).at(i) = 1;
		this->rotMatrix.at(i).at(i) = 1;
	}
	this->projectionMatrix.at(0).at(0) = s;
	this->projectionMatrix.at(1).at(1) = s / aspect;
	this->projectionMatrix.at(3).at(2) = -1;
	this->projectionMatrix.at(2).at(2) = -far / (far - near);
	this->projectionMatrix.at(2).at(3) = -far * near / (far - near);

}
void Camera::toCameraSpace(Triangle& tri, Triangle& result)
{
	Triangle tmp;

	tri.project(this->rotMatrix, tmp);

	tmp.project(this->cameraMatrix, result);
}
void Camera::toScreenSpace(Triangle& tri, Triangle &result)
{
	/*
	Triangle tmp;
	this->toCameraSpace(tri, tmp);
	tmp.project(this->projectionMatrix, result);*/
	if (this->recalc)
	{

		matMul(this->cameraMatrix, this->rotMatrix, this->tmpMatrix1);
		matMul(this->projectionMatrix, this->tmpMatrix1, this->tmpMatrix2);
		this->recalc = false;
	}
	tri.project(this->tmpMatrix2, result);
}
void Camera::move(double right, double up, double forward)
{
	Point vel = Point(right, up, forward);
	this->move(vel);
}
void Camera::move(Point &vel)
{
	std::vector<std::vector<double>> matrix,tmp;

	for (int i = 0; i < 4; i++)
	{
		matrix.push_back(std::vector<double>(4, 0));
		matrix.at(i).at(3) = vel.position.at(i);
		matrix.at(i).at(i) = 1;
	}
	matMul(matrix, this->cameraMatrix, tmp);
	this->cameraMatrix = tmp;

	this->recalc = true;
}
void Camera::rotateX(double degree)
{
	if (degree == 0)
	{
		return;
	}
	double rad = TO_RAD(degree);
	double cosVal = cos(rad);
	double sinVal = sin(rad);
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < 4; i++)
	{
		matrix.push_back(std::vector<double>(4, 0));
	}
	matrix.at(0).at(0) = 1;
	matrix.at(1).at(1) = cosVal;
	matrix.at(1).at(2) = -sinVal;
	matrix.at(2).at(1) = sinVal;
	matrix.at(2).at(2) = cosVal;
	matrix.at(3).at(3) = 1;
	this->rotate(matrix);
}
void Camera::rotateY(double degree)
{
	if (degree == 0)
	{
		return;
	}
	double rad = TO_RAD(degree);
	double cosVal = cos(rad);
	double sinVal = sin(rad);
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < 4; i++)
	{
		matrix.push_back(std::vector<double>(4, 0));
	}
	matrix.at(1).at(1) = 1;
	matrix.at(0).at(0) = cosVal;
	matrix.at(2).at(0) = -sinVal;
	matrix.at(0).at(2) = sinVal;
	matrix.at(2).at(2) = cosVal;
	matrix.at(3).at(3) = 1;
	this->rotate(matrix);
}
void Camera::rotateZ(double degree)
{
	if (degree == 0)
	{
		return;
	}
	double rad = TO_RAD(degree);
	double cosVal = cos(rad);
	double sinVal = sin(rad);
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < 4; i++)
	{
		matrix.push_back(std::vector<double>(4, 0));
	}
	matrix.at(2).at(2) = 1;
	matrix.at(0).at(0) = cosVal;
	matrix.at(0).at(1) = -sinVal;
	matrix.at(1).at(0) = sinVal;
	matrix.at(1).at(1) = cosVal;
	matrix.at(3).at(3) = 1;
	this->rotate(matrix);
}
void Camera::rotate(std::vector<std::vector<double>>& matrix)
{

	std::vector<std::vector<double>> tmpMatrix;


	matMul(matrix, this->cameraMatrix, tmpMatrix);
	this->cameraMatrix = tmpMatrix;
	this->recalc = true;
}