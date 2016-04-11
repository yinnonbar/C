/**
 ============================================================================
 Name        : waredb.c
 Author      : Yinnon Bratspiess
 Description : This script is a ware manager. it gets a database of products which are in the ware
 * and deals with functions that happens on the products such as add new products, send products
 * away and clean products from the ware.
 ============================================================================ 
 **/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

// -------------------------- const definitions -------------------------
#define MAX_NUM_OF_LINES 1000 
#define MAX_LENGTH_OF_LINE 1000
//regex for sent product line : barcode, tab, quantity
#define SENT_PRODUCT_REGEX "%d\t%f\n"
//regex for product line : name, barcode, quantity, year-month
#define PRODUCT_REGEX "%[^\t]\t%d\t%f\t%d-%d\n"
#define LEGAL_COMMAND_LINE_SIZE 4
#define RECEIVED "received"
#define SENT "sent"
#define CLEAN "clean"
#define HYPHEN_SIGN '-'
#define EPSILON 0.001
//max legal name length
#define NAME_LENGTH 21
#define FIRST_LEGAL_YEAR 0
#define FIRST_LEGAL_MONTH 0
#define NUM_OF_MONTHS 12
//a num bigger than this number is at least a 5 digit number
#define FIVE_DIGITS_NUMBER 9999
#define NEGATIVE_NUMBER 0
#define LEGAL_QUANTITY_SIZE 0
#define ILEGAL_BARCODE_SIZE 0
#define TRUE 1
#define FALSE 0

// -------------------------- structs -----------------------------------
/**
 * struct for Product. includes 5 fields :
 * char name[NAME_LENGTH] - name of the product. max length - 20 chars.
	int barcode - a barcode number. a 4 digitis number.
	float quantity - the amount of the product.
	int month - month of expirition date
	int year - year of expirition date
 **/
typedef struct Product 
{
	char name[NAME_LENGTH];
	int barcode;
	float quantity;
	int month;
	int year;
}Product;

// ------------------------------ functions -----------------------------

/**
 * This function checks if barcode is valid means has 4 digit (smaller than 10000 and not an 
 * negative number)
 * input :
 * 		int barcode - a given barcode number
 * output :
 * 		int 1 if the barcode is valid, 0 otherwise
 **/
int barcodeCheckValidation (int barcode)
{
	//if barcode is a 4 digit number return 1, else return 0
	if (barcode <= FIVE_DIGITS_NUMBER && barcode > NEGATIVE_NUMBER )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * This function is the parser of the program. getting as input a file, reading the lines of it
 * put it in the array as products in the given format and returns the number of products given
 * in the file. 
 * input :
 * 		FILE *file - a given file
 * 		Product productsList[] - an array of products
 * output :
 * 		int which is the number of products in the given file.
 **/
int parser (FILE *file, Product productsList[])
{
	int numOfProducts = 0;
	// using fscanf, while there are still lines in the file read line by line and seperate the
	// info in the specific line to create a product from it in the matching struct and increase
	// the products counter by one.
	while (fscanf(file, PRODUCT_REGEX, productsList[numOfProducts].name,\
		   &productsList[numOfProducts].barcode, &productsList[numOfProducts].quantity, \
		   &productsList[numOfProducts].year, &productsList[numOfProducts].month) != EOF)
	{
		// if the input doesnt meets the requirments such as barcode is not a 4 digit number or
		// quantity is negative number or the year is smaller than zero or month is not between
		//0 to 12 than print unknown file format and exit.
		if (barcodeCheckValidation(productsList[numOfProducts].barcode) == ILEGAL_BARCODE_SIZE \
			|| productsList[numOfProducts].quantity < LEGAL_QUANTITY_SIZE \
			|| productsList[numOfProducts].year < FIRST_LEGAL_YEAR \
			|| productsList[numOfProducts].month < FIRST_LEGAL_MONTH \
			|| productsList[numOfProducts].month > NUM_OF_MONTHS)
		{
			printf("unknown file format \n");
			exit(EXIT_FAILURE);
		}
		else
		{
			numOfProducts++;
		}			
	}
	return numOfProducts;
}

/**
 * This function is the parser for a sent file. getting as input a file, reading the lines of it
 * put it in the array as products in the given format and returns the number of products given
 * in the file.
 * This parser is differ from the other parser because the file given in a sent file includes only
 * barcode and quantity 
 * input :
 * 		FILE *sentFile - a given sent file
 * 		Product sentList[] - an array of products
 * output :
 * 		int which is the number of products in the given file.
 **/
int sentParser(FILE *sentFile, Product sentList[])
{
	int numOfProducts = 0;
	// using fscanf, while there are still lines in the file read line by line and seperate the
	// info in the specific line to create a product from it in the matching struct and increase
	// the products counter by one.
	while (fscanf(sentFile, SENT_PRODUCT_REGEX, &sentList[numOfProducts].barcode, \
		   &sentList[numOfProducts].quantity) != EOF)
	{
		if (barcodeCheckValidation(sentList[numOfProducts].barcode) == ILEGAL_BARCODE_SIZE)
		{
			exit(EXIT_FAILURE);
		}
		else 
		{	
			numOfProducts++;
		}
	}
	return numOfProducts;
}

/**
 * This function is given two numbers and returns the minimal one. 
 * input :
 * 		float num1, num2 - two floats
 * output :
 * 		float - the minimal number from num1 and num2.
 **/
float minimumFinder (float num1, float num2)
{
	if (num1 < num2)
	{
		return num1;
	}
	else 
	{
		return num2;
	}
}



/**
 * This function is a comperator given two products and compare between their names.
 * input :
 * 		Product p1, p2 - two given products
 * output :
 * 		int - negative number if p1's name is smaller, positive if p1's name is bigger and zero if
 * 		it's equal.
 **/
int nameComparison(Product p1, Product p2)
{
	//return negative number if p1 name is smaller, positive number if p1 is bigger
	//and zero if equal.
	return (strcmp(p1.name, p2.name));
}	

/**
 * This function is a comperator given two products and compare between their dates.
 * input :
 * 		Product p1, p2 - two given products
 * output :
 * 		int - -1 if p1's date is smaller, 1 if p1's date is bigger and sends to name comparison 
 * 		if it's equal.
 **/
int dateComparison(Product p1, Product p2)
{
	// if p1 is smaller than p2 and differ by year (p1 ends before p2)
	if (p1.year < p2.year)
	{
		return -1;
	}
	else if (p1.year > p2.year) 
	{
		return 1;
	}
	// p1.year == p2.year
	else 
	{
		// if they have the same year but p1 is smaller than p2
		// (p1 ends before p2)
		if (p1.month < p2.month)
		{
			return -1;
		}
		else if (p1.month > p2.month)
		{
			return 1;
		}
		//equals by year and month
		else 
		{
			return (nameComparison(p1, p2));
		}	
	}
}

/**
 * This function is the main comperator in the program. 
 * given two products and compare between their barcods, if equals sends to date comparison.
 * input :
 * 		Product p1, p2 - two given products
 * output :
 * 		int - -1 if p1's barcode is smaller, 1 if p1's barcode is bigger and sends to date 
 * 		comparison if it's equal.
 **/
int comparison(Product p1, Product p2)
{
	// if p1 barcode is smaller than p2 barcode
	if (p1.barcode < p2.barcode) 
	{
		return -1;
	}
	// if p1 barcode is bigger than p2 barcode
	else if (p1.barcode > p2.barcode)
	{
		return 1;
	}
	// if p1 barcode is equal to p2 barcode
	else
	{
		return (dateComparison(p1, p2));
	}
}

/**
 * This function is buuble sort. sorting the input in an asscending order using the comperators 
 * above.
 * input :
 * 		Product productsList[] - a given list of products. 
 * 		int listSize - size of the list.
 * output :
 * 		void
 **/
void bubbleSort (Product productsList[], int listSize)
{
	// a temp product
	Product swap;
	int i, j;
	for (i = 0; i < (listSize - 1); i++)
	{
		for (j = 0; j < listSize - i - 1; j++)
		{
			// using comparison between current product and the next one and if the next one is
			// smaller than bubbling the current product up.
			if (comparison(productsList[j], productsList[j + 1]) > 0)
			{
				swap = productsList[j];
				productsList[j] = productsList[(j + 1)];
				productsList [(j + 1)] = swap;
			}
		}
	}
}

/**
 * This function deals with case of received. gets as input a list of products that received to the
 * ware and should be added to the list of products.
 * input :
 * 		Product productList[] - the list of products currently in the ware 
 * 		int *productListSize - number of current products in the ware
 * 		Product receivedList[] - the list of the files that received and should be enterd to the 
 * 		ware
 * 		int receivedListSize - number of products should be added.
 * output :
 * 		void
 **/
void received (Product productList[], int *productListSize, Product receivedList[], int \
			   receivedListSize)
{
	int i, j, productsCounter;
	for (i = 0; i < receivedListSize; i++)
	{
		// a counter for each product in the received list and initilzed for every product,
		// checks wether the current product is already in the ware or not
		productsCounter = 0;
		for (j = 0; j < *productListSize; j++)
		{
			// the product exist in the ware (means has the same name, expiriton date and barcode)
			// than add it quantity to the already exist product quantity in the ware
			if ((strcmp(receivedList[i].name, productList[j].name) == 0) && \
				(receivedList[i].barcode == productList[j].barcode) && \
				receivedList[i].year == productList[j].year \
				&& receivedList[i].month == productList[j].month)
			{
				productList[j].quantity += receivedList[i].quantity;
			}
			// if the product is not the current product were checking than increment the counter by
			//one
			else 
			{
				productsCounter += 1;
			}
		}
		//if the counter is equal to the number of products in the ware means weve checked each
		// and every one and this product does not exist in the ware than we add it to the ware.
		if (productsCounter == *productListSize)
		{
			strcpy(productList[*productListSize].name, receivedList[i].name);
			productList[*productListSize].barcode = receivedList[i].barcode;
			productList[*productListSize].quantity = receivedList[i].quantity;
			productList[*productListSize].year = receivedList[i].year;
			productList[*productListSize].month = receivedList[i].month; 
			(*productListSize) += 1;
		}
	}
}

/**
 * This function deals with case of sent. gets as input a list of products that should be sent from
 * the ware.
 * input :
 * 		Product productList[] - the list of products currently in the ware 
 * 		int *productListSize - number of current products in the ware
 * 		Product* sentList  - the list of the files that should be sent.
 * 		int receivedListSize - number of products should be sent.
 * output :
 * 		void
 **/
void sent (Product* productList, int productListSize, Product* sentList, int sentListSize)
{
	int i, j;
	for (i = 0; i < sentListSize; i++)
	{
		// the quantity requested for current product
		float requiredQuantity = sentList[i].quantity;
		for (j = 0; j < productListSize; j++)
		{
			// if the barcode matches and we still need to send more quantity of current product
			if (sentList[i].barcode == productList[j].barcode && requiredQuantity > 0)
			{ 
				// puts in min the minimum quantity from the product in the sent list or the 
				//product list and reduce it from the required quantity and the current product
				//quantity
				float min = minimumFinder(sentList[i].quantity, productList[j].quantity); 
				requiredQuantity -= min;
				productList[j].quantity -= min;
			}
		}
		// after finish looking at all the products in the ware if we couldnt find enought products
		// to send (means we need to send more than 0.001) than print not enough items in warehouse.
		if (requiredQuantity > EPSILON )
		{
				printf("not enough items in warehouse\n");
				exit(EXIT_FAILURE);
		}
	}
}

/**
 * This function deals with case of clean. gets as input a date and should delete all the products 
 * that expired or that there's no more quantity from them.
 * input :
 * 		Product* productList - list of products in the ware
 *  	int *productListSize - size of products in the ware
 * 		int year - a given year. if 0 ignore the date.
 * 		int month - a given month. if 0 ignore the month.
 * output :
 * 		void
 **/

void clean (Product* productList, int *productListSize, int year, int month)
{
	int i = 0;
	while (i < *productListSize)
	{
		// if current product should be cleard means : theres no more quantity from it, or it has
		// expired than put the product from the last place in the list in the current place and
		// reduce the list's size by one.
		if (productList[i].quantity < EPSILON || \
			(productList[i].year == year && productList[i].month < month) || \
		    productList[i].year < year || \
			(productList[i].year <= year && month == 0)) 
		{
			productList[i] = productList[(*productListSize -1)];
			*productListSize -= 1;
		}
		// if not stand in one of the conditions above means the product should not be cleard,
		// increment i by one and keep look again in the next product.
		else 
		{
			i++;
		}	
	}
}

/**
 * This is the main function in the program. gets as input a command line and send it to the 
 * matching function depends on the line's command.
 * input :
 * 		int argc - num of arguments  
 * 		char* argv[] - string includes the arguments given by user
 * output :
 * 		0 if files running well, else another num
 **/
int main(int argc, char* argv[])
{
	FILE *file;
	Product productsList[MAX_NUM_OF_LINES];
	// openning the file with reading permission
	file = fopen(argv[1], "r");
	char* commandName = argv[2];
	//in case of NULL means no file was given or wrong location
	if (file == NULL) 
	{
		printf("<filename>: no such file\n");
	}
	// illeagl input
	if (argc != LEGAL_COMMAND_LINE_SIZE) 
	{	
		printf("USAGE: waredb <db file> <command> <command arg file>\n");
		return EXIT_FAILURE;
	}
	//sending the file to the parser and gets the number of products in the file and put them in 
	// the products list.
	int numOfProducts = parser(file, productsList);
	// sorting the list.
	bubbleSort(productsList, numOfProducts);
	//case of "received"
	if (!strcmp(commandName, RECEIVED))
	{	
		//gets the name of the file for received file 
		FILE* receivedFile = fopen(argv[3], "r");
		Product receivedList[MAX_NUM_OF_LINES];
		//sends the file to the parser
		int numOfReceivedProducts = parser(receivedFile, receivedList); 
		//using received function on the two lists : the products list and received list in order
		// to add them to the ware.
		received(productsList, &numOfProducts, receivedList, numOfReceivedProducts);
	}
	//case sent
	else if (!strcmp(commandName, SENT))
	{
		//gets the name of the file for sent file
		FILE* sentFile = fopen(argv[3], "r");
		Product sentList[MAX_NUM_OF_LINES];
		//sending the file to the sent parser in order to make a list of products from it.
		int numOfSentProducts = sentParser(sentFile, sentList);
		// sending the products list and the sent list to sent function in order to check if the
		// products exists and if they does send them.
		sent (productsList, numOfProducts, sentList, numOfSentProducts);	
	}
	//case clean
	else if (!strcmp(commandName, CLEAN))
	{
		char* date = argv[3];		
		int year;
		int month; 
		//seperate the date given in the line to year and month.
		sscanf(date, "%d-%d", &year, &month);
		//if the year is a legal year value (means bigger than 0) and the month is a legal month 
		//(between 0 and 12) than sending the list and the date to the clean function in order to 
		//check which products  should be cleared, otherwise print error
		if (year < FIRST_LEGAL_YEAR || month > NUM_OF_MONTHS || month < FIRST_LEGAL_MONTH)
		{
			printf("USAGE: waredb <db file> <command> <command arg file>\n");
		}
		else
		{
			clean(productsList, &numOfProducts, year, month);
		}
	}	
	//sorting the list after the action has performed.
	bubbleSort(productsList, numOfProducts);
	fclose(file);
	//opening the file with writing permission in order to write the list to the db
	file = fopen(argv[1], "w");
	int k;
	for (k = 0; k <numOfProducts; k++)
	{
		//puts the products in the db in the matching format.
		fprintf(file, "%s\t%d\t%.3f\t%d-%d\n", productsList[k].name, productsList[k].barcode, \
				productsList[k].quantity, productsList[k].year, productsList[k].month);
	}
	fclose(file);
	return EXIT_SUCCESS;
}
