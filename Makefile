success: Map_Maker.out
	clear; figlet pogchamp | lolcat

Map_Maker.out: main.cc
	g++ main.cc -lncursesw -fsanitize=address -g -o Map_Maker.out

clean:
	rm Map_Maker.out
