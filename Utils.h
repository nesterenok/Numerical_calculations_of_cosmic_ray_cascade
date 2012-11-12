#pragma once

template<typename T>
T ** alloc_2d_array(const int rows, const int cols)
{
	T ** res = new T* [rows];
	*res = new T[rows*cols];

	for (int i = 1; i < rows; ++i) {

		res[i] = res[0] + cols*i;
	}

	return res;
}

template<typename T>
void free_2d_array(T ** array)
{
	delete [] *array;
	delete []  array;
}
