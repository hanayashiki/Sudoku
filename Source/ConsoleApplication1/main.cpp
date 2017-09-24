#include "stdafx.h"
#include <iostream>
const int CREATE = 1;
const int SOLVE = 2;
using namespace std;

FILE* input;

Matrix matrix = Matrix();
Stack changes = Stack();
Stack roll_back = Stack();

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

void solve() {
	int x, y, count;
	char candi_buf[10] = "";
	Point * p = NULL;
	//
	//cout << x << ", " << y << ", " << count << endl;
	while (true)
	{
		matrix.get_min_point(x, y, count);
		vector<Point *> ranking = matrix.rank();
		int ranking_size = ranking.size();
		if (ranking_size == 0) {
			p = NULL;
			goto rollback;
		}
		//cout << "changes add " << ranking_size << "elements." << endl;
		for (int i = ranking_size-1; i >= 0; i--) {
			p = ranking.at(i);
			p->show_candidates(candi_buf);
			//cout << "get candidates: " << candi_buf;
			int point_x, point_y;
			p->get_pos(&point_x, &point_y);
			for (int i = 0; candi_buf[i] != '\0'; i++)
			{
				int to_fill = candi_buf[i] - '0';
				Change* subchange = new Change(point_x, point_y, to_fill);
				changes.push(subchange);
				//std::cout << "push " << i << endl;
			}
		}
		Change* change_imple;
		changes.pop(change_imple);
		matrix.fill_in(change_imple);
		//cout << "fill in:" << endl;
		//matrix.display();
		p = matrix.get_min_point(x, y, count);
		change_imple->set_trials(count);
		change_imple->display("Augment1: ");
		roll_back.push(change_imple);

		rollback:
		if (matrix.get_zeroes() == 0) {
			cout << "result:" << endl;
			matrix.display();
			return;
		}
		if (p == NULL)
		{
			Change* stack_top;
			cout << "No trials" << endl;
			matrix.display();
			cout << matrix.get_zeroes() << endl;
			while (true) {
				if (roll_back.get_top(stack_top))
				{
					stack_top->decre_trials();
					if (stack_top->get_trials() <= 0)
					{
						cout << "matrix rollback." << endl;
						roll_back.pop(stack_top);
						matrix.roll_back(stack_top);
						stack_top->display("Roll back: ");
						delete stack_top;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			changes.pop(stack_top);
			matrix.fill_in(stack_top);
			p = matrix.get_min_point(x, y, count);
			stack_top->set_trials(count);
			roll_back.push(stack_top);
			stack_top->display("Augment2: ");
			matrix.display();
			//getchar();
		}
	}
}