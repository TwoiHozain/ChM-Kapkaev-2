#pragma once
#include <vector>

//Сделать красиво
class CSRMatrix
{
	std::vector<std::vector<double>> matrix;
public:
	CSRMatrix(size_t size)
	{
		matrix.resize(size);

		for (size_t i = 0; i < matrix.size(); i++)
			matrix[i].resize(size);
	}

	std::vector<double>& operator[](int ind) { return matrix[ind]; }

};