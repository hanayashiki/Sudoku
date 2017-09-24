#pragma once
#include "stdafx.h"

typedef Change * Item;

class Stack 
{
private:
	enum {STACKSIZE = 400000};
	Item items[STACKSIZE];
	int top;
	bool delete_when_pop;
public:
	Stack();
	bool isempty() const;
	bool isfull() const;
	bool push(const Item & item);
    // push() returns false if stack already is full, true otherwise
	bool pop(Item & item);
	// pop top into item
	bool get_top(Item & item) const;
	int size();
	int count();
};