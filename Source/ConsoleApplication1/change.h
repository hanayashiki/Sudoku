#pragma once
class Change {
private:
	// 1 <= tg_x <= 9; same for tg_y
	int tg_x;
	int tg_y;
	int fill_num;
	int trials;
public:
	Change(int x, int y, int num);
	void set_trials(int t);
	bool decre_trials(); // trials > 0 return true else false
	void get_change(int &x, int &y, int &fig) const;
};