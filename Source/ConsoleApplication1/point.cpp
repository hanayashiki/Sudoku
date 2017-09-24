#include "stdafx.h"

Point::Point(){
	//std::cout << "set zero" << std::endl;
	figure = 0;
	init_candi();
}

void Point::fill_in(int f) {
	if (f >= 0 || f <= 9) {
		figure = f;
		if (f == 0) {
			//std::cout << "fill in zero" << std::endl;
		}
	}
	else {
		std::cout << "Point.figure value error." << std::endl;
		exit(1);
	}
}

void Point::init_candi() {
	candi_count = 9;
	for (int i = 0; i < 9; i++) {
		candidates[i] = 1;
	}
}

bool Point::remove_candi(int f) {
	if (f <= 0 || f > 9) {
		std::cout << "Point.remove_candi figure value error: " << f << std::endl;
	}
	if (candi_count >= 1) {
		if (candidates[f - 1] == 1) {
			candi_count--;
		}
		candidates[f-1] = 0;
		return true;
	}
	else {
		return false;
	}
}

