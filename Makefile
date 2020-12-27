Map_Maker.out: main.cc
	g++ main.cc -o Map_Maker.out -lncursesw

clean:
	rm Map_Maker.out
