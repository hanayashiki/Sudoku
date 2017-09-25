#include "stdafx.h"

Matrix::Matrix() {
	zeroes = 81;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			table[i - 1][j - 1].set_pos(i, j);
			table[i - 1][j - 1].init_candi();
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			group_vectors[i][j] = 0;
		}
	}
	for (int i = 0; i < 9; i++) {
		row_vectors[i] = 0;
	}
	for (int i = 0; i < 9; i++) {
		column_vectors[i] = 0;
	}
}

void Matrix::fill_in(Change* c) {
	int x, y, figure;
	c->get_change(x, y, figure);
	fill_in_figure(x, y, figure);
	//std::cout << "(" << x << ", " << y << ", " << figure << ")" << std::endl;
}

void Matrix::fill_in_figure(int x, int y, int figure) {
	Point & tg_point = table[x - 1][y - 1];
	tg_point.fill_in(figure);
	if (figure != 0) {
		zeroes--;
		group_vectors[(x - 1) / 3][(y - 1) / 3] |= 1 << figure;
		row_vectors[x - 1] |= 1 << figure;
		column_vectors[y - 1] |= 1 << figure;
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

int Matrix::get_point_candi_fast(int x, int y) {
	int n = group_vectors[(x - 1) / 3][(y - 1) / 3] |
		row_vectors[x - 1] | column_vectors[y - 1];
	///count 1

	const int MASK1 = 0x55555555;
	const int MASK2 = 0x33333333;
	const int MASK4 = 0x0f0f0f0f;
	const int MASK8 = 0x00ff00ff;
	const int MASK16 = 0x0000ffff;

	n = (n & MASK1) + ((n >> 1) & MASK1);
	n = (n & MASK2) + ((n >> 2) & MASK2);
	n = (n & MASK4) + ((n >> 4) & MASK4);
	n = (n & MASK8) + ((n >> 8) & MASK8);
	n = (n & MASK16) + ((n >> 16) & MASK16);
	//display();
	//std::cout << "count " << x << ", " << y << " = " << 9-n << std::endl;

	return 9 - n;
}

inline int Matrix::get_point_value(int x, int y) const {
	const Point & tg_point = table[x - 1][y - 1];
	return tg_point.get_figure();
}

bool Matrix::roll_back(Change* c) {
	int x, y, figure;
	c->get_change(x, y, figure);
	int old_value = get_point_value(x, y);
	zeroes++;
	if (old_value != 0) {
		Point & tg_point = table[x - 1][y - 1];
		tg_point.fill_in(0);
		group_vectors[(x - 1) / 3][(y - 1) / 3] &= ~(1 << figure);
		row_vectors[x - 1] &= ~(1 << figure);
		column_vectors[y - 1] &= ~(1 << figure);
		return true;
	}
	else {
		//std::cout << "Roll back value error at: (" << x << ", " << y << ")" << std::endl;
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

void Matrix::dump(FILE* f) const {
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			fprintf(f, "%d", get_point_value(i, j));
			fprintf(f, "%c", (j == 9 ? '\n' : ' '));
		}
	}
	fprintf(f, "\n");
}


Point * Matrix::get_min_point() {
	Point * min_p = NULL;
	int min_count = 9999;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if (get_point_value(i, j) == 0) {
				int count = set_point_candi(i, j);
				if (count < min_count && count != 0) {
					//display();
					//std::cout << "min changed! " << std::endl;
					min_p = &table[i - 1][j - 1];
					min_count = count;
					//display();
				}
			}
		}
	}
	return min_p;
}

Point * Matrix::get_min_point_fast() {
	Point * min_p = NULL;
	int min_count = 9999;
	int min_x, min_y;
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if (get_point_value(i, j) == 0) {
				int count = get_point_candi_fast(i, j);
				if (count < min_count && count != 0) {
					//display();
					//std::cout << "min changed! " << std::endl;
					min_p = &table[i - 1][j - 1];
					min_count = count;
					min_x = i;
					min_y = j;
					//display();
				}
			}
		}
	}
	if (min_p) {
		//std::cout << "got count " << min_x << ", " << min_y << " = " << min_count << std::endl;
		set_point_candi(min_x, min_y);
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