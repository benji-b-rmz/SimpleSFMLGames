#!/bin/bash

clear

g++ -c pong.cpp -std=c++11

g++ pong.o -o pong -lsfml-graphics -lsfml-window -lsfml-system
