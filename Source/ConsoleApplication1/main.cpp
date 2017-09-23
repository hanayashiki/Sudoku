#include "stdafx.h"
#include <iostream>
const int CREATE = 1;
const int SOLVE = 2;

FILE* input;

Matrix matrix = Matrix();

int read_argv(int argc, char** argv);

void solve();

void read_file(FILE* f);

int main(int argc, char** argv)
{
	using namespace std;
	cout << "fuck you" << endl;
	int mission = read_argv(argc, argv);
	if (mission == SOLVE) {
		fopen_s(&input, argv[2], "r");
		read_file(input);
		solve();
	}
	getchar();
    return 0;
}

int read_argv(int argc, char** argv)
{
	if (strcmp("-s", argv[1]) == 0) {
		return SOLVE;
	}
}

void read_file(FILE* f)
{
	for (int i = 1; i <= 9; i++) 
	{
		for (int j = 1; j <= 9; j++) 
		{
			int d;
			fscanf_s(f, "%d", &d);
			matrix.fill_in_figure(i, j, d);
		}
	}
	matrix.display();
}

void solve() {

}