#include "tictactoe.h"

// show the symbol at the board
const string CTictactoe::m_roles = "XOABCDEFGHIJKLMNPQRSTUVWYZ ";

CTictactoe::CTictactoe() : m_players_num(0), m_board_length(0), m_win_condition(0), m_turn(0), m_winner(NOT_FINISHED), m_row_head_width(0), m_spot_width(3), m_clock(0), m_row_input(""), m_col_input(""), m_msg("")
{
}

int CTictactoe::initialize(int players_num, int board_length, int win_condition)
{
	if (players_num > 26 || players_num < 2) {
		return -1;
	}
	if (board_length < win_condition || board_length * board_length < players_num * win_condition) {
		return -2;
	}

	m_players_num = players_num;
	m_board_length = board_length;
	m_win_condition = win_condition;

	m_row_head_width = get_number_length<int>(board_length);       // used to display the row number
	m_spot_width = m_row_head_width > 3 ? m_row_head_width : 3;    // used to show the width of spots

	// the position of the input field of row and col
	m_pos_row.X = 5;
	m_pos_row.Y = m_board_length * 2 + 2;
	m_pos_col.X = 8;
	m_pos_col.Y = m_board_length * 2 + 3;

	// record the status of the game to judge the winning
	m_board.clear();
	for (int i = 0; i < board_length * board_length; ++i) {
		m_board.push_back(26);
	}

	m_steps.clear();

	m_turn = 0;
	m_winner = NOT_FINISHED;
	m_clock = clock();
	m_row_input.clear();
	m_col_input.clear();
	m_msg.clear();
	return RET_OK;
}

/* input the row and col number to put specific symbol to specific position */
int CTictactoe::move(int row, int col, int role, clock_t t)
{
	if (row <= 0 || row > m_board_length ||
		col <= 0 || col > m_board_length) {
		return -1;
	}

	_step s;
	s.row = row - 1;
	s.col = col - 1;
	s.role = role == -1 ? (m_turn % m_players_num) : role;
	s.t = t == -1 ? (clock() - m_clock) : t;
	s.checksum = s.row + s.col + s.role + s.t;

	// return -1 if the position has been occupied
	if (m_board[s.row * m_board_length + s.col] != 26) {
		return -2;
	}

	// record game status
	m_board[s.row * m_board_length + s.col] = s.role;

	// record every step to replay
	m_steps.push_back(s);

	// user to record the users turn
	++m_turn;

	return RET_OK;
}

/* judge if the game is over */
int CTictactoe::judge()
{
	int sum1(0), sum2(0), sum3(0), sum4(0);
	int start_row(0), start_col_l(0), start_col_r(0);
	int win_square = m_win_condition - 1;
	m_winner = NOT_FINISHED;
	for (int role = 0; role < m_players_num; ++role) {
		for (int i = 0; i < m_board_length; ++i) {
			sum1 = sum2 = 0;
			for (int j = 0; j < m_board_length; ++j) {
				sum1 = m_board[i * m_board_length + j] == role ? sum1 + 1 : 0;    // judge if the row meet winning condition
				sum2 = m_board[j * m_board_length + i] == role ? sum2 + 1 : 0;    // judge if the col meet winning conditon
				if (sum1 == m_win_condition || sum2 == m_win_condition) {
					m_winner = role;
					return m_winner;
				}

				/* Allow winning by a square whose side dimensions are n - 1, where n is the win sequence count. */
				if (sum1 == win_square && i + win_square - 1 < m_board_length) {
					int a(0), b(0), c(0);
					for (int k = 0; k < win_square; ++k) {
						a = m_board[(i + k) * m_board_length + j] == role ? a + 1 : 0;
						b = m_board[(i + k) * m_board_length + j - win_square + 1] == role ? b + 1 : 0;
						c = m_board[(i + win_square - 1) * m_board_length + j - k] == role ? c + 1 : 0;
					}
					if (a == win_square && b == win_square && c == win_square) {
						m_winner = role;
						return m_winner;
					}
				}
			}
		}

		/* the diagnoal winning */
		start_row = 0;
		start_col_l = 0;
		start_col_r = m_board_length - 1;
		int row_tmp(0), col_l_tmp(0), col_r_tmp(0);
		do {
			row_tmp = start_row;
			col_l_tmp = start_col_l;
			col_r_tmp = start_col_r;
			sum1 = sum2 = 0;
			do {
				sum1 = m_board[row_tmp * m_board_length + (col_l_tmp++)] == role ? sum1 + 1 : 0;
				sum2 = m_board[row_tmp * m_board_length + (col_r_tmp--)] == role ? sum2 + 1 : 0;
				--row_tmp;
			} while (row_tmp >= 0 && row_tmp < m_board_length &&
					 col_l_tmp >= 0 && col_l_tmp < m_board_length &&
					 col_r_tmp >= 0 && col_r_tmp < m_board_length);
			if (sum1 == m_win_condition || sum2 == m_win_condition) {
				m_winner = role;
				return m_winner;
			}

			if (start_row < m_board_length) {
				++start_row;
			}
			else {
				++start_col_l;
				--start_col_r;
			}
		} while (start_row >= 0 && start_row < m_board_length &&
				 start_col_l >= 0 && start_col_l < m_board_length &&
				 start_col_r >= 0 && start_col_r < m_board_length);
	}
	if (m_turn >= m_board_length * m_board_length)
		m_winner = TIED;
	return m_winner;
}

/* show the board and the user interface */
void CTictactoe::display() const
{
	string row_header(m_row_head_width, ' ');
	system("cls");
	for (int i = 0; i < m_board_length * 2; ++i) {
		for (int j = 0; j < m_board_length * 2; ++j) {
			if (0 == j) {
				cout.flags(ios::right);
				if (0 == i % 2) cout << setw(m_row_head_width) << " ";
				else cout << setw(m_row_head_width) << i / 2 + 1;
				cout.flags(ios::adjustfield);
			}
			else if (0 == i) {
				if (0 == j % 2) cout << " ";
				else cout << align_center(j / 2 + 1, m_spot_width);
			}
			else if (0 == i % 2) {
				if (0 == j % 2) cout << "+";
				else cout << setfill('-') << setw(m_spot_width) << '-';
				cout << setfill(' ');
			}
			else {
				if (0 == j % 2) cout << "|";
				else cout << align_center(m_roles[m_board[i / 2 * m_board_length + j / 2]], m_spot_width);
			}
		}
		cout << endl;
	}
	cout << endl;

	cout << "Turn: " << get_current_role() << endl;

	cout << "row:   " << endl;
	cout << "column: " << endl;

	cout << endl << endl << endl << endl;
	cout << "Tips:" << endl;
	cout << "1. Input 'save' or 'exit' to save your game or exit the game." << endl;
	cout << "2. Only need input the filename once when save a new game ." << endl;
	cout << "3. Allow winning by a square whose side dimensions is win_condition - 1. And the square is hollow." << endl;
}

/* when game is over, show the result */
void CTictactoe::show_result()
{
	display();
	string str_result("");
	if (NOT_FINISHED == m_winner) {
		return;
	}
	else if (TIED == m_winner) {
		str_result = "Game is tied!";
	}
	else {
		str_result = " ";
		str_result[0] = m_roles[m_winner];
		str_result += " wins!";
	}
	show_info(str_result);
}

/* clear the game status to start a new game */
void CTictactoe::clear()
{
	initialize(m_players_num, m_board_length, m_win_condition);
}

/* ask the user to input row or col number */
int CTictactoe::get_input(_input_type in)
{
	int x = (in == ROW) ? m_pos_row.X : m_pos_col.X;
	int y = (in == ROW) ? m_pos_row.Y : m_pos_col.Y;
	string &input = (in == ROW) ? m_row_input : m_col_input;
	int number = -1;
	while (1) {
		clear_input(in);
		getline(cin, input);   // if using "cin >> a", there will be a problem when inputting space
		if (4 == input.length() && input == "save") return SAVE;
		if (4 == input.length() && input == "exit") return EXIT;
		if (RET_OK != check_string_number(input)) {
			show_info("Only number or 'save/exit' can be accepted! Try again!");
			continue;
		}
		clear_info();
		STR2INT(input, number);
		if (number <= 0 || number > m_board_length) {
			// clear old error message before showing new error message
			TO_STR(m_board_length, m_msg);
			m_msg = "The range of row is [1, " + m_msg + "]! Try again!";
			show_info(m_msg);
			continue;
		}
		// if inputting a correct number, clear the error message and jump out the while loop
		clear_info();
		break;
	}

	return number;
}

/* the public function for invoking */
int CTictactoe::get_row_col(int &row, int &col)
{
	row = col = -1;
	row = get_input(ROW);
	if (SAVE == row || EXIT == row) {
		return RET_OK;
	}

	col = get_input(COL);
	if (SAVE == col || EXIT == col) {
		return RET_OK;
	}

	if (m_board[(row - 1) * m_board_length + (col - 1)] != 26) {
		show_info("This spot is not available!Try again!");
		clear_input(ROW);
		clear_input(COL);
		return -1;
	}

	return RET_OK;
}

/* save the game status */
int CTictactoe::save(const string &filename)
{
	if (RET_OK != CTictactoe::check_filename(filename))
		return -1;

	ofstream outfile;
	outfile.open(filename + ".gamedata", ofstream::binary);
	if (!outfile) return -2;

	unsigned long file_size = sizeof(_step) * m_steps.size() + sizeof(file_size) + sizeof(m_players_num) + sizeof(m_board_length) + sizeof(m_win_condition);

	int ret = RET_OK;

	do {
		outfile.write(reinterpret_cast<char*>(&file_size), sizeof(file_size));
		if (outfile.bad() || outfile.fail()) {
			ret = -3;
			break;
		}
		outfile.write(reinterpret_cast<char*>(&m_players_num), sizeof(m_players_num));
		if (outfile.bad() || outfile.fail()) {
			ret = -3;
			break;
		}
		outfile.write(reinterpret_cast<char*>(&m_board_length), sizeof(m_board_length));
		if (outfile.bad() || outfile.fail()) {
			ret = -3;
			break;
		}
		outfile.write(reinterpret_cast<char*>(&m_win_condition), sizeof(m_win_condition));
		if (outfile.bad() || outfile.fail()) {
			ret = -3;
			break;
		}

		vector<_step>::iterator it = m_steps.begin();
		for (; it != m_steps.end(); ++it) {
			outfile.write(reinterpret_cast<char*>(&(*it)), sizeof(*it));
			if (outfile.bad() || outfile.fail()) {
				ret = -3;
				break;
			}
		}
	} while (0);

	outfile.close();
	return ret;
}

/* load a saved game */
int CTictactoe::load(const string &filename, int speed)
{
	if (RET_OK != CTictactoe::check_filename(filename))
		return -1;

	ifstream infile;
	infile.open(filename + ".gamedata", ifstream::binary | ifstream::ate);
	if (!infile) return -2;
	
	unsigned long file_size;
	unsigned long size = static_cast<unsigned long>(infile.tellg());
	infile.seekg(ifstream::beg);

	int ret = RET_OK;

	do {
		infile.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));
		if (infile.bad() || infile.fail()) {
			ret = -3;
			break;
		}
		if (file_size != size) {
			ret = -4;
			break;
		}
		infile.read(reinterpret_cast<char*>(&m_players_num), sizeof(m_players_num));
		if (infile.bad() || infile.fail()) {
			ret = -3;
			break;
		}
		infile.read(reinterpret_cast<char*>(&m_board_length), sizeof(m_board_length));
		if (infile.bad() || infile.fail()) {
			ret = -3;
			break;
		}
		infile.read(reinterpret_cast<char*>(&m_win_condition), sizeof(m_win_condition));
		if (infile.bad() || infile.fail()) {
			ret = -3;
			break;
		}

		clear();
		_step step;
		clock_t last_t = 0;
		while (infile.good() && !infile.eof() && EOF != infile.peek()) {
			infile.read(reinterpret_cast<char*>(&step), sizeof(step));
			if (infile.bad() || infile.fail()) {
				ret = -3;
				break;
			}
			// check whether the file is modified illegally or not
			if (step.checksum != (step.row + step.col + step.role + step.t)) {
				clear();
				ret = -4;
				break;
			}
			// replay the game with the specific speed
			if (speed > 0) {
				display();
				show_info("Replaying...");
				Sleep((step.t - last_t) / speed);
			}

			// read one step, move one step
			if (RET_OK != move(step.row + 1, step.col + 1, step.role, step.t)) {
				clear();
				ret = -5;
				break;
			}
			last_t = step.t;
		}
	} while (0);

	infile.close();
	return ret;
}

/* change the position of cursor */
inline void CTictactoe::gotoxy(int x, int y) const
{
	COORD C;
	C.X = x;
	C.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), C);
}

/* show some info at the bottom */
inline void CTictactoe::display_info(const string &msg) const
{
	HANDLE consolehwnd;
	consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_INTENSITY);
	gotoxy(0, m_board_length * 2 + 5);
	cout << msg << endl;
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

/* show some info at the bottom */
inline void CTictactoe::show_info(const string &msg)
{
	string str = msg;
	clear_info();
	m_msg = str;
	display_info(m_msg);
}

/* clear the info */
inline void CTictactoe::clear_info()
{
	m_msg.replace(0, m_msg.size(), m_msg.size(), ' ');
	display_info(m_msg);
	m_msg.clear();
}

/* clear the users input field */
inline void CTictactoe::clear_input(_input_type in)
{
	int x = (in == ROW) ? m_pos_row.X : m_pos_col.X;
	int y = (in == ROW) ? m_pos_row.Y : m_pos_col.Y;
	string &input = (in == ROW) ? m_row_input : m_col_input;
	input.replace(0, input.size(), input.size(), ' ');
	gotoxy(x, y);
	cout << input;
	gotoxy(x, y);
}

/* check whether if string contains number only or not */
int CTictactoe::check_string_number(const string &str)
{
	string::size_type index = 0;
	for (; index != str.size(); ++index) {
		if (str[index] < 0x30 || str[index] > 0x39) {
			return -1;
		}
	}
	return RET_OK;
}

/* check whether the string is an legal name or not */
int CTictactoe::check_filename(const string &filename)
{
	string::size_type index = 0;
	for (; index != filename.size(); ++index) {
		if ((filename[index] >= '0' && filename[index] <= '9') ||
			(filename[index] >= 'a' && filename[index] <= 'z') ||
			(filename[index] >= 'A' && filename[index] <= 'Z') || 
			filename[index] == '_') {
		}
		else {
			return -1;
		}
	}
	return RET_OK;
}

/* get the digit number, for example 12345 have 5 digits */
template<class T> int CTictactoe::get_number_length(T number)
{
	string str;
	TO_STR(number, str);
	return str.length();
}

/* help to show the board */
string CTictactoe::align_center(const string &str, int width)
{
	string str_t("");
	int str_w = str.length();
	if (str_w >= width) {
		str_t = str;
	}
	else {
		str_t.replace(0, str_t.length(), width, ' ');
		str_t.replace((width - str_w) / 2, str_w, str);
	}

	return str_t;
}

string CTictactoe::align_center(char ch, int width)
{
	string str_t(" ");
	str_t[0] = ch;

	return align_center(str_t, width);
}

string CTictactoe::align_center(int val, int width)
{
	string str_t("");
	TO_STR(val, str_t);

	return align_center(str_t, width);
}