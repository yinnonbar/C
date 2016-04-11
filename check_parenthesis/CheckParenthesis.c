/**
 ============================================================================
 Name        : CheckParenthesis.c
 Author      : Yinnon Bratspiess
 Description : This script checks the parenthesis of a given file. it checks 
 * wheter a given file can be compiled depends on its brackets. 
 ============================================================================ 
 **/

// ------------------------------ includes ------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

// -------------------------- const definitions -------------------------
#define MAX_NUM_OF_BRACKETS 5000
//a number that is bigger than a legal input. Its given that there are no more than 5000 couples
//of brackets, and therefore 10001 is an illegal size of the containing array.
#define EMPTY_STACK 0
#define ILLEGAL_NUMBER -10001
#define OPEN_CURLY_BRACKET '{'
#define OPEN_ROUND_BRACKET '('
#define OPEN_DIAMOND_BRACKET '<'
#define OPEN_SQUARED_BRACKET '['
#define CLOSED_CURLY_BRACKET '}'
#define CLOSED_ROUND_BRACKET ')'
#define CLOSED_DIAMOND_BRACKET '>'
#define CLOSED_SQUARED_BRACKET ']'

// ------------------------------ functions -----------------------------
/**
 * This function gets a stack, num of current elements in stack and the data which we want to enter
 * and pushs the data to the top of the stack and returns the num of elements currently in the stack
 * input :
 * 		char* stack - a stack 
 * 		int numOfElements - num of elements in the stack
 * 		char data - a given data should be pushed into the stack
 * output :
 * 		int - num of elements in the stack
 **/
int push (char* stack, int numOfElements, char data)
{
	numOfElements++;
	stack[numOfElements] = data;
	return numOfElements;
}

/**
 * This function gets the num of current elements in stack the last char in the stack and the 
 * current char, checks if the chars combination is legal and returns a number as its result.
 * input :
 * 		int numOfElements - num of elements in the stack
 * 		char lastChar - the char in the top of the stack
 * 		char currentChar - the current char that should be checked
 * output :
 * 		int - num of elements in the stack if the combination is legal, else -10001 (more than a
 * 				legal number of input chars)
 **/
int pop (int numOfElements, char lastChar, char currentChar)
{
	//switch which checks the last char
	switch (lastChar)
	{
		//case of '{'
		case (OPEN_CURLY_BRACKET):
			//if the current char is not '}' than change the num of elements to -10001 which means
			// illeagal input was given, else decrease the num of elements by one.
			if (currentChar != CLOSED_CURLY_BRACKET)
			{
				numOfElements = ILLEGAL_NUMBER;
			}
			else
			{
				numOfElements --;
			}
			break;
		//case of '('
		case (OPEN_ROUND_BRACKET):
			//if the current char is not ')' than change the num of elements to -10001 which means
			// illeagal input was given, else decrease the num of elements by one.
			if (currentChar != CLOSED_ROUND_BRACKET)
			{
				numOfElements = ILLEGAL_NUMBER;
			}
			else
			{
				numOfElements --;
			}
			break;
		//case of '<'
		case (OPEN_DIAMOND_BRACKET):
			//if the current char is not '>' than change the num of elements to -10001 which means
			// illeagal input was given, else decrease the num of elements by one.
			if (currentChar != CLOSED_DIAMOND_BRACKET)
			{
				numOfElements = ILLEGAL_NUMBER;
			}
			else
			{
				numOfElements --;
			}
			break;
		//case of '['
		case (OPEN_SQUARED_BRACKET):
			//if the current char is not ']' than change the num of elements to -10001 which means
			// illeagal input was given, else decrease the num of elements by one.
			if (currentChar != CLOSED_SQUARED_BRACKET)
			{
				numOfElements = ILLEGAL_NUMBER;
			}
			else
			{
				numOfElements --;
			}
			break;
		default:
			break;
		
	}
	return numOfElements;
}

/**
 * This is the main function gets as input a file and checks if the file parentheses are fine or 
 * not.
 * input :
 * 		int argc - num of arguments  
 * 		char* argv[] - string includes the arguments given by user
 * output :
 * 		0 if files running well, else another num
 **/
int main(int argc, char* argv[])
{
	FILE* file;
	char currentChar, lastChar;
	char bracketsStack[MAX_NUM_OF_BRACKETS];
	int numOfElements = 0;  
	// if a wrong command was given by user including another num of arguments than 2
	if (argc != 2)
	{      
		printf("please supply a file!\n");
		printf("usage:CheckParenthesis <filename>\n");
		return EXIT_FAILURE;
	}   
    // opening the file with reading permission
	file = fopen(argv[1], "r");
    // if file does not exist or wrong location
	if (file == NULL)	
	{      
		printf("%s no such file", argv[1]);
	    return EXIT_FAILURE;   
	}
		// while not EOF read char by char with fgetc
		while ((currentChar = fgetc(file)) != EOF)
	    {
			// if current char is an opening bracket than push it to the stack
			if (currentChar == OPEN_CURLY_BRACKET || currentChar == OPEN_DIAMOND_BRACKET \
				|| currentChar == OPEN_ROUND_BRACKET || currentChar == OPEN_SQUARED_BRACKET)
			{
				numOfElements = push(bracketsStack, numOfElements, currentChar);
			}
			// if its a closing bracket get the element in the top of the stack
			else if (currentChar == CLOSED_CURLY_BRACKET || currentChar == CLOSED_DIAMOND_BRACKET \
					 || currentChar == CLOSED_ROUND_BRACKET || currentChar == CLOSED_SQUARED_BRACKET)
			{
				lastChar = bracketsStack[numOfElements];
				//checking wether the brackets matchs or not with the pop function. if not print
				//bad structure
				if (numOfElements == 0)
				{
					printf("Bad structure\n");
					numOfElements = ILLEGAL_NUMBER;
					break;
				}
				numOfElements = pop(numOfElements, lastChar, currentChar);
				if (numOfElements == ILLEGAL_NUMBER)
				{
					printf("Bad structure\n");
					break;
				}
			}
		}
	//if finished running one the file and there are still open brackets with no closing brackets
	//than print bad structure
	if (numOfElements > 0)
	{
		printf("Bad structure\n");
	}
	// if finished running on the file and all of the brackets had their matching brackets print ok
	else if (numOfElements == EMPTY_STACK)
	{
		printf("Ok\n");
	}	
	fclose(file);
	return EXIT_SUCCESS;	
}
