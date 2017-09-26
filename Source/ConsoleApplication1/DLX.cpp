#include "stdafx.h"
using namespace std;

void DLX::display_row(int x) const{
	//find row
	Node* current_row = head->down;
	for (int i = 0; i < x; i++) {
		current_row = current_row->down;
	}
	Node* current_node = current_row->right;
	cout << "Row " << x << ": ";
	for (int j = 0; j < current_row->size; j++) {
		cout << "(" << current_node->x << ", "<< current_node->y <<") :"<< 1 << " ";
		current_node = current_node->right;
	}
	cout << ";" << endl;
}

void DLX::display_col(int x) const {
	//find row
	Node* current_col = head->right;
	for (int i = 0; i < x; i++) {
		current_col = current_col->right;
	}
	Node* current_node = current_col->down;
	cout << "Col " << x << ": ";
	for (int j = 0; j < current_col->size; j++) {
		cout << "(" << current_node->x << ", " << current_node->y << ") :" << 1 << " ";
		current_node = current_node->down;
	}
	cout << ";" << endl;
}

DLX::DLX(vector<vector<int>> &matrix, int m, int n)
	: _row(m), _col(n), _updates(0)
{
	head = new Node;
	head->left = head;
	head->right = head;
	head->up = head;
	head->down = head;
	init();
	make_from(matrix);
}

void DLX::init() {
	Node* new_node;
	for (int x = 0; x < _col; x++) {
		new_node = new Node;
		new_node->up = new_node;
		new_node->down = new_node;
		new_node->right = head->right;
		new_node->left = head;

		new_node->right->left = new_node;
		head->right = new_node;
	}
	for (int y = 0; y < _row; y++) {
		new_node = new Node(_row - y);
		new_node->down = head->down;
		new_node->up = head;
		new_node->right = NULL;
		new_node->left = NULL;

		new_node->down->up = new_node;
		head->down = new_node;
	}
}

void DLX::make_from(vector<vector<int>> & matrix) {
	Node *current_row, *current_col, *new_node, *current_row_first;
	current_row = head;
	for (int row = 0; row < _row; row++) {
		current_row = current_row->down;
		current_col = head;
		for (int col = 0; col < _col; col++) {
			//cout << "implement" << row << "," << col << endl;
			current_col = current_col->right;
			if (matrix[row][col] == 0) {
				continue;
			}
			new_node = new Node;
			//set node
			new_node->x = row;
			new_node->y = col;
			//set root
			new_node->col_root = current_col;
			new_node->row_root = current_row;
			//insert 
			new_node->down = current_col;
			new_node->up = current_col->up; // get tail
			new_node->up->down = new_node; // attach to tail
			current_col->up = new_node; // becomes new tail

			if (current_row->size == 0) {
				current_row->right = new_node;
				new_node->left = new_node;
				new_node->right = new_node;
			}

			current_row->size++;
			current_row_first = current_row->right;
			new_node->left = current_row_first->left;
			new_node->right = current_row_first; // tail attaches to first current_row_first
			new_node->left->right = new_node;
			current_row_first->left = new_node;

			current_col->size++;
		}
	}
}

void DLX::cover(Node* c_root) // close a colomn and relevant rows
{
	_updates++;
	c_root->left->right = c_root->right;
	c_root->right->left = c_root->left;
	Node *i, *j;
	i = c_root->down;
	while (i != c_root)
	{
		j = i->right;
		while (j != i)
		{
			j->down->up = j->up;
			j->up->down = j->down;
			j->col_root->size--;
			j = j->right;
			//cout << "close: " << "(" << j->x << "," << j->y << ")" << endl;
		}
		i = i->down;
	}
}

void DLX::recover(Node* c_root)
{
	Node *i, *j;
	i = c_root->up;
	while (i != c_root)
	{
		j = i->left;
		while (j != i)
		{
			j->col_root->size++;
			j->down->up = i;
			j->up->down = j;
			j = j->left;
			//cout << "open: " << "(" << j->x << "," << j->y << ")" << endl;
		}
		i = i->up;
	}
	c_root->right->left = c_root;
	c_root->left->right = c_root;
}

bool DLX::search(int k)
{
	///cout << "search: " << k << endl;
	if (head->right == head) {
		return true;
	}
	int min_size = INT_MAX;
	Node *c = head->right;
	Node *c_root = c;
	while (c != head) {
		//find the row of smallest size
		if (c->size < min_size) {
			min_size = c->size;
			c_root = c;
			if (min_size == 1) {
				break;
			}
			if (min_size == 0) {
				return false;
			}
		}
		c = c->right;
	}
	cover(c_root); //close the colomn and relevant rows 

	Node *current_row, *current;
	for (current_row = c_root->down; current_row != c_root; 
		 current_row = current_row->down)  // try adding each row
		{
		result.push_back(current_row->row_root->num); // try regarding it as an answer
		//cout << "result_push: " << current_row->row_root->num << "; " << endl;
		for (current = current_row->right; current != current_row;
			 current = current->right) 
		{
			//cout << "second cover" << endl;
			cover(current->col_root);
		}
		if (search(k + 1)) {
			return true;
		}
		for (current = current_row->left; current != current_row;
			current = current->left)
		{
			//cout << "second recover" << endl;
			recover(current->col_root);
		}
		result.pop_back();
	}
	recover(c_root);
	return false;
}