#include "stdafx.h"
#include <iostream>
#define DEBUG 0
#define DEBUG2 0
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
void create(int c);

void create_fill_in(int*);

bool read_file(FILE* f);

#define random(x) (rand()%x)

void shuffle(char* str) {
	int l = strlen(str);
	for (int i = 0; i < l; i++) {
		int r = random(l);
		char randc = str[r];
		char tmp = str[i];
		str[i] = randc;
		str[r] = tmp;
	}
	//cout << str << endl;
}

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
			if (DEBUG) cout << "result " << case_ << ": " << endl;
			if (DEBUG) matrix.display();
			if (DEBUG) cout << "vcheck: " << matrix.check_validity() << endl;
			root->clean(root);
			matrix = Matrix();
			root = new Change(0, 0, 0);
		}
	}
	else if (mission == CREATE) {
		int count = 0;
		int r = sscanf_s(argv[2], "%d", &count);
		cout << "create." << endl;
		if (r > 0) {
			create(count);
		}
		else {
			cout << "Input error. -c should be followed by an integer." << endl;
			exit(0);
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
	else if (strcmp("-c", argv[1]) == 0) {
		return CREATE;
	}
}

bool read_file(FILE* f)
{
	int readin = 0;
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
	while (true) {
		repeat++;
		Point* p = matrix.get_min_point_fast();
		if (p) {
			Change* new_change = NULL;
			Change* last_change = NULL;
			p->show_candidates(candi_buf);
			//cout << candi_buf << " => ";
			//shuffle(candi_buf);
			//cout << candi_buf << endl;
			if (DEBUG2) cout << "candi_buf:" << candi_buf;
			int x, y;
			p->get_pos(&x, &y);
			for (int i = 0; candi_buf[i] != 0; i++) {
				new_change = new Change(x, y, candi_buf[i] - '0');
				new_change->set_next(last_change);
				new_change->set_base(now);
				last_change = new_change;
			}
			now->set_fchild(new_change);
			if (now == NULL) {
				matrix.display();
				exit(1);
			}
			now = now->get_fchild();
			matrix.fill_in(now);
			if (DEBUG2) now->display("fill in:");
			if (DEBUG2) matrix.display();
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

void create(int count) {
	int gene[9] = { 0 };
	for (gene[1] = 1; gene[1] <= 9; gene[1]++) {
		for (gene[2] = 1; gene[2] <= 9; gene[2]++) {
			for (gene[3] = 1; gene[3] <= 9; gene[3]++) {
				for (gene[4] = 1; gene[4] <= 9; gene[4]++) {
					for (gene[5] = 1; gene[5] <= 9; gene[5]++) {
						for (gene[6] = 1; gene[6] <= 9; gene[6]++) {
							for (gene[7] = 1; gene[7] <= 9; gene[7]++) {
								for (gene[8] = 1; gene[8] <= 9; gene[8]++) {
									if (count-- > 0) {
										matrix = Matrix();
										root = new Change(0, 0, 0);
										create_fill_in(gene);
										matrix.display();
										solve();
										matrix.display();
										matrix.dump(output);
										root->clean(root);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void create_fill_in(int* gene) {
	//id (7+7)%9 + 1
	matrix.fill_in_figure(1, 1, (7 + 7) % 9 + 1);
	matrix.fill_in_figure(2, 4, gene[1]);
	matrix.fill_in_figure(3, 7, gene[2]);
	matrix.fill_in_figure(4, 2, gene[3]);
	matrix.fill_in_figure(5, 6, gene[4]);
	matrix.fill_in_figure(6, 8, gene[5]);
	matrix.fill_in_figure(7, 3, gene[6]);
	matrix.fill_in_figure(8, 5, gene[7]);
	matrix.fill_in_figure(9, 9, gene[8]);
}