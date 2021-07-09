#!/bin/bash

gcc -c menu.c
gcc -c login.c
gcc -c indici.c
gcc -c auto.c
gcc -c cassa.c
gcc -c project.c

gcc -o parking login.o indici.o auto.o cassa.c project.o menu.o 
