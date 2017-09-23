#include "stdafx.h"

Matrix::Matrix() {
	// Maybe do nothing
}

void Matrix::fill_in(Change c) {
	int x, y, figure;
	c.get_change(x, y, figure);
	Point & tg_point = table[x - 1][y - 1];
	tg_point.fill_in(figure);
}

void Matrix::fill_in_figure(int x, int y, int figure) {
	Point & tg_point = table[x - 1][y - 1];
	tg_point.fill_in(figure);
}

int Matrix::set_point_candi(int x, int y) {
	int zero_x = x - 1;
	int zero_y = y - 1;
	int group_x = zero_x / 3;
	int group_y = zero_y / 3;
	int v = 0;
	Point & tg_point = table[x-1][y-1];
	// search local group
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			v = get_point_value(3*group_x + i + 1,3*group_y + j + 1);
			if (v != 0) {
				tg_point.remove_candi(v);
			}
		}
	}
	// check the line
	for (int i = 1; i <= 9; i++) {
		v = get_point_value(i, y);
		if (v != 0) {
			tg_point.remove_candi(v);
		}
	}
	// check the row
	for (int j = 1; j <= 9; j++) {
		v = get_point_value(x, j);
		if (v != 0) {
			tg_point.remove_candi(v);
		}
	}
	return tg_point.get_candi_count();
}

int Matrix::get_point_value(int x, int y) const {
	const Point & tg_point = table[x - 1][y - 1];
	return tg_point.get_figure();
}

bool Matrix::roll_back(Change c) {
	int x, y, num;
	c.get_change(x, y, num);
	int old_value = get_point_value(x, y);
	if (old_value != 0) {
		Point & tg_point = table[x - 1][y - 1];
		tg_point.fill_in(0);
		return true;
	}
	else {
		std::cout << "Roll back value error at: (" << x << ", " << y << ")" << std::endl;
		return false;
	}
}

void Matrix::display() const {
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			std::cout << get_point_value(i, j);
			std::cout << (j == 9 ? '\n' : ' ');
		}
	}
}