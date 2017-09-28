#include "stdafx.h"

//Arrangements:
//Matrix: ? * (place = 81 + (row = 9) * 9 + (column = 9) * 9 + (group = 9) * 9)

vector<vector<int>> sudoku2matrix(int sudoku[9][9]) {
	vector<vector<int>> mat;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int entry = sudoku[i][j];
			int fill = entry;
			do {
				if (entry == 0)
					fill++;
				//cout << (3 * (i / 3) + j / 3) << endl;
			    vector<int> new_row(81 + 9 * 9 + 9 * 9 + 9 * 9, 0);
				int pos = 9 * i + j;
				new_row[pos] = 1;
				new_row[81 + i * 9 + fill - 1] = 1;
				new_row[81 + 81 + j * 9 + fill - 1] = 1;
				new_row[81 + 81 + 81 + (3 * (i / 3) + j / 3) * 9 + fill - 1] = 1;
				mat.push_back(new_row);
			} while ((entry == 0) && (fill < 9));
		}
	}
	return mat;
}

void print_matrix(vector<vector<int>> & m) {
	for (unsigned int i = 0; i < m.size(); i++) {
		for (int j = 0; j < 324; j++) {
			cout << m[i][j];
		}
		cout << ";" << endl;
	}
}

void matrix2result(int sudoku[9][9], vector<int> & ans, vector<vector<int>> & mat) {
	int sz = ans.size();
	for (int i = 0; i < sz; i++) {
		//get vector
		vector<int> m_r = mat.at(ans.at(i)-1);
		//get pos
		int sz_m_r = m_r.size();
		int info_poses[4];
		int k = 0;
		for (int j = 0; j < sz_m_r; j++) {
			if (m_r.at(j) == 1) {
				info_poses[k] = j;
				k++;
			}
		}
		int row = info_poses[0] / 9;
		int col = info_poses[0] % 9;
		int figure = info_poses[1] % 9 + 1;
		sudoku[row][col] = figure;
	}
}

void print_sudoku(int sudoku[9][9]) {
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			std::cout << sudoku[i][j] << (j == 8 ? '\n' : ' ');
		}
	}
}

void DLX_solve(int sudoku[9][9]) {
	vector<vector<int>> mat = sudoku2matrix(sudoku);
	//cout << "mat.size: " << mat.size() << endl;
	DLX dlx(mat, mat.size(), 324);
	bool solve = dlx.search(0);
	//cout << "solved: " << solve << ";" << endl;
	//print_matrix(mat);
	matrix2result(sudoku, dlx.get_result(), mat);
	//print_sudoku(sudoku);
}

void dump_sudoku(FILE* output, int sudoku[9][9]) {
	cout << "dump" << endl;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fprintf(output, "%d", sudoku[i][j]);
			fputc((j == 8 ? '\n' : ' '), output);
		}
	}
}

bool check_validity(int sudoku[9][9]) {
	int record = 0;
	const int mask = 0x1ff; // 9 ones
	for (int group_x = 0; group_x < 3; group_x++) {
		for (int group_y = 0; group_y < 3; group_y++) {
			record = 0;
			for (int x = 3 * group_x; x < 3 * group_x + 3; x++) {
				for (int y = 3 * group_y; y < 3 * group_y + 3; y++) {
					int v = sudoku[x][y];
					record |= 1 << (v - 1);
				}
			}
			if (record != mask) {
				return false;
			}
		}
	}
	for (int line = 0; line < 9; line++) {
		record = 0;
		for (int row = 0; row < 9; row++) {
			int v = sudoku[line][row];
			record |= 1 << (v - 1);
		}
		if (record != mask) {
			return false;
		}
	}
	for (int row = 0; row < 9; row++) {
		record = 0;
		for (int line = 0; line < 9; line++) {
			int v = sudoku[line][row];
			record |= 1 << (v - 1);
		}
		if (record != mask) {
			return false;
		}
	}
	return true;
}
