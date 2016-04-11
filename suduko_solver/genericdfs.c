/**
 ===================================================================================================
 Name        : genericdfs.c
 Author      : Yinnon Bratspiess
 Description : This file is a generic construction of a DFS algorithm.
 ===================================================================================================
 **/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "genericdfs.h"
	
// ------------------------------ functions -----------------------------
/**
 * @brief getBest This function returns the node with the highest value in the tree, using
 * DFS algorithm.
 * @param head The head of the tree.
 * @param getChildren A function that gets a node and a pointer to an array of nodes.
 * The function allocates memory for an array of all the children of the node, populate it,
 * and returns it using the second parameter. The returned value is the number of children.
 * @param getVal A function that gets a node and returns its value, as int.
 * @param freeNode A function that frees a node from memory. This function will be called for each
 * node returned by getChildren.
 * @param copy A function that does a deep copy of a node.
 * @param best The highest possible value for a node. When the function encounters a node with that
 * value, it stops looking and returns it. If the best value can't be determined, pass 
 * UINT_MAX (defined in limits.h) for that parameter.
 * @return The node with the highest value in the tree. In case of an error, or when all the nodes
 * in the tree have a value of zero, the returned node is NULL. If some nodes share the best value,
 * the function returns the first one it encounters.
 */	
pNode getBest(pNode head, getNodeChildrenFunc getChildren,\
			  getNodeValFunc getVal, freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best)
{ 
	// a pnode for children list which will hold the childrens.
	pNode* childrenList = NULL;
	// will hold the current best node (calculeted by the getVal function)
	pNode currentBestNode = NULL; 
	// if the given head is not null than copy it to the current best node and put in current best
	//value the get val of head
	if (head != NULL)
	{
		currentBestNode = copy(head);
	}
	unsigned int currentBestValue = getVal(currentBestNode);
	// if currentBestValue is equal to best, means in the current node all the board is full in 
	// numbers that are not 0 than return that board.
	if (currentBestValue == best)
	{
		return currentBestNode;
	}

	// using get children function the get the number of children and put them in the childrenList
	int numOfChildren = getChildren(head, &childrenList);
	int i;
	// a loop running on the number of children and calling get best recursivly for each and every
	// child
	for (i = 0; i < numOfChildren; i++)
	{
		pNode child = getBest(childrenList[i], getChildren, getVal, freeNode, copy, best);
		if (child != NULL)
		{
			// if the val of child is best than copy it to currentBestNode, breaking and return it
			if (getVal(child) == best)
			{
				freeNode(currentBestNode);
				currentBestNode = copy(child);
				freeNode(child);
				break;
			}
			// if it's value is bigger than the current best node than copy it to the current
			//best node
			if (currentBestNode != NULL && getVal(child) >getVal(currentBestNode))
			{
				freeNode(currentBestNode);
				currentBestNode = copy(child);
				freeNode(child);
			}
		}
	}
	// free the children list
	for (i = 0; i < numOfChildren; i++)
	{
		freeNode(childrenList[i]);
	}
	free(childrenList);
	// in case we finished the DFS and getval of current best node is not best means there's no 
	// solution for the board return null
	if (getVal(currentBestNode) != best)
	{
		freeNode(currentBestNode);
		return NULL;

	}
	return currentBestNode;
}
