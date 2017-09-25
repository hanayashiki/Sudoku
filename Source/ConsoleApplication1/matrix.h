#pragma once
#include "stdafx.h"
class Matrix {
private:
	Point table[9][9];
	int zeroes;
	std::vector <Point*> ranking;
	int group_vectors[3][3];
	int row_vectors[9];
	int column_vectors[9];
public:
	Matrix();
	void fill_in(Change* c);
	bool roll_back(Change* c);
	void display() const;
	void dump(FILE* f) const;
	Point* get_min_point();
	Point* get_min_point_fast();
	int set_point_candi(int i, int j);
	int get_point_candi_fast(int x, int y);
	inline int get_point_value(int i, int j) const;
	void fill_in_figure(int i, int j, int f);
	std::vector <Point*> rank();
	bool check_validity() const;
	int get_zeroes() const {
		return zeroes;
	}
};