#pragma once
#include "stdafx.h"

vector<vector<int>> sudoku2matrix(int sudoku[9][9]);
void matrix2result(int sudoku[9][9], vector<int> & ans, vector<vector<int>> & mat);
void print_sudoku(int sudoku[9][9]);