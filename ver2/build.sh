#!/bin/sh
g++ -c pieces.cpp -o pieces.o
g++ -c main.cpp   -o main.o

if [ "$(uname -s)" = "Linux" ]; then
  g++ -c linux.cpp -o linux.o
  g++ pieces.o linux.o main.o -o tetris
else
  g++ -c win.cpp -o win.o
  g++ pieces.o win.o main.o -o tetris
fi