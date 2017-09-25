#include "stdafx.h"
#include <iostream>
const int CREATE = 1;
const int SOLVE = 2;
using namespace std;

FILE* input;

Matrix matrix = Matrix();
Change* root = new Change(0, 0, 0);

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
	cout << "result:" << endl;
	matrix.display();
	root->clean(root);
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
	bool first = true;
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
	Change* now = root;
	while (true) {
		Point* p = matrix.get_min_point();
		if (p) {
			Change* new_change = NULL;
			Change* last_change = NULL;
			p->show_candidates(candi_buf);
			int x, y;
			p->get_pos(&x, &y);
			//cout << "min at (" << x << ", " << y << ")" << endl;
			for (int i = 0; candi_buf[i] != 0; i++) {
				new_change = new Change(x, y, candi_buf[i] - '0');
				new_change->set_next(last_change);
				new_change->set_base(now);
				last_change = new_change;
			}
			now->set_fchild(new_change);
			now = now->get_fchild();
			matrix.fill_in(now);
			//now->display("fill in:");
			//matrix.display();
			if (matrix.get_zeroes() == 0) {
				return;
			}
		}
		else {
			//...todo: rollback
			//now->display("roll back:");
			matrix.roll_back(now);
			if (now->get_next() != NULL) {
				now = now->get_next();
			}
			else {
				while (now->get_base()) {
					//now->get_base()->display("roll back:");
					matrix.roll_back(now->get_base());
					if (now->get_base()->get_next() != NULL) {
						now = now->get_base()->get_next();
						break;
					}
					now = now->get_base();
				}
			}
			matrix.fill_in(now);
			//now->display("fill in:");
		}
	}
}