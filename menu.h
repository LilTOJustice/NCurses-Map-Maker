#include "wchars.h"

bool confirm() { //For second chances
	clear();
	mvprintw(5, 1, "Are you sure?(y/n)");
	while(true) {
		int ch = getch();
		if (ch == 'y' || ch == 10) {
			clear();
			return true;
		}
		else if (ch == 'n') {
			clear();
			return false;
		}
	}
}

std::string menu(std::vector <std::string> choices) { //Menu of action choices
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	WINDOW * win = newwin(choices.size() + 2, max_x - 10, max_y - 4 - choices.size(), max_x/32);
	refresh();
	int currChoice = 0;
	wattron(win, COLOR_PAIR(7));
	for (size_t i = 0; i < choices.size(); i++) mvwprintw(win + 3, i+1, 0, "   %s", choices.at(i).c_str()); //Print regular choices
	while(true) {
		wattron(win, COLOR_PAIR(7));
		box(win, 0, 0);
		wattroff(win, COLOR_PAIR(7));
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
		else if (ch == 10) {
			break;
		}
	}
	delwin(win);
	clear();
	return choices.at(currChoice);
}

template<typename T>
T selector(std::string msg, const std::vector<T>& choices, int starting_index = 0, int precision = 0) { //Up/down arrow selecting
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
