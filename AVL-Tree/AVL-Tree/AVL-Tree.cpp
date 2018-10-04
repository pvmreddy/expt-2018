// AVL-Tree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

typedef struct Node {
	int data; 
	Node *left; 
	Node *right; 
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
				printf("%d ", nextNode->data);
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
		printf("%d\n", data);
		return newNode; 
	}

	return NULL; 
}

void addNode(Node **node, int data)
{
	Node *curNode = *node;
	if (curNode && (curNode->data != data)) {

		if (data > curNode->data) {
			addNode(&curNode->right, data);
		}
		else {
			addNode(&curNode->left, data);
		}
	}
	else {
		*node = createNode(data);
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

void nodeRotateRight(Node *node)
{
	if (node && node->left) {		
		Node *tempLeft = node->left;// This one is going away

		Node *newNode = createNode(node->data);		
		newNode->left = node->left;
		newNode->right = tempLeft->right;

		node->data = tempLeft->data;
		node->left = tempLeft->left;
		node->right = newNode;
		
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

		node->data = tempRight->data;
		node->right = tempRight->right;
		node->left = newNode;

		free(tempRight);
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

void removeNode(Node **node, int data)
{
	Node *nodeToRemove = findNode(node, data);

	if (nodeToRemove)
	{
		Node *parent = findParentNode(node, data);

		if (parent) {

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
			} // Case 2 : Has one child node 
			else if (nodeToRemove->left && !nodeToRemove->right) {
				if (data > parent->data) {
					parent->right = nodeToRemove->left;
				}
				else
				{
					parent->left = nodeToRemove->left;
				}

				free(nodeToRemove);
			}
			else if (nodeToRemove->right && !nodeToRemove->left) {
				if (data > parent->data) {
					parent->right = nodeToRemove->right;
				}
				else
				{
					parent->left = nodeToRemove->right;
				}

				free(nodeToRemove);
			} // Case 3 : Has 2 children
			else {
				Node *nodeToFree = findLargestNode(nodeToRemove->left);
				if (nodeToFree) {
					nodeToRemove->data = nodeToFree->data;
					nodeToRemove->left = nodeToFree->left;
					free(nodeToFree);
				}

			}

		} // No parent means this is the root node
		else {

			// Case 1 : Leaf Node
			if (!nodeToRemove->left && !nodeToRemove->right)
			{
				*node = NULL;
				free(nodeToRemove);
			} // Case 2 : Has one child node 
			else if (nodeToRemove->left && !nodeToRemove->right) {				
				*node = nodeToRemove->left;
				free(nodeToRemove);
			}
			else if (nodeToRemove->right && !nodeToRemove->left) {
				*node = nodeToRemove->right;
				free(nodeToRemove);
			} // Case 3 : Has 2 children
			else {
				Node *nodeToFree = findLargestNode(nodeToRemove->left);
				if (nodeToFree) {
					nodeToRemove->data = nodeToFree->data;
					nodeToRemove->left = nodeToFree->left;
					free(nodeToFree);
				}

			}
			
		}

		printf("\nRemoved %d", data);
	}
}

int main()
{
	Node *tree = NULL; 

	int count = 0; 
    std::cout << "Hello World!\n"; 

	do {
		addNode(&tree, rand()%100);
	} while (count++ < 10);
	
	printf("\nTraversing \n");
	traverseNode(tree);

	bfsTree(tree);

	count = 0;

	
	removeNode(&tree, 41);
	

	printf("\nTraversing after removal\n");
	traverseNode(tree);

	bfsTree(tree);

	removeNode(&tree, 67);


	printf("\nTraversing after removal\n");
	traverseNode(tree);

	bfsTree(tree);

	removeNode(&tree, 69);


	printf("\nTraversing after removal\n");
	traverseNode(tree);

	bfsTree(tree);

	removeNode(&tree, 35);


	printf("\nTraversing after removal\n");
	traverseNode(tree);

	bfsTree(tree);
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
