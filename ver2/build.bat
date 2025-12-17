g++ -c pieces.cpp -o pieces.o
g++ -c main.cpp -o main.o

g++ pieces.o main.o -o tetris