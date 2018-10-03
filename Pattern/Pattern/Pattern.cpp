// Pattern.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <malloc.h>  
typedef struct {
	int width;
	int height; 
}Square;

void showOutput(char *buffer, int width, int height)
{
	if (buffer) {
		int i, j;
		char *p = buffer;
		for (i=0; i< height; i++) {
			for (j=0; j<width; j++) {
				printf("%c", *p);
				p++;
			}
			printf("\n");
		}
	}
}

int main()
{
	int i, j;
	int height = 10, width = 10;
	char *buffer = NULL, *p=NULL; 
	Square square = { 3,3 };

	buffer =(char*) malloc(height*width);
	if (!buffer) {
		printf("No memory");
		return 0;
	}

	memset(buffer, ' ', width*height);

	for (i = 0, p = buffer; i < height; i++) {
		for (j = 0; j < width; j++) {
			/* Skip the pixels that will be covered by the square */
			if ((i >(height - square.height) / 2) && (i < (height + square.height) / 2) && (j < square.width)) {
				/* Write yellow to the buffer */
				*p = '*';
			}
			p++;
		}
	}

	showOutput(buffer, width, height);

    return 0;
}

