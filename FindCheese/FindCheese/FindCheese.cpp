// FindCheese.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/*
============================================================================
Name        : FindTheCheese.c
Author      : Vamsi Mohan Reddy Pulagam
Version     :
Copyright   : Use as you like
Description : Hello World in C, Ansi-style
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_LEN 10

char grid[BLOCK_LEN][BLOCK_LEN] = { 0 };

int start[2] = { 0 };

int getRand(int n)
{
	return (rand() % n);
}

void printGrid() {
	int i, j;
	printf("\n");
	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}

void createGrid(int n) {
	int i, j;
	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (i % (BLOCK_LEN - 1) == 0 || j % (BLOCK_LEN - 1) == 0) {
				grid[i][j] = '#';
			}
			else {
				grid[i][j] = ' ';
			}
		}
	}

	do {
		grid[getRand(BLOCK_LEN - 1)][getRand(BLOCK_LEN - 1)] = '#';
	} while (n--);

	i = (i = getRand(BLOCK_LEN - 2)) ? i : 1;
	j = (j = getRand(BLOCK_LEN - 2)) ? j : 1;
	grid[i][j] = 'C';

	while ('C' == grid[i][j]) {
		i = (i = getRand(BLOCK_LEN - 2)) ? i : 1;
		j = (j = getRand(BLOCK_LEN - 2)) ? j : 1;
	}

	grid[i][j] = 'M';
	start[0] = i;
	start[1] = j;

	printGrid();
}

int findCheese(int d, int l) {
	if (grid[d][l] == 'C')
		return 1;

	if (grid[d][l] == '#' || grid[d][l] == '.' || grid[d][l] == 'M')
		return 0;

	if (grid[d][l] == ' ') {
		grid[d][l] = '.';
	}

	if (!findCheese(d, l + 1) && !findCheese(d + 1, l) && !findCheese(d, l - 1)
		&& !findCheese(d - 1, l)) {
		grid[d][l] = ' ';
		return 0;
	}

	return 1;
}

int main(void) {
	srand(time(NULL));
	createGrid(BLOCK_LEN*2);
	if (!findCheese(start[0], start[1] + 1)
		&& !findCheese(start[0] + 1, start[1])
		&& !findCheese(start[0], start[1] - 1)
		&& !findCheese(start[0] - 1, start[1])) {
		printf("\nCheese cannot be found !!!\n");
	}
	else {
		printf("\nCheese found !!!\n");
		printGrid();
	}
	return EXIT_SUCCESS;
}


