#include <ncurses.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <codecvt>
#include <climits>
#include <cmath>
#include <exception>
#include "menu.h" //header for main menu

void turn_on_ncurses() {
    initscr();//Start curses mode
    start_color(); //Enable Colors if possible
    init_pair(1, COLOR_WHITE, COLOR_BLACK); //Set up some color pairs
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    timeout(10); //Set a max delay for key entry
}

int main() {
    system("clear");
    //first make sure they can run the program
    bool flag_fail = false;
	setlocale(LC_ALL, ""); //initialize widechars
    turn_on_ncurses();
    int max_x = 0, max_y = 0; //Stores screen size
    getmaxyx(stdscr, max_y, max_x);
    clear();
    endwin();
    if (max_x < 37) {
        std::cout << "Your screen is not wide enough!\n";
        flag_fail = true;
    }
    if (max_y < 36) {
        std::cout << "Your screen is not tall enough!\n";
        flag_fail = true;
    }
    if (flag_fail) {
        std::cout << "Quitting...\n";
        return 0;
    }
    std::vector<std::vector<wchar_t>> map;
    std::vector<wchar_t> choices; //holds all sybols for user
    int SIZE_x = 0, SIZE_y = 0;
    std::string mapname; //Holds name of map
    std::wifstream load_map, choicefile;
    choicefile.open("symbols");
    choicefile.imbue(std::locale(choicefile.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
    flag_fail = false; //reset flag
    if (choicefile.good()) { //fill up choices
        choicefile >> std::ws;
        while (choicefile) {
            wchar_t nextchar = choicefile.get();
            if (!iscntrl(nextchar) && nextchar >= 0) choices.push_back(nextchar); //only accept valid chars
        }
        if (!choices.size()) flag_fail = true;
    }
    else flag_fail = true;
    if (flag_fail) {
        std::cout << "\"symbols\" file not found or no valid symbols were found, create this file and paste in any unicode symbols you want to use. Quitting..." << std::endl;
        return 0;
    }
    bool mapload = false;
    int x = 0, y = 0; //Stores position of user's cursor
    while(true) {
    printw(" _   _  ____ _   _ ____  ____  _____ ____    __  __    _    ____\n| \\ | |/ ___| | | |  _ \\/ ___|| ____/ ___|  |  \\/  |  / \\  |  _ \\\n|  \\| | |   | | | | |_) \\___ \\|  _| \\___ \\  | |\\/| | / _ \\ | |_) |\n| |\\  | |___| |_| |  _ < ___) | |___ ___) | | |  | |/ ___ \\|  __/\n|_| \\_|\\____|\\___/|_| \\_\\____/|_____|____/  |_|  |_/_/   \\_\\_|\n __  __    _    _  _______ ____\n|  \\/  |  / \\  | |/ / ____|  _ \\\n| |\\/| | / _ \\ | ' /|  _| | |_) |\n| |  | |/ ___ \\| . \\| |___|  _ <\n|_|  |_/_/   \\_\\_|\\_\\_____|_| \\_\\\n");
        std::string choice = menu({"Create Map", "Load Map", "Controls"});
        if (choice == "Controls") {
            printw("Controls:\nArrow keys: Traverse map\nW/S: change symbol up/down\nA/D: change symbol page up/down\nEnter/Space: Place character\nX/Backspace: Remove character\ni: Insert Mode (for entering text)\nESC(has delay)/ENTER(instant): Exit text mode\n");
            cont();
            continue;
        }
        else if (choice == "Create Map") {
            mapname = userInput("Enter new map name");
            std::string size_y, size_x;
            while (true) {
                size_y = userInput("Enter new map height");
                try {
                    SIZE_y = std::stoi(size_y);
                }
                catch (std::exception &e) {
                    clear();
                    printw("Invalid size!");
                    continue;
                }
                if (SIZE_y < 1) {
                    printw("Invalid size!");
                    continue;
                }
                break;
            }
            while (true) {
                size_x = userInput("Enter new map height");
                try {
                    SIZE_x = std::stoi(size_x);
                }
                catch (std::exception &e) {
                    clear();
                    printw("Invalid size!");
                    continue;
                }
                if (SIZE_x < 1) {
                    printw("Invalid size!");
                    continue;
                }
                break;
            }
        }
        else if (choice == "Load Map") {
            mapname = userInput("Enter file name/path");
            mapload = true;
        }
        load_map.open(mapname);
        load_map.imbue(std::locale(load_map.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
        if (mapload && !load_map.good()) { //if loading a new map that doesnt exist
            printw("Map under name/path \"%s\" not found!", mapname.c_str());
            cont();
            continue;
        }
        break;
    }
    if (mapload) {
        bool gotx = false;
        while(load_map) {
            std::wstring temp;
            std::getline(load_map, temp);
            if (load_map) {
                if (!gotx) {
                    SIZE_x = temp.length();
                    gotx = true;
                }
                SIZE_y++;
            }
        }
        map.resize(SIZE_y);
        for (auto &v : map) v.resize(SIZE_x, L' '); //Resize 2d vector
        load_map.close();
        load_map.open(mapname);
        for (int i = 0; i < SIZE_y; i++) {
            for (int j = 0; j < SIZE_x; j++) {
                if (load_map.peek()) load_map.get(map.at(i).at(j));
            }
            load_map.get();
        }
        load_map.close();
    }
    map.resize(SIZE_y);
    for (auto &v : map) v.resize(SIZE_x, L' '); //Resize 2d vector
    int currchar = 0;
    int charpage = 0;
    int x_offset = 0, y_offset = 0; //how far user has rolled over the edge of the view window in x/y direction
    int ch; //holds character input
    bool insertmode = false;
    bool drawmode = false;
    bool grid = true;
    while (true) { //editing loop
        ch = getch();
        charpage = currchar/10;
        //scrolling
        if (x - x_offset >= max_x - 40)
            // x_offset = x - (max_x - 40) + 1;
            x_offset++;
        if (x - x_offset < 0)
            x_offset--;
        if (y - y_offset >= max_y)
            //y_offset = y - max_y + 1;
            y_offset++;
        if (y - y_offset < 0)
            y_offset--;
        if (x < max_x - 40) x_offset = 0;
        if (y < max_y) y_offset = 0;
        mvprintw(1, max_x - 35, "%lc", L'▲');
        for (int i = 0; i < 10; i++) { //Print choices, but not 10 if on last page
            mvprintw(i*2 + 2, max_x - 37, "                                               "); //clean up from previous frame (pretty sure a number isnt gonna be this long)
            if (i == currchar%10) attron(A_REVERSE);
            if (i+(charpage*10) < (int)choices.size()) //dont go out of range on last page
                mvprintw(i*2 + 2, max_x - 37, "%i: %lc", charpage*10+i+1, choices.at(i+(charpage*10)));
            attroff(A_REVERSE);
        }
        mvprintw(21, max_x - 35, "%lc", L'▼');
        for (int i = 0; i < max_y; i++) { //Print map
            for (int j = 0; j < max_x - 40; j++) {
                if (i < (int)map.size() && j < (int)map.at(i).size()) {
                    if (map.at(i + y_offset).at(j + x_offset) == L' ')
                        mvprintw(i, j, "%lc", grid ? L'.' : L' ');
                    else
                        mvprintw(i, j, "%lc", map.at(i + y_offset).at(j + x_offset));
                }
            }
        }
        mvprintw(23, max_x - 37, "Symbol Page: %i/%i  ", charpage+1, (int)ceil(choices.size()/10.0));
        mvprintw(25, max_x - 37, "x= %i/%i, y= %i/%i  ", x, SIZE_x-1, y, SIZE_y-1);
        mvprintw(27, max_x - 37, "Press Q to Save/Quit");
        mvprintw(29, max_x - 37, "Ctrl-C to Quit Without Saving");
        if (!insertmode) mvprintw(31, max_x - 37, "i: Insert mode                    ");
        if (!drawmode) mvprintw(33, max_x - 37, "f: Draw mode                        ");
        mvprintw(35, max_x - 37, "g: Toggle Grid");
        move(y -  y_offset, x - x_offset); //move cursor to users location
        if (ch == KEY_UP) { //traversal
            y--;
            if (y < 0) y = SIZE_y - 1; //Wrap around
        }
        else if (ch == KEY_DOWN) {
            y++;
            if (y >= SIZE_y) y = 0;
        }
        else if (ch == KEY_LEFT) {
            x--;
            if (x < 0) x = SIZE_x - 1;
        }
        else if (ch == KEY_RIGHT) {
            x++;
            if (x >= SIZE_x) x = 0;
        }
        else if (ch == 'w' && !insertmode) { //Character cycling
            currchar--;
            if (currchar < 0) currchar = choices.size()-1; //wrap around
        }
        else if (ch == 's' && !insertmode) {
            currchar++;
            if ((size_t)currchar >= choices.size()) currchar = 0; //wrap around
        }
        else if (ch == 'a' && !insertmode && ceil(choices.size()/10.0) > 1) { //only if more than one page of chars
            if (charpage) currchar -= (10 + currchar%10); //go back one
            else currchar = choices.size()-1 - (choices.size()%11); //or wrap around
            if (currchar < 0) currchar = 0; //edge case
        }
        else if (ch == 'd' && !insertmode && ceil(choices.size()/10.0) > 1) { //only if more than one page of chars
            currchar += (10 - currchar%10); //go forward one
            if ((size_t)currchar >= choices.size()) currchar = 0; //or wrap around
        }
        else if ((ch == ' ' || ch == '\n' || (drawmode && ch != 'f')) && !insertmode) { //Character placing
            map.at(y).at(x) = choices.at(currchar);
        }
        else if (ch == KEY_BACKSPACE || ch == 'x' || ch == 'X') map.at(y).at(x) = L' '; //Character clearing
        else if (ch == 'i' && !insertmode) { //insert mode
            insertmode = true;
            attron(COLOR_PAIR(5));
            mvprintw(31, max_x - 37, "*INSERT MODE* ENTER to exit");
            attroff(COLOR_PAIR(5));
            continue;
        }
        else if (ch == 'f' && !insertmode) {
            drawmode = !drawmode;
            if (drawmode) mvprintw(33, max_x - 37, "*DRAW MODE* f to exit      ");
            else mvprintw(31, max_x - 37, "f: Draw mode                      ");
        }
        else if ((ch == 'q' || ch == 'Q') && !insertmode)  { //Save and quit
            curs_set(0);
            attron(COLOR_PAIR(5));
            mvprintw(27, max_x - 37, "Are you sure? (y/n)              ");
            attroff(COLOR_PAIR(5));
            refresh();
            while(true) {
                ch = getch();
                if (ch != ERR) break;
            }
            if (ch == 'y' || ch == '\n') break;
            curs_set(1);
        }
        else if (insertmode && ch == '\n') { //disable insert mode
            insertmode = false;
            mvprintw(31, max_x - 37, "i: Insert mode                    ");
        }
        else if (!insertmode && ch == 'g') { //toggle grid
            grid = !grid;
        }
        if (insertmode && (isalnum(ch) || ch == ' ' || ispunct(ch) || ch == KEY_BACKSPACE)) {
            if (ch == ' ') {
                map.at(y).at(x) = L' ';
                x++;
                if (x >= SIZE_x) x--;
            }
            else if (ch == KEY_BACKSPACE) {
                if (x > 0) map.at(y).at(x-1) = L' ';
                x--;
                if (x < 0) x++;
            }
            else {
                map.at(y).at(x) = static_cast<wchar_t>(ch);
                x++;
                if (x >= SIZE_x) x--;
            }
        }
        refresh();
    }
    //Now save the map
    std::wofstream wof(mapname);
    wof.imbue(std::locale(wof.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
    for (int i = 0; i < SIZE_y; i++) {
        for (int j = 0; j < SIZE_x; j++) {
            wof.put(map.at(i).at(j));
            if (j == SIZE_x - 1) wof.put('\n');
        }
    }
    clear();
    endwin();
    system("clear; echo Success!");
}
