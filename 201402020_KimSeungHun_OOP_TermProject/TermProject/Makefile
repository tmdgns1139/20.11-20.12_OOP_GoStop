CC=clang++

# *.cpp ? 
all: gostop.exe *.cpp

state.o : state.cpp
	$(CC) -c state.cpp -o state.o -std=c++17

pocket.o : pocket.cpp
	$(CC) -c pocket.cpp -o pocket.o -std=c++17

card.o : card.cpp
	$(CC) -c card.cpp -o card.o -std=c++17

player.o : player.cpp state.o pocket.o card.o 
	$(CC) -c player.cpp -o player.o -std=c++17

board.o : board.cpp player.o
	$(CC) -c board.cpp -o board.o -std=c++17

judge.o : judge.cpp player.o
	$(CC) -c judge.cpp -o judge.o -std=c++17

controller.o : controller.cpp board.o judge.o
	$(CC) -c controller.cpp -o controller.o -std=c++17

view.o : view.cpp
	$(CC) -c view.cpp -o view.o -std=c++17


gostop.exe : main.cpp view.o controller.o
	$(CC) main.cpp -o gostop.exe view.o controller.o board.o judge.o player.o card.o pocket.o state.o -std=c++17

clean:
	rm -r *.o

