#pragma once
#include "stdafx.h"

using namespace std;

struct Node {
	struct Node* up, *down, *left, *right, *col_root, *row_root;
	int x, y;
	int num; // priviledge for row roots, records the order of the row
	int size; // for column roots, records the number of nodes 
	Node(int i = -1) : num(i), size(0), x(0), y(0) {};//constructor  
};

class DLX {
public:
	DLX(vector<vector<int>> & matrix, int m, int n);
	~DLX();
	void init();
	void make_from(vector<vector<int>> & matrix);
	void cover(Node* n);
	void recover(Node* n);
	bool search(int k); //core algorithm
	vector<int> get_result() const 
	{
		return result; 
	}
	void display_row(int x) const;
	void display_col(int x) const;
private:
	Node* head;
	vector<int> result;
	int _row, _col;
	vector<Node*> pool;
};
