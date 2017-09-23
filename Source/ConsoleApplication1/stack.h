#pragma once

typedef int Item;

class Stack 
{
private:
	enum {STACKSIZE = 100};
	Item items[STACKSIZE];
	int top;
public:
	Stack();
	bool isempty() const;
	bool isfull() const;
	bool push(const Item & item);
    // push() returns false if stack already is full, true otherwise
	bool pop(Item & item);
	// pop top into item
	int size();
	int count();
};