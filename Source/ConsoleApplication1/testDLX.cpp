#include "stdafx.h"

void show_result_m(DLX & dlx, vector<vector<int>> & mat);

int fucked_main() {
	int sudoku[9][9] =
	{
		4, 7, 0, 9, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 8, 0, 1,

		0, 0, 6, 0, 0, 0, 0, 0, 0,
		8, 0, 1, 0, 0, 2, 5, 0, 0,
		0, 0, 0, 7, 0, 0, 0, 4, 0,

		0, 0, 0, 0, 5, 1, 0, 0, 0,
		6, 0, 0, 0, 0, 0, 0, 7, 0,
		0, 0, 0, 0, 0, 8, 0, 0, 0
	};
	vector<vector<int>> mat = sudoku2matrix(sudoku);
	//cout << "mat.size: " << mat.size() << endl;
	DLX dlx(mat, mat.size(), 324);
	bool solve = dlx.search(0);
	//cout << "solved: " << solve << ";" << endl;
	//print_matrix(mat);
	show_result_m(dlx, mat);
	matrix2result(sudoku, dlx.get_result(), mat);
	print_sudoku(sudoku);
	getchar();
	return 0;
}

void show_result_m(DLX & dlx, vector<vector<int>> & mat) {
	int sum = 0;
	vector <int> ans = dlx.get_result();
	//cout << "result: " << ";" << endl;
	for (unsigned int i = 0; i < ans.size(); i++) {
		//cout << ans.at(i) << " ";
		//cout << "mat row: ";
		for (unsigned int j = 0; j < mat[ans.at(i) - 1].size(); j++) {
			sum += mat[ans.at(i) - 1][j];
			//cout << mat[ans.at(i) - 1][j] << " ";
		}
		//cout << endl;
	}
	cout << "sum: " << sum << endl;
}
