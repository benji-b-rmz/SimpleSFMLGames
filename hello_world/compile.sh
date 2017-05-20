#!/bin/bash

clear

g++ -c game.cpp

g++ game.o -o sfml-game -lsfml-graphics -lsfml-window -lsfml-system
