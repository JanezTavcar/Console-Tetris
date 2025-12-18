g++ -c pieces.cpp -o pieces.o

g++ -c win.cpp -o win.o
g++ -c linux.cpp -o linux.o

g++ -c main.cpp -o main.o

g++ pieces.o win.o main.o -o tetris