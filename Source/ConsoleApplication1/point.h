#pragma once
#include "stdafx.h"
class Point {
private:
	int figure;
	int candidates[9];
	int candi_count;
public:
	Point();
	void fill_in(int c);
	void init_candi();
	bool remove_candi(int f); // f is the candidate itself.
	int get_candi_count() const{
		return candi_count;
	}
	int get_figure() const{
		return figure;
	}
};

