#ifndef __CTICTACTOE_H__
#define __CTICTACTOE_H__

#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <windows.h>

using namespace std;

#define RET_OK          0
#define NOT_FINISHED   -1
#define TIED           -2
#define SAVE           -2
#define EXIT           -3

typedef enum enum_input { ROW = 0, COL } _input_type;

typedef struct {
	int row;
	int col;
	int role;
	clock_t t;
	unsigned long checksum;
} _step;

class CTictactoe
{
public:
	CTictactoe();
	~CTictactoe() {}

public:
	int initialize(int players_num, int board_length, int win_condition);
	char get_current_role() const { return m_roles[m_turn % m_players_num]; }
	int move(int row, int col, int role = -1, clock_t t = -1);
	int judge();
	void display() const;
	void show_result();
	void clear();

	int get_row_col(int &row, int &col);

	int save(const string &filename);
	int load(const string &filename, int speed);

	static int check_string_number(const string &str);
	static int check_filename(const string &filename);
	template<class T> static int get_number_length(T number);
	static string align_center(const string &str, int width);
	static string align_center(char ch, int width);
	static string align_center(int val, int width);

private:
	int get_input(_input_type in);
	void gotoxy(int row, int col) const;
	void display_info(const string &msg) const;
	void show_info(const string &msg);
	void clear_info();

	void clear_input(_input_type in);

private:
	int m_players_num;
	int m_board_length;
	int m_win_condition;
	int m_turn;
	int m_winner;
	int m_row_head_width;
	int m_spot_width;
	vector<_step> m_steps;
	vector<int> m_board;
	clock_t m_clock;

	COORD m_pos_row;
	COORD m_pos_col;

	string m_row_input;
	string m_col_input;
	string m_msg;

	const static string m_roles;
};

#define TO_STR(val, str) do { \
	stringstream ss; \
	ss << (val); \
	ss >> (str); \
} while(0)

#define STR2INT(str, val) do { \
	stringstream ss; \
	ss << (str); \
	ss >> (val); \
} while(0)

#endif // __CTICTACTOE_H_