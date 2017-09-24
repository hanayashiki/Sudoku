#include "stdafx.h"
#include <iostream>
const int CREATE = 1;
const int SOLVE = 2;
using namespace std;

FILE* input;

Matrix matrix = Matrix();

int read_argv(int argc, char** argv);

void solve();

void read_file(FILE* f);

int main(int argc, char** argv)
{
	int mission = read_argv(argc, argv);
	if (mission == SOLVE) {
		fopen_s(&input, argv[2], "r");
		read_file(input);
		solve();
	}
	fclose(input);
	cout << "result:";
	matrix.display();
	cout << "vcheck: " << matrix.check_validity() << endl;
	getchar();
    return 0;
}

int read_argv(int argc, char** argv)
{
	if (strcmp("-s", argv[1]) == 0) {
		return SOLVE;
	}
	else {
		return CREATE;
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

void multi_print(char c, int n) {
	for (int i = 0; i < n; i++) {
		cout << c;
	}
}

void solve() {
	int level = 0;
	int x, y, count;
	bool first;
	char candi_buf[10] = "";
	//
	//cout << x << ", " << y << ", " << count << endl;
	//1. build up root
	//2. get a min point, if failed, rollback root's change, 
	//   try root.next (if null, rollback) or root.base (not null, otherwise quit)
	//3. build possible changes, linking them to the root, mark root as expanded
	//4. set now to be root's fchild
	//5. implement change
	//6. go to 2
	Change* root = new Change(0, 0, 0);
	Change* now;
	while (true) {
		Point* p = matrix.get_min_point();
		if (p) {
			Change* new_change = NULL;
			Change* last_change = NULL;
			p->show_candidates(candi_buf);
			for (int i = 0; candi_buf[i] != 0; i++) {
				int x, y;
				p->get_pos(&x, &y);
				new_change = new Change(x, y, candi_buf[i] - '0');
				new_change->set_next(last_change);
				new_change->set_base(root);
				last_change = new_change;
			}
			root->set_fchild(new_change);
			if (first) {
				first = false;
				now = root->get_fchild();
			}
			matrix.fill_in(now);
			if (matrix.get_zeroes() == 0) {
				return;
			}
		}
		else {
			//...todo: rollback
		}
	}
}