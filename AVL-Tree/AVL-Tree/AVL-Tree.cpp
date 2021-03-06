// AVL-Tree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <time.h>

typedef struct Node {
	int data; 
	Node *left; 
	Node *right;
	int height; 
}Node;

typedef struct NodeQueueElement {
	Node *node;
	NodeQueueElement *next;
}NodeQueueElement;

typedef struct NodeQueue_ {
	NodeQueueElement *front;
	NodeQueueElement *back;
	int count; 
}NodeQueue_;

NodeQueue_* NodeQueueInit()
{
	NodeQueue_ *nQueue = (NodeQueue_*)malloc(sizeof(NodeQueue_));

	if (nQueue) {
		memset(nQueue, 0, sizeof(NodeQueue_));
		return nQueue;
	}

	return NULL; 
}

void NodeEnqueue(NodeQueue_ **nQueue, Node *node) {

	if (!node)
		return; 

	NodeQueueElement *newQueueElement = (NodeQueueElement*)malloc(sizeof(NodeQueueElement));
	
	if (newQueueElement) {
		newQueueElement->next = NULL;
		newQueueElement->node = node;

		if (*nQueue) {
			(*nQueue)->back->next = newQueueElement;
			(*nQueue)->back = newQueueElement;
			(*nQueue)->count++;
		}
		else {
			*nQueue = NodeQueueInit();
			(*nQueue)->front = (*nQueue)->back = newQueueElement;
			(*nQueue)->count = 1;
		}
	}
}

Node* NodeDequeue(NodeQueue_ **nQueue) {

	if (*nQueue && (*nQueue)->count)
	{
		NodeQueueElement *newQueueElement = (*nQueue)->front;
		Node *node = newQueueElement->node;
		(*nQueue)->front = (*nQueue)->front->next;
		(*nQueue)->count--;	

		if (!(*nQueue)->count)
		{
			(*nQueue)->back = NULL;
			free(*nQueue);
			*nQueue = NULL;
		}

		free(newQueueElement);

		return node;
	}

	return NULL; 
}

Node* NodeQueuePeek(NodeQueue_ *nQueue) {

	if (nQueue && nQueue->count)
	{
		return nQueue->front->node;
	}

	return NULL;
}

void bfsTree(Node * node)
{
	NodeQueue_ *nQueue = NULL;
	
	printf("\nTraverse BFS\n");

	if (node)
	{
		NodeEnqueue(&nQueue, node);

		while (nQueue && nQueue->count)
		{
			Node *nextNode = NodeDequeue(&nQueue);

			if (nextNode) {		
				printf("%2d(%d)  ", nextNode->data, nextNode->height);
				NodeEnqueue(&nQueue, nextNode->left);				
				NodeEnqueue(&nQueue, nextNode->right);
			}
		}
	}
}

Node* createNode(int data)
{
	Node *newNode = (Node*)malloc(sizeof(Node));

	if (newNode) {
		newNode->data = data;
		newNode->left = newNode->right = NULL;
		newNode->height = -1;
		
		return newNode; 
	}

	return NULL; 
}

int nodeHeight(Node *node)
{
	return (node ?  node->height : -1);
}

int maxOf(int a, int b)
{
	return (a > b ? a : b);
}

void nodeRotateRight(Node *node)
{
	if (node && node->left) {
		Node *tempLeft = node->left;// This one is going away

		Node *newNode = createNode(node->data);
		newNode->left = tempLeft->right;
		newNode->right = node->right;
		newNode->height = maxOf(nodeHeight(newNode->left), nodeHeight(newNode->right)) + 1;

		node->data = tempLeft->data;
		node->left = tempLeft->left;
		node->right = newNode;
		node->height = maxOf(nodeHeight(node->left), nodeHeight(node->right)) + 1;

		free(tempLeft);
	}
}

void nodeRotateLeft(Node *node)
{
	if (node && node->right) {
		Node *tempRight = node->right; // This one is going away

		Node *newNode = createNode(node->data);
		newNode->left = node->left;
		newNode->right = tempRight->left;
		newNode->height = maxOf(nodeHeight(newNode->left), nodeHeight(newNode->right)) + 1;

		node->data = tempRight->data;
		node->right = tempRight->right;
		node->left = newNode;
		node->height = maxOf(nodeHeight(node->left), nodeHeight(node->right)) + 1;

		free(tempRight);
	}
}

void nodeRotateLeftRight(Node *node) 
{
	if (node && node->left && node->left->right)
	{
		Node *tempLeft = node->left;
		Node *tempLeftRight = tempLeft->right; 
		Node *newNode = createNode(node->data);

		newNode->right = node->right;
		
		node->data = tempLeftRight->data;		
		node->right = newNode;

		tempLeft->right = tempLeftRight->left;

		tempLeft->height = maxOf(nodeHeight(tempLeft->left), nodeHeight(tempLeft->right)) + 1;
		newNode->height = maxOf(nodeHeight(newNode->left), nodeHeight(newNode->right)) + 1;

		node->height = maxOf(nodeHeight(node->left), nodeHeight(node->right)) + 1;
		
		free(tempLeftRight);
	}
}

void nodeRotateRightLeft(Node *node)
{
	if (node && node->right && node->right->left) 
	{
		Node *tempRight = node->right; 
		Node *tempRightLeft = tempRight->left; 
		Node *newNode = createNode(node->data);

		newNode->left = node->left;

		node->data = tempRightLeft->data;
		node->left = newNode;

		tempRight->left = tempRightLeft->right;

		tempRight->height = maxOf(nodeHeight(tempRight->left), nodeHeight(tempRight->right)) + 1;
		newNode->height = maxOf(nodeHeight(newNode->left), nodeHeight(newNode->right)) + 1;

		node->height = maxOf(nodeHeight(node->left), nodeHeight(node->right)) + 1;

		free(tempRightLeft);
	}
}

void rebalanceNode(Node *curNode)
{
	curNode->height = maxOf(nodeHeight(curNode->left), nodeHeight(curNode->right)) + 1;

	if (nodeHeight(curNode->left) - nodeHeight(curNode->right) > 1)
	{
		if (nodeHeight(curNode->left->left) > nodeHeight(curNode->left->right))
			nodeRotateRight(curNode);
		else
			nodeRotateLeftRight(curNode);
	}
	else if (nodeHeight(curNode->right) - nodeHeight(curNode->left) > 1) 
	{
		if (nodeHeight(curNode->right->left) < nodeHeight(curNode->right->right))
			nodeRotateLeft(curNode);
		else
			nodeRotateRightLeft(curNode);
	}
}

void addNode(Node **node, int data)
{
	Node *curNode = *node;

	if (curNode) {
		if (curNode->data != data) {
			if (data > curNode->data) {
				addNode(&curNode->right, data);
			}
			else {
				addNode(&curNode->left, data);
			}

			rebalanceNode(curNode);
		}
	}
	else {
		*node = createNode(data);
		(*node)->height++;
	}
}

void traverseNode(Node *node)
{
	if (node)
	{
		printf("%02d", node->data);				
		if (node->right)
			printf("->");
		traverseNode(node->right);
		if (node->left)
			printf("\n|\n");
		traverseNode(node->left);
	}	
}



Node* findNode(Node **node, int data)
{
	Node *curNode = *node; 

	if (curNode) {
		if (data == curNode->data) {
			return curNode;
		}
		else if (data < curNode->data) {
			return findNode(&curNode->left, data);
		}
		else if (data > curNode->data) {
			return findNode(&curNode->right, data);
		}
	}

	return NULL;
}

Node* findParentNode(Node **node, int data)
{
	Node *curNode = *node;

	if (curNode && (data == curNode->data))
		return NULL; 

	if (curNode) {
		if ((curNode->left && (data == curNode->left->data)) 
			||(curNode->right && (data == curNode->right->data))){
			return curNode;
		}
		else if (data < curNode->data) {
			return findParentNode(&curNode->left, data);
		}
		else if (data > curNode->data) {
			return findParentNode(&curNode->right, data);
		}
	}

	return NULL; 
}

Node* findLargestNode(Node *node)
{
	if (node) {
		if (node->right) {
			return findLargestNode(node->right);
		}
		else {
			return node;
		}
	} 

	return NULL;
}


// removeNode needs to be recursive to rebalce after node removal
void removeNode(Node **node, int data)
{
	Node *curNode = *node;

	if (curNode) 
	{
		if (curNode->data != data)
		{	
			Node *parent = curNode; 
			Node *left = curNode->left;
			Node *right = curNode->right;
			Node *nodeToRemove = NULL;
			Node *nodeToFree = NULL;

			if ((left && (data == left->data)) || (right && (data == right->data)))
			{
				nodeToRemove = (data > curNode->data) ? right : left;				

				// Case 1 : Leaf Node
				if (!nodeToRemove->left && !nodeToRemove->right)
				{
					if (parent->left == nodeToRemove)
					{
						parent->left = NULL;
					}
					else {
						parent->right = NULL;
					}
					free(nodeToRemove);
				} // Case 2 : Has a child with smaller data 
				else if (nodeToRemove->left) 
				{
					//replace the largest data less than current
					nodeToFree = findLargestNode(nodeToRemove->left);

					nodeToRemove->data = nodeToFree->data;

					removeNode(&nodeToRemove->left, nodeToFree->data);

				} // Case 3 : Has no child with smaller data
				else if (nodeToRemove->right && !nodeToRemove->left) 
				{
					if (data > parent->data) {
						parent->right = nodeToRemove->right;
					}
					else
					{
						parent->left = nodeToRemove->right;
					}

					free(nodeToRemove);
				} 
			}
			else 
			{
				if (data > curNode->data)
				{
					removeNode(&curNode->right, data);
				}
				else 
				{
					removeNode(&curNode->left, data); 
				}
			}

			rebalanceNode(curNode);
		}
		else 
		{
			// This is the root node

			// Case 1 has no children
			if (!curNode->left && !curNode->right)
			{
				free(curNode);
				*node = NULL; 
			} // Case 2 has a child with smaller data
			else if (curNode->left)
			{
				Node *nodeToFree = findLargestNode(curNode->left);

				curNode->data = nodeToFree->data;

				removeNode(&curNode->left, nodeToFree->data);

				rebalanceNode(curNode);
			} // Case 3 has no child with smaller data
			else if (curNode->right)
			{
				*node = curNode->right;
				free(curNode);
				rebalanceNode(*node);
			}			
		}	
	}	
}

int main()
{
	Node *tree = NULL; 

	int count = 0; 

    std::cout << "Hello World!\n"; 

	srand((unsigned)time(NULL));

	do {
		int data = rand() % 100; 
		printf("%d\n", data);
		addNode(&tree, data);
	} while (count++ < 20);
		
	printf("\nTraversing \n");
	traverseNode(tree);

	bfsTree(tree);
	
	count = 0; 

	do {

		int data = rand() % 100;		
		printf("\ntry to remove %d\n", data);
		removeNode(&tree, data);
		printf("\nTraversing after removal\n");
		traverseNode(tree);
		bfsTree(tree);

		if ((count % 5) == 0)
		{
			printf("\nAdd %d\n", data = rand() % 100);
			addNode(&tree, data);
			traverseNode(tree);
			bfsTree(tree);
		}
	} while (tree && count++ < 80);
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
