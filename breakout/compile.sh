#!/bin/bash

clear

g++ -c main.cpp -std=c++11

g++ main.o -o breakout -lsfml-graphics -lsfml-window -lsfml-system
