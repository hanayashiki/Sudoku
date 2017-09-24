#include "stdafx.h"

Stack::Stack()
{
	top = 0;
}

bool Stack::isempty() const
{
	if (top == 0) {
		std::cout << "stack empty" << std::endl;
	}
	return top == 0;
}

bool Stack::isfull() const
{
	return top == STACKSIZE;
}

bool Stack::push(const Item & item)
{
	if (top < STACKSIZE)
	{
		items[top++] = item;
		return true;
	}
	else 
	{
		std::cout << "stack full" << std::endl;
		getchar();
		return false;
	}
}

bool Stack::pop(Item & item)
{
	if (top > 0)
	{
		item = items[--top];
		return true;
	}
	else
	{
		std::cout << "stack pop failed" << std::endl;
		getchar();
		return false;
	}
}

bool Stack::get_top(Item & item) const
{
	if (top > 0)
	{
		item = items[top-1];
		return true;
	}
	else
	{
		return false;
	}
}

int Stack::size()
{
	return STACKSIZE;
}

int Stack::count()
{
	return top+1;
}
