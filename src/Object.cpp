
#include "Object.hpp"


Object::Object()
{
	this->initModelMatrix();
}
Object::Object(std::vector<Triangle> &tris)
{
	this->initModelMatrix();
	for (int i = 0; i < tris.size(); i++)
	{
		this->tris.push_back(tris.at(i));
	}
}
void Object::initModelMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		this->modelMatrix.push_back(std::vector<double>(4, 0));
		this->modelMatrix.at(i).at(i) = 1;
	}
}
void Object::getTris(std::vector<Triangle> &result)
{
	Triangle tri;
	for (int i = 0; i < this->tris.size(); i++)
	{
		this->tris.at(i).project(this->modelMatrix, tri);
		result.push_back(tri);
	}
}
void Object::move(double x, double y, double z)
{

	std::vector<std::vector<double>> moveMatrix, resultMatrix;

	for (int i = 0; i < 4; i++)
	{
		moveMatrix.push_back(std::vector<double>(4, 0));
		moveMatrix.at(i).at(i) = 1;
	}
	moveMatrix.at(0).at(3) = x;
	moveMatrix.at(1).at(3) = y;
	moveMatrix.at(2).at(3) = z;
	matMul(this->modelMatrix, moveMatrix, resultMatrix);
	this->modelMatrix = resultMatrix;
}
void Object::rotateX(double degree)
{

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
void Object::rotateY(double degree)
{

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
void Object::rotateZ(double degree)
{

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
void Object::rotate(std::vector<std::vector<double>>& matrix)
{
	std::vector<std::vector<double>> tmpMatrix;

	for (int i = 0; i < 4; i++)
	{
		tmpMatrix.push_back(std::vector<double>(4, 0));
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmpMatrix.at(i).at(j) += this->modelMatrix.at(j).at(i) * matrix.at(i).at(j);
		}
	}
	this->modelMatrix = tmpMatrix;
}
void Object::rotateLocalX(double degree)
{
	for (int i = 0; i < this->tris.size(); i++)
	{
		this->tris.at(i).rotX(degree);
	}
}
void Object::rotateLocalY(double degree)
{
	for (int i = 0; i < this->tris.size(); i++)
	{
		this->tris.at(i).rotY(degree);
	}
}
void Object::rotateLocalZ(double degree)
{
	for (int i = 0; i < this->tris.size(); i++)
	{
		this->tris.at(i).rotZ(degree);
	}
}