#include "stdafx.h"

Matrix::Matrix() {
	zeroes = 81;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			table[i-1][j-1].set_pos(i, j);
		}
	}
}

void Matrix::fill_in(Change* c) {
	int x, y, figure;
	c->get_change(x, y, figure);
	Point & tg_point = table[x - 1][y - 1];
	tg_point.fill_in(figure);
	if (figure != 0) {
		zeroes--;
	}
	//std::cout << "(" << x << ", " << y << ", " << figure << ")" << std::endl;
}

void Matrix::fill_in_figure(int x, int y, int figure) {
	Point & tg_point = table[x - 1][y - 1];
	tg_point.fill_in(figure);
	if (figure != 0) {
		zeroes--;
	}
}

int Matrix::set_point_candi(int x, int y) {
	int zero_x = x - 1;
	int zero_y = y - 1;
	int group_x = zero_x / 3;
	int group_y = zero_y / 3;
	int v = 0;
	Point & tg_point = table[x-1][y-1];
	int point_cache[10][10];
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			point_cache[i][j] = get_point_value(i, j);
		}
	}
	tg_point.init_candi();
	// search local group
	for (int i = 0; i < 3; i++) {
		int x1 = 3 * group_x + i + 1;
		for (int j = 0; j < 3; j++) {
			int y1 = 3 * group_y + j + 1;
			v = point_cache[x1][y1];
			//std::cout << "(" << x << ", " << y << ")" << " checking " << v << " at (" << x1 << ", "<< y1 << ")" << std::endl;
			if (v != 0) {
				//if ((x == 1 && y == 3) || (x == 2 && y == 1))
				//	display();
				tg_point.remove_candi(v);
				//if ((x == 1 && y == 3) || (x == 2 && y == 1))
				//	display();
				//std::cout << "(" << x << ", "<< y << ")" << " removes " << v << std::endl;
			}
		}
	}
	// check the line
	for (int i = 1; i <= 9; i++) {
		v = point_cache[i][y];
		if (v != 0) {
			tg_point.remove_candi(v);
		}
	}
	// check the row
	for (int j = 1; j <= 9; j++) {
		v = point_cache[x][j];
		if (v != 0) {
			tg_point.remove_candi(v);
		}
	}
	return tg_point.get_candi_count();
}

inline int Matrix::get_point_value(int x, int y) const {
	const Point & tg_point = table[x - 1][y - 1];
	return tg_point.get_figure();
}

bool Matrix::roll_back(Change* c) {
	int x, y, num;
	c->get_change(x, y, num);
	int old_value = get_point_value(x, y);
	zeroes++;
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

Point * Matrix::get_min_point() {
	Point * min_p = NULL;
	int min_count = 9999;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			int count = set_point_candi(i, j);
			if (count < min_count && get_point_value(i, j) == 0 && count != 0) {
				//display();
				//std::cout << "min changed! " << std::endl;
				min_p = &table[i - 1][j - 1];
				min_count = count;
				//display();
			}
		}
	}
	return min_p;
}

std::vector<Point*> Matrix::rank() {
	using namespace std;
	vector <Point*> buckets[10];
	for (int i = 0; i < 10; i++) {
		buckets[i].clear();
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int count = table[i][j].get_candi_count();
			//cout << "get_candi_count (" << i + 1 << ", "<< j + 1 << ", " << count << ")" << endl;
			if (count >= 1 && count <= 9) {
				//cout << "bucket_push (" << i + 1 << ", " << j + 1 << ", " << count << ")" << endl;
				buckets[count].push_back(&table[i][j]);
			}
		}
	}
	ranking.clear();
	for (int i = 1; i <= 9; i++) {
		int size_bucket = buckets[i].size();
		for (int j = 0; j < size_bucket; j++) {
			ranking.push_back(buckets[i].at(j));
			//cout << "ranking_push: i = "<< i << "," << buckets[i].at(j)->get_figure() << endl;
			int p_x[1], p_y[1];
			buckets[i].at(j)->get_pos(p_x, p_y);
			//cout << "at: (" << *p_x << ", " << *p_y << ")" << endl;
		}
	}
	for (int i = 0; i < ranking.size(); i++) {
		Point* p = ranking.at(i);
		int p_x, p_y;
		p->get_pos(&p_x, &p_y);
		//cout << "(" << p_x << ", " << p_y << ")" << endl;
	}
	return ranking;
}

bool Matrix::check_validity() const{
	int record = 0;
	const int mask = 0x1ff; // 9 ones
	for (int group_x = 0; group_x < 3; group_x++) {
		for (int group_y = 0; group_y < 3; group_y++) {
			record = 0;
			for (int x = 3 * group_x; x < 3 * group_x + 3; x++) {
				for (int y = 3 * group_y; y < 3 * group_y + 3; y++) {
					int v = get_point_value(x + 1, y + 1);
					record |= 1 << (v-1);
				}
			}
			if (record != mask) {
				return false;
			}
		}
	}
	for (int line = 1; line <= 9; line++) {
		record = 0;
		for (int row = 1; row <= 9; row++) {
			int v = get_point_value(line, row);
			record |= 1 << (v - 1);
		}
		if (record != mask) {
			return false;
		}
	}
	for (int row = 1; row <= 9; row++) {
		record = 0;
		for (int line = 1; line <= 9; line++) {
			int v = get_point_value(line, row);
			record |= 1 << (v - 1);
		}
		if (record != mask) {
			return false;
		}
	}
	return true;
}