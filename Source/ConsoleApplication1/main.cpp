#include "stdafx.h"
#include <iostream>
#define DEBUG 1
const int CREATE = 1;
const int SOLVE = 2;
using namespace std;
int repeat = 0;
int case_ = 0;

FILE* input;
FILE* output;

Matrix matrix = Matrix();
Change* root = new Change(0, 0, 0);

int read_argv(int argc, char** argv);

void solve();

bool read_file(FILE* f);

int main(int argc, char** argv)
{
	int mission = read_argv(argc, argv);
	fopen_s(&output, "sudoku.txt", "w");
	if (mission == SOLVE) {
		fopen_s(&input, argv[2], "r");
		while (read_file(input)) {
			case_++;
			if (DEBUG) cout << "new round:" << endl;
			if (DEBUG) matrix.display();
			solve();
			matrix.dump(output);
			if (DEBUG) cout << "result:" << endl;
			if (DEBUG) matrix.display();
			if (DEBUG) cout << "vcheck: " << matrix.check_validity() << endl;
			root->clean(root);
			root = new Change(0, 0, 0);
		}
	}
	fclose(input);
	fclose(output);
	if (DEBUG) cout << "finished" << endl;
	//if (DEBUG) getchar();
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

bool read_file(FILE* f)
{
	int readin = 0;
	matrix.reset();
	for (int i = 1; i <= 9; i++) 
	{
		for (int j = 1; j <= 9; j++) 
		{
			int d;
			readin += fscanf_s(f, "%d", &d);
			matrix.fill_in_figure(i, j, d);
		}
	}
	//matrix.display();
	//if (DEBUG) cout << "readin = " << readin << endl;
	return readin == 81;
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
	//
	Change* now = root;
	//if (DEBUG) cout << "Start solving" << endl;
	while (true) {
		repeat++;
		//if (DEBUG && (repeat % 10000 == 0)) cout << "repeat: " << repeat << endl;
		Point* p = matrix.get_min_point();
		if (p) {
			Change* new_change = NULL;
			Change* last_change = NULL;
			p->show_candidates(candi_buf);
			int x, y;
			p->get_pos(&x, &y);
			//cout << "min at (" << x << ", " << y << ")" << endl;
			//if (DEBUG && (case_==2)) cout << "subtree: " << strlen(candi_buf) << endl;
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
					Change* base = now->get_base();
					matrix.roll_back(now->get_base());
					if (now->get_base()->get_next() != NULL) {
						now = now->get_base()->get_next();
//						base->clean_desc(base);
//						delete base;
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