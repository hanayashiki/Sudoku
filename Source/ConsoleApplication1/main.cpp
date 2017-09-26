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

int sudoku[9][9] = { 0 };

int read_argv(int argc, char** argv);

void create(int c);

void create_fill_in(int*);

bool read_file(FILE* f, int s[9][9]);

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
		while (read_file(input, sudoku)) {
			case_++;
			if (DEBUG) cout << "new round:" << endl;
			if (DEBUG) matrix.display();
			//cout << "read: " << endl;
			//print_sudoku(sudoku);
			DLX_solve(sudoku);
			dump_sudoku(output, sudoku);
			if (DEBUG) cout << "result " << case_ << ": " << endl;
			if (DEBUG) matrix.display();
			if (DEBUG) cout << "vcheck: " << matrix.check_validity() << endl;
			//cout << "cuts: " << cut_used << endl;
		}
		fclose(input);
	}
	else if (mission == CREATE) {
		int count = 0;
		int r = sscanf_s(argv[2], "%d", &count);
		//cout << "create." << endl;
		if (r > 0 && count > 0) {
			create(count);
		}
		else {
			cout << "Input error. -c should be followed by a positive integer." << endl;
			//getchar();
			exit(0);
		}
	}
	else {
		cout << "Please use -s or -c." << endl;
	}
	fclose(output);
	if (DEBUG) cout << "finished" << endl;
	//if (DEBUG) getchar();
    return 0;
}

int read_argv(int argc, char** argv)
{
	if (argc <= 1) {
		return -1;
	}
	if (strcmp("-s", argv[1]) == 0) {
		return SOLVE;
	}
	else if (strcmp("-c", argv[1]) == 0) {
		return CREATE;
	}
	return -1;
}

bool read_file(FILE* f, int sudoku[9][9])
{
	int readin = 0;
	for (int i = 0; i < 9; i++) 
	{
		for (int j = 0; j < 9; j++) 
		{
			int d;
			readin += fscanf_s(f, "%d", &d);
			sudoku[i][j] = d;
		}
	}
	//matrix.display();
	//if (DEBUG) cout << "readin = " << readin << endl;
	return readin == 81;
}

void create(int gene_count) {
	matrix.fill_in_figure(1, 1, (7 + 7) % 9 + 1);
	int level = 0;
//	int x, y, count;
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
		Point* p = matrix.get_min_point_fast();
		Change* cut_trial = NULL;
		if ((p && (p->get_candi_count() > 1))) {
			cut_trial = corner();
		}
		if (cut_trial) {
			//cout << "----------used cut-----------" << endl;
			cut_trial->set_base(now);
			cut_trial->set_next(NULL);
			now->set_fchild(cut_trial);
			//matrix.display();
			//cout << "------" << endl;
			matrix.fill_in(cut_trial);
			now = cut_trial;
			//matrix.display();
			// back to while head
		}
		else {
			//Point* p = matrix.get_min_point_fast();
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
					matrix.dump(output);
					fprintf(output, "\n");
					if (--gene_count == 0) {
						return;
					}
				}
			}
			else {
				//...todo: rollback£ý
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
	
}

void create_fill_in(int* gene) {
	//id (7+7)%9 + 1

}