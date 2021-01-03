#include "wchars.h"

//This header is meant to easily implement common user-interface elements into ncurses

bool confirm(int y = 5, int x = 1) { //Give user a second chance
	clear();
	mvprintw(y, x, "Are you sure?(y/n)");
	while(true) {
		int ch = getch();
		if (ch == 'y' || ch == 10) { //10 = enter
			clear();
			return true;
		}
		else if (ch == 'n') {
			clear();
			return false;
		}
	}
}

void cont(std::string msg = "", int y = -1, int x = -1) { //Press any key to continue
    curs_set(0);
    if (y == -1 || x == -1) {
        if (msg == "") printw("\n\nPress any key to continue");
        else printw(("\n" + msg).c_str());
        int ch = getch();
        while (ch == ERR) ch = getch();
    }
    else {
        if (msg == "") mvprintw(y, x, "\n\nPress any key to continue");
        else mvprintw(y, x, msg.c_str());
        int ch = getch();
        while (ch == ERR) ch = getch();
    }
    curs_set(1);
    clear();
    refresh();
}

std::string userInput(std::string prompt, int y = -1, int x = -1) { //let user input text
    (y == -1 || x == -1) ? printw(("\n" + prompt + ": ").c_str()) : mvprintw(y, x, (prompt + ": ").c_str());
    int ch = getch();
    std::string input = "";
    int cur_y, cur_x, home_y, home_x;
    getyx(stdscr, home_y, home_x);
    while ((ch != KEY_ENTER && ch != 10 && ch != '\n') || input.size() == 0) {
        getyx(stdscr, cur_y, cur_x);
        ch = getch();
        if (isprint(ch)) input += ch;
        else if (ch == KEY_BACKSPACE && input.size() > 0) {
            input.pop_back();
            mvaddch(cur_y, cur_x - 1, ' '); //put space back into screen buffer
        }
        mvprintw(home_y, home_x, input.c_str());
    }
    clear();
    refresh();
    return input;
}

std::string menu(std::vector <std::string> choices) { //Menu of action choices
    curs_set(0);
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	WINDOW * win = newwin(choices.size() + 2, max_x - 10, max_y - 4 - choices.size(), max_x/32);
	refresh();
	int currChoice = 0;
	for (size_t i = 0; i < choices.size(); i++) mvwprintw(win + 3, i+1, 0, "   %s", choices.at(i).c_str()); //Print regular choices
	while(true) {
		box(win, 0, 0);
		for (size_t i = 0; i < choices.size(); i++) {
			if (static_cast<int>(i) == currChoice && choices.size() > 1) mvwprintw(win, i+1, 3, " %lc %s", wchars::rightcontinuearrow, choices.at(i).c_str());  //If current choice, then highlight
			else mvwprintw(win, i+1, 3, "   %s", choices.at(i).c_str());
			wattroff(win, A_REVERSE); //Otherwise, dont
		}
		wrefresh(win);
		int ch = getch();
		if (ch == KEY_DOWN || tolower(ch) == 'w') {
			currChoice++;
			if (static_cast<size_t>(currChoice) == choices.size()) currChoice = 0;
		}
		else if (ch == KEY_UP || tolower(ch) == 's') {
			currChoice--;
			if (currChoice == -1) currChoice = choices.size() - 1;
		}
		else if (ch == KEY_ENTER || ch == 10) {
			break;
		}
	}
	delwin(win);
	clear();
    curs_set(1);
	return choices.at(currChoice);
}

template<typename T>
T selector(std::string msg, const std::vector<T>& choices, int starting_index = 0, int precision = 0) { //Up/down arrow for selecting amounts
	int currChoice = starting_index;
	int amountOfSpaces = msg.length(); //Find how many spaces to displace the menu
	std::string space;
	for (int i = 0; i < amountOfSpaces + 2; i++) { //Build spaces for up/down arrow
		space+=" ";
	}
	std::string foundation_space = space;
	while(true) {
		refresh();
		int ch = getch();
		if (ch == KEY_UP || ch == 'w') currChoice++;
		else if(ch == KEY_DOWN || ch == 's') currChoice--;
		else if (ch == 10) {
			if(confirm()) break;
			continue;
		}
		else if (ch == ERR){;}
		if (currChoice < 0) currChoice = choices.size() - 1;
		else if (currChoice == choices.size()) currChoice = 0;
		std::string pre_print = std::to_string(choices.at(currChoice));
		std::string print;
		if (precision > 0) {
			int precision_count = 0;
			bool decimal_found = false;
			for (char c : pre_print) {
				if (decimal_found) precision_count++;
				if (c == '.') decimal_found = true;
				print += c;
				if (precision_count == precision) break;
			}
		}
		else print = pre_print;
		if (print.length() > 2)
			for (int i = 0; i < print.length()/2; i++) space += " ";
		mvprintw(0, 0, "%s%lc\n%s: %s\n%s%lc\n", space.c_str(), wchars::uparrow, msg.c_str(), print.c_str(), space.c_str(), wchars::downarrow);
		space = foundation_space;
	}
	return choices.at(currChoice);
}
