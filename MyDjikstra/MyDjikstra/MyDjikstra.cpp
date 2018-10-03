// MyDjikstra.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_LEN 10

unsigned int grid[BLOCK_LEN][BLOCK_LEN] = { 0 };

/*
* Node would be like:
*		char identifier		'S' - source
*							'D' - destination
*							'#' - block
*							' ' - path
*		3 bytes for holding distance with LSB set indicating node visited
*/

#define VALUE(x)	((char)(x & 0xFF))
#define LENGTH(x)	((unsigned int)((x & 0xFFFFFE00) >> sizeof(char)))
#define VISITED(x)	(x & 0x100)

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
			printf("%c", VALUE(grid[i][j]));
		}
		printf("\n");
	}
}

void fillNode(unsigned int *n, unsigned int dist, int visited, char val)
{
	*n = ((dist & 0xEFFFFF) << 9) | ((visited & 0x1) << 8) | (val);
}

void createGrid(int n) {
	int i, j;
	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (i % (BLOCK_LEN - 1) == 0 || j % (BLOCK_LEN - 1) == 0) {
				fillNode(&grid[i][j], -1, 0, '#');
			}
			else {
				fillNode(&grid[i][j], -1, 0, ' ');
			}
		}
	}

	do {
		fillNode(&grid[getRand(BLOCK_LEN - 1)][getRand(BLOCK_LEN - 1)], -1, 0, '#');
	} while (n--);

	i = (i = getRand(BLOCK_LEN - 2)) ? i : 1;
	j = (j = getRand(BLOCK_LEN - 2)) ? j : 1;

	fillNode(&grid[i][j], -1, 0,'D');

	while ('D' == VALUE(grid[i][j])) {
		i = (i = getRand(BLOCK_LEN - 2)) ? i : 1;
		j = (j = getRand(BLOCK_LEN - 2)) ? j : 1;
	}

	fillNode(&grid[i][j], 0, 0, 'S');
	start[0] = i;
	start[1] = j;

	printGrid();
}

int findD(int i, int j, int dist)
{
	if ('D' == VALUE(grid[i][j])) {
		if (!VISITED(grid[i][j])) {
			if (LENGTH(grid[i][j]) > dist + 1) {
				fillNode(&grid[i][j], dist + 1, 1, VALUE(grid[i][j]));
			}
		}
		return 1;
	}
	else { 
		return 0; 
	}
}

int main()
{
	createGrid(BLOCK_LEN);
    return 0;
}

