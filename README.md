# NCurses-Map-Maker
Easily create your own text map files with the NCurses Map Maker
```
  _   _  ____ _   _ ____  ____  _____ ____    __  __    _    ____
 | \ | |/ ___| | | |  _ \/ ___|| ____/ ___|  |  \/  |  / \  |  _ \
 |  \| | |   | | | | |_) \___ \|  _| \___ \  | |\/| | / _ \ | |_) |
 | |\  | |___| |_| |  _ < ___) | |___ ___) | | |  | |/ ___ \|  __/
 |_| \_|\____|\___/|_| \_\____/|_____|____/  |_|  |_/_/   \_\_|

  __  __    _    _  _______ ____
 |  \/  |  / \  | |/ / ____|  _ \
 | |\/| | / _ \ | ' /|  _| | |_) |
 | |  | |/ ___ \| . \| |___|  _ <
 |_|  |_/_/   \_\_|\_\_____|_| \_\
```

*Requires the ncursesw library!*

Important things to know:
- This program supports any unicode characters displayable
- Make sure to change the "symbols" file if you want to add more placeable objects

Controls:

Arrow keys: Traverse map

W/S: change symbol up/down

A/D: change symbol page up/down

Enter/Space: Place character

X/Backspace: Remove character

i: Insert Mode (for entering text)

g: Toggle Grid

ENTER: Exit insert mode

f: Toggle draw mode (automatically places a character whenever you move, great for drawing lines of characters)

Features including mouse support and a functioning menu coming soon

FOR CSCI4X STUDENTS: How to read from a file with unicode
```c++
#include<fstream> //make sure file streams are included at the top of your code
std::wifstream file; //has to be a wifstream
file.open("Filename");
file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>)); //just copy paste this line
```

Happy map making!
