#pragma once
#include "stdafx.h"
class Point {
private:
	int candidates[9];
	int candi_count;
	int figure;
	int x;
	int y;
public:
	Point();
	void fill_in(int c);
	void init_candi();
	bool remove_candi(int f); // f is the candidate itself.
	void set_candidates(int avail) {
		for (int i = 0; i < 9; i++) {
			avail = avail >> 1;
			candidates[i] = (avail & 0x1) == 1;
		}
	}
	int get_candi_count() const{
		return figure == 0 ? candi_count : 10;
	}
	inline int get_figure() const {
		return figure;
	}
	void show_candidates(char* buf) const {
		int j = 0;
		for (int i = 0; i < 9; i++) {
			if (candidates[i] != 0) {
				buf[j++] = i+1+'0';
			}
		}
		buf[j] = '\0';
	}
	void set_pos(int i, int j) {
		x = i;
		y = j;
	}
	void get_pos(int * i, int * j) const {
		*i = x;
		*j = y;
	}
};

