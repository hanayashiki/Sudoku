#pragma once
#include "stdafx.h"
class Matrix {
private:
	Point table[9][9];
public:
	Matrix();
	void fill_in(Change c);
	bool roll_back(Change c);
	void display() const;
	int set_point_candi(int i, int j);
	int get_point_value(int i, int j) const;
	void fill_in_figure(int i, int j, int f);
};