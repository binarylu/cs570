/*
	assignment for tic-tac-toe part 2
*/

#include "tictactoe.h"

#define MAX_PLAYERS         26
#define MAX_BOARD_SUGGESTED 19

// show error information when user have a bad choice
void show_error(const string &msg)
{
	HANDLE consolehwnd;
	consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << msg << endl;
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// ask user to input a proper filename
string get_filename(const string &prompt)
{
	string input("");
	while (1) {
		cout << prompt;
		getline(cin, input);
		if (RET_OK == CTictactoe::check_filename(input)) {
			break;
		}
		show_error("Only [0-9a-zA-z_] can be accepted! Try again!");
	}
	return input;
}

// ask user to input a number and check it
int get_number(const string &prompt)
{
	string input("");
	int number(-1);
	while (1) {
		cout << prompt;
		getline(cin, input);
		if (RET_OK == CTictactoe::check_string_number(input)) {
			STR2INT(input, number);
			break;
		}
		show_error("Only number can be accepted! Try again!");
	}
	return number;
}

// ask user to make a 1 or 2 choice
int ask_for_1_2(const string &prompt)
{
	int num = -1;
	while (1) {
		num = get_number(prompt);
		if (num == 1 || num == 2) {
			break;
		}
		show_error("Only 1 or 2 can be accepted! Try again!");
	}
	return num;
}

int ask_for_resume_or_start()
{
	return ask_for_1_2("1-resume a saved game, 2-start a new game: ");
}

// when an user chooses reload, ask him/her to input a spped to replay
int ask_for_load_speed()
{
	int num = -1;
	while (1) {
		num = get_number("0-just show the saved status, 1-replay(speed x1), 2-replay(speed x2): ");
		if (num == 0 || num == 1 || num == 2) {
			break;
		}
		show_error("Only 0, 1 or 2 can be accepted! Try again!");
	}
	return num;
}

int ask_for_players_num()
{
	int num = -1;
	string str("");
	while (1) {
		num = get_number("How many players are playing? ");
		if (num >= 2 && num <= 26) {
			break;
		}
		TO_STR(MAX_PLAYERS, str);
		str = "The player numer is from 2 to " + str + "! Try again!";
		show_error(str);
	}
	return num;
}

int ask_for_board_length()
{
	int num = -1, ret = -1;
	while (1) {
		num = get_number("Input the length of the board: ");
		if (num > MAX_BOARD_SUGGESTED) {
			ret = get_number("It's a too large board that may cause some display issues. Are you sure?(1:YES, 2:NO) ");
			if (1 == ret) break;
			else if (2 == ret) continue;
			else show_error("Only 1 or 2 can be accepted! Try again!");
		}
		else if (num < 3) {
			show_error("The minimum of board length is 3!");
		}
		else {
			break;
		}
	}
	return num;
}

int ask_for_win_condition()
{
	return get_number("What the win sequence count? ");
}

// invoked when a game is over
int ask_for_restart_or_exit()
{
	return ask_for_1_2("1-restart a new game, 2-exit: ");
}

// ask the user to input the filename when he/she input "save"
int ask_for_save(CTictactoe &tic, string &filename)
{
	system("cls");
	while (1) {
		// just need to input the filename once in one game
		if (filename == "") {
			filename = get_filename("Input the file name: ");
		}
		if (RET_OK != tic.save(filename)) {
			show_error("Fail to save!");
			if (1 == ask_for_1_2("1-retry, 2-give up: ")) {
				filename.clear();
				continue;
			}
		}
		break;
	}
	tic.display();
	return 0;
}

// When a game is over and the user chooses to restart, ask if he/she wants to save the game
void ask_for_save2(CTictactoe &tic, string &filename, bool is_saved)
{
	system("cls");
	// if the game has been saved, just return
	if (!is_saved && 1 == ask_for_1_2("Do you want to save your game? 1-yes, 2-no: ")) {
		ask_for_save(tic, filename);
	}
}

// invoked when the user input "exit"
void ask_for_exit(CTictactoe &tic, string &filename, bool is_saved)
{
	system("cls");
	// if the game has been saved, just exit
	if (!is_saved && 1 == ask_for_1_2("1-save&exit, 2-exit: ")) {
		ask_for_save(tic, filename);
	}
	exit(0);
}

int main()
{
	int player_num(0), board_length(0), win_condition(0);
	bool is_saved(false);

	CTictactoe tic;

	/* ask the user to choose some options */
	string str("");
	cout << "Please input your option as the prompts, only number can be accepted:" << endl;
	while (1) {
		// The user chooses to resume a saved game
		if (1 == ask_for_resume_or_start()) {
			int s = ask_for_load_speed();                      // ask the user to input the replay speed
			string filename("");
			filename = get_filename("Input the file name: ") ; // ask the user to input the filename of the saved game
			if (0 == tic.load(filename, s)) {                  // load the saved game
				is_saved = true;
				break;
			}
			show_error("Fail to load! Choose to reload or start a new game: ");
		}
		// The user chooses to start a new game
		else {
			while (1) {
				player_num = ask_for_players_num();
				board_length = ask_for_board_length();
				win_condition = ask_for_win_condition();
				int ret = tic.initialize(player_num, board_length, win_condition);    // initialize the game with the user's choices
				if (RET_OK == ret)
					break;
				else if (-1 == ret) {  // only 2-26 players can be accepted
					TO_STR(MAX_PLAYERS, str);
					str = "The player numer is from 2 to " + str + "! Try again!";
					show_error(str);
				}
				else if (-2 == ret) {  // the winning is impossible
					show_error("Winning is impossible with these conditions! Try again!");
				}
			}
			break;
		}
	}

	/* start the game */
	int row, col;

	string filename("");

	while (1) {
		filename.clear();
		while (NOT_FINISHED == tic.judge()) {                // judge if the game is over
			tic.display();
			while (1) {
				if (RET_OK != tic.get_row_col(row, col))     // get the row and col number from number. If the number is illegal or the spot has been occupied, reinput
					continue;

				if (SAVE == row || SAVE == col) {            // when the user input "save"
					ask_for_save(tic, filename);
					is_saved = true;
					continue;
				}
				else if (EXIT == row || EXIT == col) {       // when the user input "exit"
					ask_for_exit(tic, filename, is_saved);
					continue;
				}

				if (RET_OK == tic.move(row, col)) {          // If the user's input is legal and move successfully, jump out the loop and the save status is not-saved
					is_saved = false;
					break;
				}
			}
		}

		tic.show_result();                                   // show the result of the game

		if (1 == ask_for_restart_or_exit()) {                // When the game is over, ask the users to restart or exit
			ask_for_save2(tic, filename, is_saved);
			tic.clear();
		}
		else {
			ask_for_exit(tic, filename, is_saved);
		}
	}

	return 0;
}