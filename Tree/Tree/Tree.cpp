// Tree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

typedef struct Heading
{
	int		weight;
	const char*	text; 
} Heading;

typedef struct Node
{
	Heading	*data;
	Node**	child;
	int		childCount;
} Node;

Node* CreateNode(Heading *head)
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	
	if (newNode) {
		Heading *data = (Heading*)malloc(sizeof(Heading));
		if (data) {
			data->text = _strdup(head->text);
			data->weight = head->weight;

			newNode->child = (Node**) malloc(sizeof(Node**)); 
			newNode->childCount = 0;
			newNode->data = data;
		}		
	}

	return newNode;
}

Heading headings[] = {
	1, "Invertibrates",
	1, "Vertibrates",
	2, "Pices",
	2, "Amphibians",
	3, "Frogs",
	2, "Reptiles",
	3, "Snakes",
	3, "Lizards",
	2, "Aves",
	2, "Mammals",
	3, "Cats",
	3, "Dogs",
	3, "Monkeys",
	4, "Humans",
	1, "Unknown",
	0, NULL // END
};

void printHeadings(Heading *headings)
{
	Heading *pHeading = headings;

	while (pHeading->weight)
	{
		for (int i = 1; i < pHeading->weight; i++)
			printf("\t");
		printf("%s\n", pHeading->text);

		pHeading++;
	}
}

Node* parseHeadings(Heading **pHeadings)
{
	Node *tree = CreateNode(*pHeadings);

	do {
		(*pHeadings)++;

		if (!(*pHeadings)->weight)
			break;

		if ((*pHeadings)->weight > tree->data->weight) {			
			tree->child[tree->childCount++] = parseHeadings(pHeadings);
		}
		else {
			(*pHeadings)--;
			break;
		}

	} while ((*pHeadings)->weight);

	return tree;

}

void parseTree(Node *tree)
{	
	Heading *pHeading = tree->data;

	for (int i = 1; i < pHeading->weight; i++)
		printf("\t");
	printf("%s\n", pHeading->text);

	for (int child = 0; child < tree->childCount; child++)
		parseTree(tree->child[child]);
}

int main()
{
	Heading top = { 0,NULL };
	Node *tree = CreateNode(&top);
	Heading *pHeadings = headings;	
    std::cout << "Hello World!\n\n"; 
	printHeadings(headings);
	
	while (pHeadings->weight) {
		tree->child[tree->childCount++] = parseHeadings(&pHeadings);
		if(pHeadings->weight)
			pHeadings++;
	}
	printf("\n*********Building DONE**********\n");
	parseTree(tree);
	printf("\n*********Parsing DONE**********\n");

	return 0; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
