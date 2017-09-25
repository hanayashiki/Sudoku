#pragma once
class Change {
private:
	// 1 <= tg_x <= 9; same for tg_y
	int tg_x;
	int tg_y;
	int fill_num;

	Change* base;
	Change* next;
	Change* fchild;
public:
	bool expanded;
	bool validity;

	Change(int x, int y, int num);
	void get_change(int &x, int &y, int &fig) const;
	void display(char * s);
	void set_base(Change* c) {
		base = c;
	}
	void set_next(Change* c) {
		next = c;
	}
	void set_fchild(Change* c) {
		fchild = c;
	}
	Change* get_base() {
		return base;
	}
	Change* get_next() {
		return next;
	}
	Change* get_fchild() {
		return fchild;
	}
	void clean(Change* c) {
		if (c->get_fchild()) {
			clean(c->get_fchild());
		}
		if (c->get_next()) {
			clean(c->get_next());
		}
		delete c;
	}
};