/**
 ===================================================================================================
 Name        : sudukutree.c
 Author      : Yinnon Bratspiess
 Description : This function implements all the functions for the generic DFS and make them fit
 * 			   for a suduku tree structure in order to solve a suduku board.
 ===================================================================================================
 **/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "genericdfs.h"

// -------------------------- const definitions -------------------------
// not an int number indicator
#define NOT_AN_INT -1
// indicator for an illegal getVal value
#define ILLEGAL_VALUE -1
#define TRUE 1
#define FALSE 0
#define NO_CHILDREN 0

// -------------------------- structs -----------------------------------
typedef struct SudukuBoardStruct 
{
	int** board;
	int size; 
}SudukuBoardStruct;

// ------------------------------ functions -----------------------------
void freeSudukuFunc(void* sudukuStruct);

/**
 * this function gets a num and returns its sqrt, if its sqrt is not an int return -1
 * input :
 * 		int size - a given num
 * output :
 * 		the sqrt of the num, if it's not an int returns -1
 **/ 
int sqrtCheck(int size)
{
	int i;
	// running from 0 to the num and checks if i * i is the requested num. if it does returns it
	// because it's is sqrt
	for (i = 0; i <= size; i++)
	{
		if (i * i == size)
		{
			return i;
		}
	}
	// if finished running and didn't find a matching num it's sqrt is not an int
	return NOT_AN_INT;
}

/**
 * This function returns the number of numbers in the board that are not 0
 * input :
 * 		void* sudukuStruct - a pNode struct
 * output :
 * 		the number of nums that are not 0 in the board. -1 if the struct is null
 **/
unsigned int getSudukuValFunc(void* sudukuStruct)
{
	if (sudukuStruct == NULL)
	{
		return ILLEGAL_VALUE;
	}
	// casting the pNode to a SudukuBoardStruct
	SudukuBoardStruct* suduku = (SudukuBoardStruct*) sudukuStruct;
	int i, j;
	int counter = 0;
	// running on each and every slot in the board and if it's not 0 add it to a counter
	for (i = 0; i < suduku->size; i++)
	{
		for (j = 0; j < suduku->size; j++)
		{
			if (suduku->board[i][j] != 0)
			{
				counter++;
			}
		}
	}
	return counter;
}

/**
 * this function checks if a number is valid in the board in the sent slot. 
 * returns 0 if the number already exist in the little board, row or col and 1 if its not exist
 * input :
 * 		SudukuBoardStruct* suduku - a suduku struct 
 * 		int num - the given num
 * 		int numRow - the num's row in the board
 * 		int numCol - the num's col in the board
 * output :
 * 		0 if the number already exits, 1 else.
 * 		
 **/
int isValid(SudukuBoardStruct* suduku, int num, int numRow, int numCol)
{
	// an indicator. set by default to true
	int indicator = TRUE;
	int i;

	// running on the size of the board. checks if the value of the current slot is equal to one
	// of the values in the slot in his row or col than return false
	for (i = 0; i < (*suduku).size; i++)
	{

		if (((*suduku).board[numRow][i] == num || (*suduku).board[i][numCol] == num))
		{
			indicator = FALSE;
			return indicator;
		}
	}
	// setting start points to start check in the little board. 
	int startRowPoint = (numRow / sqrtCheck((*suduku).size)) * sqrtCheck((*suduku).size);
	int startColPoint = (numCol / sqrtCheck((*suduku).size)) * sqrtCheck((*suduku).size);
	// 2 counters for the loops
	int rowCounter, colCounter;
	// setting finish points to start check in the little board.
	int rowFinishPoint = startRowPoint + sqrtCheck((*suduku).size);
	int colFinishPoint = startColPoint + sqrtCheck((*suduku).size);
	// a for loop inside a for loop checks if the number exists in the little board and if it does
	// return false
	for (rowCounter = startRowPoint; rowCounter < (rowFinishPoint); rowCounter++)
	{
		for (colCounter = startColPoint; colCounter < (colFinishPoint); colCounter++)
		{
			if ((*suduku).board[rowCounter][colCounter] == num)
			{
				indicator = FALSE;
				return indicator;
			}
		}
	}
	// if passed the rows and cols check and the little board means the number is legal and return
	//true
	return indicator;	
}

/**
 * This function copies a suduku struct to another one and returns it
 * input :
 * 		void* originalSuduku - a pNode struct
 * output :
 * 		a pNode that is a copy of the given original suduku
 **/
void* copySudukuFunc(void* originalSuduku)
{
	//casting the given suduku to a SudukuBoardStruct 
	SudukuBoardStruct* suduku = (SudukuBoardStruct*)originalSuduku;
	//creating a new suduku 
	SudukuBoardStruct* newSuduku = (SudukuBoardStruct*)malloc(sizeof(SudukuBoardStruct));
	if (newSuduku == NULL)
	{
		return NULL;
	}
	// copies the size
	newSuduku->size = suduku->size;
	//malloc the size of the board as needed by the size that was copied
	newSuduku->board = (int**)malloc(newSuduku->size * sizeof(int*));
	if (newSuduku->board == NULL)
	{
		return NULL;
	}
	int i, j;
	// a for loop inside a for loop running on each slot in the original suduku and copy it's
	// value to the new suduku board exactly in the same slot
	for (i = 0; i < newSuduku->size; i++)
	{
		newSuduku->board[i] = (int*)malloc(sizeof(int) * newSuduku->size);
		if (newSuduku->board[i] == NULL)
		{
			return NULL;
		}
		for (j = 0; j < newSuduku->size; j++)
		{
			newSuduku->board[i][j] = suduku->board[i][j];
			
		}
	}
	return (void*)newSuduku;
}

/**
 * this is a function that gets a node and a pointer to an array of nodes.
 * The function allocates memory for an array of all the children of the node, populate it,
 * and returns it using the second parameter. The returned value is the number of children.
 * input :
 * 		void* sudukuStruct - a pNode struct
 * 		void*** childrenListStruct - a pointer to an array of pNodes
 * output :
 * 		the number of children. -1 indicates illeagal value
 **/
int getSudukuChildrenFunc(void* sudukuStruct, void*** childrenListStruct)
{
	SudukuBoardStruct* suduku = (SudukuBoardStruct*) sudukuStruct;
	SudukuBoardStruct** childrenList = (SudukuBoardStruct**)malloc(sizeof(SudukuBoardStruct*) * \
	suduku -> size);
	// if it's null return -1 which is the output for illegal value
	if (childrenList == NULL)
	{
		free(childrenList);
		return ILLEGAL_VALUE;
	}
	*childrenListStruct = (void**)childrenList;
	int check;
	int i, j, k;
	// a counter for the children
	int currentChild = 0;
	// a for loop inside a for loop running on each slot in the board
	for (i = 0; i < (*suduku).size; i++)
	{
		for (j = 0; j < (*suduku).size; j++)
		{
			if ((*suduku).board[i][j] != 0)
			{
				continue;
			}
			// if the value of the current slot is 0 than run on each number from 1 to the legal
			// size of the board and checks if this number is legal to put in this slot.
			for (k = 1; k < ((*suduku).size + 1); k++)
			{
				// if this value is valid for this slot than put it in this slot, and copy the 
				// suduku to childrenList in the place of the counter and increase the counter by 
				//one
				if ((check = isValid(suduku, k, i, j)) == 1)
				{
					(*suduku).board[i][j] = k;						
					childrenList[currentChild] = copySudukuFunc(suduku);
					currentChild++;
				}
			}
			return currentChild;
		}
	}
	// count how much numbers are not 0 in the board
	int numbersCounter = getSudukuValFunc(suduku);
	// if the counter is 0 means no children and the board is full of numbers that are not 0 return 
	// no children
	if (currentChild == 0 && numbersCounter == (suduku->size * suduku->size))
	{
		return NO_CHILDREN;
	}
	return ILLEGAL_VALUE;		
}

/**
 * this is a function that frees a memory of a given node
 * input :
 * 		void* sudukuStruct - a pNode struct
 * output :
 * 		void
 **/
void freeSudukuFunc(void* sudukuStruct)
{
	//casting to SudukuBoardStruct*
	SudukuBoardStruct* suduku = (SudukuBoardStruct*)sudukuStruct; 
	int i;
	// a loop running on the size of the board and frees each array
	for (i = 0; i < (*suduku).size; i++)
	{
		free((*suduku).board[i]);
	}
	// free the board and the struct
	free((*suduku).board);
	free(suduku);
	
}
