#include "stdafx.h"
extern Matrix matrix;

int find_lock(int i, int j, int group_i, int group_j);

void Binarycout(int n)
{
	using namespace std;
	for (int i = 31; i >= 0; i--)
	{
		cout << ((n >> i) & 1);
	}
	cout << endl;
}


Change* corner() {
	for (int group_i = 0; group_i < 3; group_i++) {
		for (int group_j = 0; group_j < 3; group_j++) {
			for (int di = 0; di < 3; di++) {
				for (int dj = 0; dj < 3; dj++) {
					int i = 3 * group_i + di;
					int j = 3 * group_j + dj;
					if (matrix.get_point_value(i + 1, j + 1) == 0) {
						int lock = find_lock(i, j, group_i, group_j);
						if (lock != 0) {
							int verif = (1 << lock) & (matrix.group_vectors[group_i][group_j]
								| matrix.row_vectors[i] | matrix.column_vectors[j]);
							if (verif == 0) {
								Change* c = new Change(i + 1, j + 1, lock);
								return c;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

int find_lock(int i, int j, int group_i, int group_j) {
	int other_column[2];
	int other_row[2];
	int other_group[2];
	/*if (i % 3 == 0) {
		other_row[0] = i + 1;
		other_row[1] = i + 2;
	}
	if (i % 3 == 1) {
		other_row[0] = i - 1;
		other_row[1] = i + 1;
	}
	if (i % 3 == 2) {
		other_row[0] = i - 2;
		other_row[1] = i - 1;
	}*/

	other_row[0] = (i + 1) % 3 + 3 * group_i;
	other_row[1] = (i + 2) % 3 + 3 * group_i;

	/*if (j % 3 == 0) {
		other_column[0] = j + 1;
		other_column[1] = j + 2;
	}
	if (j % 3 == 1) {
		other_column[0] = j - 1;
		other_column[1] = j + 1;
	}
	if (j % 3 == 2) {
		other_column[0] = j - 2;
		other_column[1] = j - 1;
	}*/

	other_column[0] = (j + 1) % 3 + 3 * group_j;
	other_column[1] = (j + 2) % 3 + 3 * group_j;

	/*if (group_j % 3 == 0) {
		other_group[0] = group_j + 1;
		other_group[1] = group_j + 2;
	}
	if (group_j % 3 == 1) {
		other_group[0] = group_j - 1;
		other_group[1] = group_j + 1;
	}
	if (group_j % 3 == 2) {
		other_group[0] = group_j - 2;
		other_group[1] = group_j - 1;
	}*/
	other_group[0] = (group_j + 1) % 3;
	other_group[1] = (group_j + 2) % 3;


	int vector_r0 = matrix.row_vectors[other_row[0]];
	int vector_r1 = matrix.row_vectors[other_row[1]];

	int vector_c0 = matrix.column_vectors[other_column[0]];
	int vector_c1 = matrix.column_vectors[other_column[1]];

	/*int vector_g0 = matrix.group_vectors[group_i][other_group[0]];
	int vector_g1 = matrix.group_vectors[group_i][other_group[1]];*/
	/*
	std::cout << "/" << std::endl;
	Binarycout(vector_g0);
	Binarycout(vector_g1);
	Binarycout(vector_c0);
	Binarycout(vector_c1);
	std::cout << "/" << std::endl;
	*/
	int and_result_rc = vector_r0 & vector_r1 & vector_c0 & vector_c1;
	if (and_result_rc != 0) {
		for (int i = 1; i <= 9; i++) {
			and_result_rc = and_result_rc >> 1;
			if ((and_result_rc & 0x1) == 0x1) {
				//std::cout << "cut 1" << std::endl;
				return i;
			}
		}
	}

	/*int and_result_gc = vector_g0 & vector_g1 & vector_c0 & vector_c1;
	if (and_result_gc != 0) {
		for (int i = 1; i <= 9; i++) {
			and_result_gc = and_result_gc >> 1;
			if ((and_result_gc & 0x1) == 0x1) {
				//std::cout << "cut 2" << std::endl;
				return i;
			}
		}
	}*/

	return 0;

}