/**
 ===================================================================================================
 Name        : sudukutree.h
 Author      : Yinnon Bratspiess
 Description : This is the header for sudukutree.h
 ===================================================================================================
 **/

#ifndef sudukutree_H
#define suduketree_H
//********      structs
typedef struct SudukuBoardStruct 
{
	int** board;
	int size; 
}SudukuBoardStruct;


//********      types and functions types
/**
 * this function gets a num and returns its sqrt, if its sqrt is not an int return -1
 * input :
 * 		int size - a given num
 * output :
 * 		the sqrt of the num, if it's not an int returns -1
 **/ 
int sqrtCheck(int size);

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
int isValid(SudukuBoardStruct* suduku, int num, int numRow, int numCol);

/**
 * This function copies a suduku struct to another one and returns it
 * input :
 * 		void* originalSuduku - a pNode struct
 * output :
 * 		a pNode that is a copy of the given original suduku
 **/
void* copySudukuFunc(void* originalSuduku);

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
int getSudukuChildrenFunc(void* sudukuStruct, void*** childrenListStruct);

/**
 * This function returns the number of numbers in the board that are not 0
 * input :
 * 		void* sudukuStruct - a pNode struct
 * output :
 * 		the number of nums that are not 0 in the board. -1 if the struct is null
 **/
unsigned int getSudukuValFunc(void* sudukuStruct);

/**
 * this is a function that frees a memory of a given node
 * input :
 * 		void* sudukuStruct - a pNode struct
 * output :
 * 		void
 **/
void freeSudukuFunc(void* sudukuStruct);

#endif // sudukutree_H 
