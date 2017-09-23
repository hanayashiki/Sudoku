#include "stdafx.h"

Point::Point(){
	figure = 0;
	init_candi();
}

void Point::fill_in(int f) {
	if (f >= 0 || f <= 9) {
		figure = f;
	}
	else {
		std::cout << "Point.figure value error." << std::endl;
	}
}

void Point::init_candi() {
	candi_count = 9;
	for (int i = 0; i < 9; i++) {
		candidates[i] = 1;
	}
}

bool Point::remove_candi(int figure) {
	if (figure <= 0 || figure > 9) {
		std::cout << "Point.remove_candi figure value error: " << figure << std::endl;
	}
	if (candi_count >= 1) {
		if (candidates[figure - 1] == 1) {
			candi_count--;
		}
		candidates[figure-1] = 0;
		return true;
	}
	else {
		return false;
	}
}

