#include "stdafx.h"

Change::Change(int x, int y, int num) {
	if (x < 1 || y < 1 || x > 9 || y > 9 || num < 1 || num > 9) {
		std::cout << "Change value error: (" << x << ", " << y << ", " << num << ")" << std::endl;
		getchar();
		exit(1);
	}
	tg_x = x;
	tg_y = y;
	fill_num = num;
}

bool Change::decre_trials() {
	if (trials > 0) {
		trials--;
		return true;
	}
	else {
		return false;
	}
}

void Change::set_trials(int t) {
	trials = t;
}

void Change::get_change(int & x, int & y, int & fig) const{
	x = tg_x;
	y = tg_y;
	fig = fill_num;
}

void Change::display(char *s) {
	std::cout << s << "(" << tg_x << ", " << tg_y << ", " << fill_num << ", " << trials <<")" << std::endl;
}