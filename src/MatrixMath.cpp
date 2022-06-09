
#include "MatrixMath.hpp"


void matMul(std::vector<std::vector<double>>& m1, std::vector<std::vector<double>>& m2, std::vector<std::vector<double>>& result)
{
	for (int i = 0; i < 4; i++)
	{
		result.push_back(std::vector<double>(4, 0));
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.at(i).at(j) += m1.at(i).at(k) * m2.at(k).at(j);
			}
		}
	}
};

double TO_RAD(double x)
{
	return x * 3.141592653589793238462643383279502 / 180.0;
};