/**
 ===================================================================================================
 Name        : sudukusolver.c
 Author      : Yinnon Bratspiess
 Description : This program is a suduku solver given a suduku board and solve it (if it's solvable)
 =================================================================================================== 
 **/
// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "genericdfs.h"
#include "sudukutree.h"

// -------------------------- const definitions -------------------------
#define LEGAL_COMMAND_LINE_SIZE 2
#define TRUE 1
#define FALSE 0
// the number in the input board that indicates the slot is empty
#define EMPTY_SLOT 0
#define EPOSITIVE_NUMBER 0
//indicates for an incomplete number returned from the sqrtCheck function
#define INCOMPLETE_NUMBER -1
// place in the command line for file's name
#define FILE_NAME 1


// ------------------------------ functions -----------------------------
/**
 * This function checks for a given slot in the board if it's legal, means it doesn't exist in 
 * the same row, col or "little board" (sqrt of board's size)
 * input :
 * 		SudukuBoardStruct* sudukuBoard - a struct of suduku board
 * 		int row - the given row of the slot
 * 		int col - the given col of the slot
 * 		int sizeOfBoard - the size of the board
 * output :
 * 		0 if not legal and else 1.
 **/
int slotLegalityCheck(SudukuBoardStruct* sudukuBoard, int row, int col, int sizeOfBoard)
{
	// indicator for the output. set by default to true
	int indicator = TRUE;
	int innerCounter;
	// a for loop running on the size of the array (num of rows and cols)
	for (innerCounter = 0; innerCounter < sizeOfBoard; innerCounter++)
	{
		// if the given slot's adress is not equal to the checked slots' adress  in the same 
		// row and col (we dont want to give false when we check the same slot as given as input
		// because it will always have the same value and therefore only checks for differ slots) 
		if ((&(sudukuBoard)->board[row][col] != &(sudukuBoard)->board[row][innerCounter]) \
			&& (&(sudukuBoard)->board[row][col] != &(sudukuBoard)->board[innerCounter][col]))
		{
				// if the checked slot is not 0 and it's value is equal to one of the slots in the
				// in the same row and col than change indicator to false and return false, since
				// the value is illegal. 
				if ((sudukuBoard->board[row][col] != EMPTY_SLOT) && ((sudukuBoard->board[row][col] \
					== sudukuBoard->board[row][innerCounter]) || (sudukuBoard->board[row][col] == \
					sudukuBoard->board[innerCounter][col])))
				{
						indicator = FALSE;
						return indicator;
				}
		}
	}
	// setting start points to start check in the little board. 
	int startRowPoint = row / sqrtCheck(sizeOfBoard) * sqrtCheck(sizeOfBoard);
	int startColPoint = col / sqrtCheck(sizeOfBoard) * sqrtCheck(sizeOfBoard);
	// 2 counters for the loops
	int rowCounter, colCounter;
	// setting finish points to start check in the little board.
	int rowFinishPoint = startRowPoint + sqrtCheck(sizeOfBoard);
	int colFinishPoint = startColPoint + sqrtCheck(sizeOfBoard);
	// a for loop inside a for loop checking again if the adress of the checkd slot is not equal 
	// to the adress of the current slot and if theire values are equal return false.
	for (rowCounter = startRowPoint; rowCounter < rowFinishPoint; rowCounter++)
	{
		for (colCounter = startColPoint; colCounter < colFinishPoint; colCounter++)
		{
			if ((&(sudukuBoard)->board[row][col] != \
				&(sudukuBoard)->board[rowCounter][colCounter]) && \
				((sudukuBoard)->board[row][col] == \
				(sudukuBoard)->board[rowCounter][colCounter]) && \
				((sudukuBoard)->board[row][col] != EMPTY_SLOT))
			{
				indicator = FALSE;
				return indicator;
			}
		}
	}
	// if finished all the checks without returning false return true.
	return indicator;
}

/**
 * This function checks for the whole board if it's values are legal or not.
 * input :
 * 		SudukuBoardStruct* sudukuBoard - a struct of suduku board
 * 		sizeOfBoard - size of the board.
 * output :
 * 		0 if not legal and else 1.
 **/
int boardLegalityCheck(SudukuBoardStruct* sudukuBoard, int sizeOfBoard)
{
	// indicator for the output. set by default to true
	int indicator = TRUE;
	int row, col;
	// a for loop inside a for loop running on all the slots in the board
	for (row = 0; row < sizeOfBoard; row++)
	{
		for (col = 0; col < sizeOfBoard; col++)
		{
			// if the legality check for the current slot is false return false 
			if (slotLegalityCheck(sudukuBoard, row, col, sizeOfBoard) == FALSE)
			{
				indicator = FALSE;
				return indicator;
			}
		}
	}
	// if passed on the whole board and didnt found an illegal slot return true
	return indicator;
}

/**
 * This function is the parser of the program. getting as input a file and a suduku struct, 
 * reading the lines of it and put it in the board in the matching place.
 * input :
 * 		FILE *file - a given file
 * 		SudukuBoardStruct* suduku - a suduku board struct.
 * output :
 * 		int which is the board size (number of rows / cols).
 **/
int parser(FILE *file, SudukuBoardStruct* suduku, char* fileName)
{
	int i, j;
	// reading the size of the board, if the size is not a positive number print error and exit
	fscanf(file, "%d", &suduku->size);
	// if the size is smaller than 0 print error message and exit
	if (suduku->size <= EPOSITIVE_NUMBER)
	{
		printf("%s:not a valid suduku file\n", fileName);
		exit(EXIT_FAILURE);
	}
	// check if the sqrt is int. if it's not print error and exit
	int sqrtSize = sqrtCheck(suduku->size);
	if (sqrtSize ==  INCOMPLETE_NUMBER)
	{
		
		printf("%s:not a valid suduku file\n", fileName);
		exit(EXIT_FAILURE);
	}
	suduku->board = (int**)malloc(suduku->size*sizeof(int*));
	if (suduku->board == NULL)
	{
		free(suduku->board);
		exit(EXIT_FAILURE);
	}
	// a for inside for loop running on the size of the board, reading the current num checks if 
	//it's a positive number and not bigger than the size and if it does enters it to the board,
	//else print error and exit.
	for (i = 0; i < suduku->size; i++)
	{
		suduku->board[i] = (int*)malloc(sizeof(int) * suduku->size);
		for (j = 0; j < suduku->size; j++)
		{
			fscanf(file, "%d", &suduku->board[i][j]);
			if (suduku->board[i][j] < EPOSITIVE_NUMBER || suduku->board[i][j] > suduku->size)
			{
				printf("%s:not a valid suduku file\n", fileName);
				exit(EXIT_FAILURE);
			}
		}
	}
	// if the given board is illegal print no solution and exit
	if (boardLegalityCheck(suduku, suduku->size) == FALSE)
	{
		printf("no solution!\n");
		exit(EXIT_FAILURE);
	}
	return suduku->size;

}


/**
 * A simple function that prints the suduku board as a 2-dimension array
 * input :
 * 		SudukuBoardStruct* sudukuBoard - a struct of suduku board
 * 		sizeOfBoard - size of the board.
 * output :
 * 		void.
 **/	
void sudukuBoardPrinter (SudukuBoardStruct* sudukuBoard, int sizeOfBoard)
{
	int row, col;
	// a for loop inside a for loop printing the current slot
	for (row = 0; row < sizeOfBoard; row++)
	{
		for (col = 0; col < sizeOfBoard; col++)
		{
			// if it's the last num in row dont print space after it, else print
			if ( col == (sizeOfBoard -1))
			{
				printf("%d", sudukuBoard->board[row][col]);
			}
			else
			{
				printf("%d ", sudukuBoard->board[row][col]);
			}
		}
		printf("\n");
	}
}

/**
 * This function is the main function of the program. responsible of getting the file, make the 
 * check if it's legal and print the board if it does, else exit with the matching error message.
 * input :
 * 		int argc - number of arguments 
 * 		char* argv[] - the arguments in the command line
 * output :
 * 		0 if finished working fine, 1 else.
 **/
int main(int argc, char* argv[])
{
	FILE *file;
	int boardSize;
	// oppening the file with reading permission
	file = fopen(argv[FILE_NAME], "r");
	//in case of NULL means no file was given or wrong location
	if (file == NULL) 
	{
		printf("please supply a file!\n");
		printf("usage: sudukusolver <filename>\n");
		exit(EXIT_FAILURE);
	}
	// illeagl input line
	if (argc != LEGAL_COMMAND_LINE_SIZE) 
	{	
		printf("please supply a file!\n");
		printf("usage: sudukusolver <filename>\n");
		exit(EXIT_FAILURE);
	}
	SudukuBoardStruct *suduku = (SudukuBoardStruct*) malloc(sizeof(SudukuBoardStruct));
	// sending the file and the suduku to the parser and put in boardSize the size of the Board
	boardSize = parser(file, suduku, argv[FILE_NAME]);	
	SudukuBoardStruct *finalSuduku = (SudukuBoardStruct*) getBest(suduku, getSudukuChildrenFunc, \
								getSudukuValFunc, freeSudukuFunc, copySudukuFunc, \
								(boardSize * boardSize));
	if (finalSuduku == NULL)
	{
		printf("no solution!\n");
		exit(EXIT_FAILURE);
	}
	printf("%d\n", boardSize);
	sudukuBoardPrinter(finalSuduku, finalSuduku->size);
	freeSudukuFunc(finalSuduku);
	freeSudukuFunc(suduku);
	fclose(file);
	return 0;
}
