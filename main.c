/*
Car Sales made in 'C'
For ARU ComSci Project
By Lloyd Alex Porter
*/


// Declaring Variables and Constants //////////////////////////////////////////////////////////////



#define _CRT_SECURE_NO_WARNINGS	// needed to allow Visual Studio to work with scanf() and other input statements

#include <stdio.h>				// include the standard input / outputs
#include <stdlib.h>				// include the standard library
#include <stdbool.h>			// include the standard boolean library
#include <string.h>				// include the string base for string comparisons
#include <ctype.h>				// include the colour type library
#include <time.h>				// include the time base library



// ! - Setting up Car Arrays
#define arrayLength 9 // all arrays are this length, always this length, so Index's are the same.
static unsigned char arrayCarID[arrayLength] = { 1,2,3,4,5,6,7,8,9 }; // main ID that the user will use to specify which car they are after
static int arrayCarValue[arrayLength] = { 15900,22790,17995,19795,12815,15805,28000,21380,19995 }; // the price of each car
static char arrayCarName[arrayLength][27] = { "Toyota Corolla","Ford F-Series","Volkswagen Golf","Volkswagen Beetle","Ford Escort","Honda Civic","Ford Model T","Honda Accord","Volkswagen Passat" }; // name of each car model
unsigned char arrayCarAvailability[arrayLength] = { 12,9,23,0,15,17,21,0,7 }; // the amount of available cars
unsigned char arrayCarSales[arrayLength] = { 0,0,0,0,0,0,0,0,0 }; // empty array of the total sales of each of the cars
int arrayCarProfits[arrayLength] = { 0,0,0,0,0,0,0,0,0 }; // empty array of the total profit for selling each car
char arrayOrder[arrayLength]; // empty array to store the order of each cars id so the program can print in a sorted order


// ! - Menu Constants
#define _viewCars '1'		// whenever user enters 1, runs the View Cars Menu selection
#define _buyCars '2'		// whenever user enters 2, runs the Buy Cars Menu selection
#define _viewSales '3'		// whenever user enters 3, runs the View Sales Menu selection
#define _quitProgram 'Q'	// whenever user enters Q, runs the Quit Program Menu selection

// ! - Array Sorting Constants
#define _carAvailability 1	// when the array sorting is passed a '1' - then sort availability
#define _carSales 2			// when the array sorting is passed a '2' -	then sort sales

// ! - Boolean Constants
#define TRUE 1 		// creating TRUE value
#define FALSE 0 	// creating FALSE value

// ! - Setting up Constants for Customer
#define _discountMinimumAge 16 		// minimum age for a discount
#define _discountMaximumAge 25 		// maximum age for a discount
#define _discountPercentage 0.20f 	// the percentage which is deducted from the original price because of discounts

// ! - Setting up Constants for Admin
#define _adminUsername "admin"    	// very secure username
#define _adminPassword "password" 	// even more secure password

// ! - Setting up Constants for File Management
#define _file "D://data.csv"		// location of the data file
#define _openFile 0 				// if the file is open, status is ZERO
#define _closeFile 1 				// if file is closed, status is ONE
#define _errorWithFile 2 			// else, an error, status is TWO
#define _writing "w" 				// for the file 'mode' writing
#define _reading "r" 				// for the file 'mode' reading
#define _appending "a" 				// for the file 'mode' appending






// File Management Code ///////////////////////////////////////////////////////////////////////////
//(Heavily) Inspired by Raz's Code



// ! - Create File connection
FILE* file; // defines 'file'
unsigned char fileStatus = _closeFile; // begin with closed file


// ! - Setup for File
FILE* createFile(char fileTitle[201]) {

	file = fopen(fileTitle, _writing); // opens writable file
	if (file != NULL) { // if file has content, close file
		fclose(file);
	}
	return file;
}


// ! - Create and Open the File
void openFile(char fileTitle[201], char dataMode[4]) {

	file = fopen(fileTitle, dataMode); // open writable files

	if (file == NULL) { // if file's value is NULL, an error as occured, file does not exist

		if (createFile(fileTitle) == NULL) { // create a file, if an error still occurs, something is very wrong
			fileStatus = _errorWithFile; // set status to ERROR

		}
		else { // file is now created
			openFile(fileTitle, dataMode); // restart this function, now with a created file
		}

	}
	else { // everything has worked
		fileStatus = _openFile; // set status to OPEN
	}
}


// ! - Close the File
void closeFile() {

	if (fileStatus == _openFile) { 	// if file is open
		fclose(file); 				// close the file
		fileStatus = _closeFile; 	// set ststus to CLOSE
	}
}


// ! - Read the Data Contained in the File
void readDataFromFile() {

	for (int index = 0; index < arrayLength; index++) { // loops nine times for nine cars

		// ! - Declare some Variables
		int carProfits = 0;
		int carSales = 0;

		// ! - Get Data from Line
		int lineData = fscanf(file, "%i,%i", &carProfits, &carSales);

		// ! - Update Necessary Arrays
		arrayCarProfits[index] = carProfits;
		arrayCarSales[index] = carSales;
		arrayCarAvailability[index] -= arrayCarSales[index];
	}
}


// ! - Get Data Contained in the File
void getDataFromFile() {

	openFile(_file, _reading);		// open file, prepare to read

	if (fileStatus == _openFile) {	// if ready to read
		readDataFromFile();			// read file

	}
	else if (fileStatus == _errorWithFile) {	// else if error reading file
		printf(" An Error Occurred while trying to read this file: %s.\n\n Press Return to Continue to the Menu...", _file); // print error
		getchar();
		getchar(); // wait for user input
	}

	closeFile(); // close the file after data has been received
}


// ! - Write the Data to the File
void writeDataToFile() {

	for (int index = 0; index < arrayLength; index++) { // loops nine times for nine cars

		char profits[50];			// create variables, ready for assignment
		char sales[50];

		// '_itoa' function was not recognised on my computer, so found this alternative on stackoverflow.com
		sprintf(profits, "%d", arrayCarProfits[index]);		// converts from numbers to string,
		sprintf(sales, ",%d", arrayCarSales[index]);		//  and again

		strcat(profits, sales);		// connect strings together

		if (index < arrayLength) {	// create a new line, if not there yet
			fprintf(file, "%s\n", profits);	// write data to file
		}
	}
}


// ! - Save Data to the File
void saveDataToFile() {

	openFile(_file, _writing);

	if (fileStatus == _openFile) {
		writeDataToFile();

	}
	else if (fileStatus == _errorWithFile) {	// else if error reading file
		printf(" An Error Occurred while trying to read this file: %s.\n\n Press Return to Continue to the Menu...", _file); // print error
		getchar();
		getchar(); // wait for user input
	}

	closeFile(); // close the file after data has been received
}







// Main Program Functions /////////////////////////////////////////////////////////////////////////



// ! - Clears the Consoles Screen
void clearScreen() {

	system("cls"); //clears screen  on Windows CMD Prompt
	//system("clear"); //clears screen on macOS Terminal

}


// ! - Wait Function before Continuing to Menu
void returnToMenu(int times) {

	// ! - Wait for user input before re-looping the program
	printf("\n\n Press Return to Continue to the Menu...\n");
	switch (times) { // how many getchar()'s are needed

	case 1: // needs one getchar()
		getchar();
		break;
	case 2: // needs two getchar()'s
		getchar();
		getchar();
		break;
	}
	printf("\n");
}


// ! - Displays Error for Incorrect Password or Username
void incorrectPorU(char pORu[8]) {          // prints statement, wait for input, then restart

	printf("\n <!########################!>");
	printf("\n <!#> Incorrect %s <#!>", pORu);
	printf("\n <!#> Restarting Program <#!>");
	printf("\n <!########################!>");
	printf("\n\n\n");

	returnToMenu(2);
}


// ! - Get Character from User Input
char consoleGetChar(char question[201]) {

	char answer;
	printf("%s", question);
	//scanf("\n%c", &answer);				// old version, keeping just in case
	answer = getchar();

	return answer;
}


// ! - Get Validated Number from User Input
char consoleGetNumberValidated(char question[2], int numberMIN, int numberMAX, bool doTimeout) {

	bool invalid = TRUE;
	char answer;
	int returnValue, answerNumber, timeout = 0;
	char answerString;

	do {

		printf("\n%s", question);
		answer = scanf("\n %d%c", &answerNumber, &answerString);
		//(scanf("%d%c", &inputCarID, &validateCarID_String) != 2 || validateCarID_String != '\n')

		if (((answer == 2) || (answerString == '\n')) && ((answerNumber >= numberMIN) && (answerNumber <= numberMAX))) {		// answer is a valid Number.

			returnValue = answerNumber;

			invalid = FALSE;

		}
		else {

			if (timeout == 2) {

				printf("\n Too many failed attempts,\n quitting to the Menu.");
				returnToMenu(2);
				getchar();
				return 0;

			}
			else if (doTimeout == TRUE) {

				timeout += 1;
			}

			printf("\n Invalid Input,\n try again.\n");
			getchar();

		}

	} while (invalid == TRUE); // repeats until input is valid

	return returnValue;
}


// ! - Sorts the Array passed to it.
int sortArray(int array[arrayLength], int index) {

	int x;
	int tempValue;
	int arrayOrder[arrayLength];

	// ! - General Bubble Sort Algorithm
	for (int index = 0; index < arrayLength; index++) {
		for (x = index + 1; x < arrayLength; x++) {
			if (array[index] < array[x]) {
				tempValue = array[index];
				array[index] = array[x];
				array[x] = tempValue;
			}
		}
	}

	for (int index = 0; index < arrayLength; index++) { // transfer data to array
		arrayOrder[index] = array[index];
	}

	return arrayOrder[index];
}


// ! - Prepares the Array ready to Sort it
void prepareToSort(char whichArray) {

	// ! - Initialises Temporary Arrays
	int arraySorted[arrayLength];
	int arrayCopy[arrayLength];

	switch (whichArray) { // decide between sorting Car Availability or Car Sales

	case _carAvailability: // Car Availability

		for (int index = 0; index < arrayLength; index++) { // duplicate array
			arrayCopy[index] = arrayCarAvailability[index];
		}

		for (int index = 0; index < arrayLength; index++) { // send each value to be sorted
			arraySorted[index] = sortArray(arrayCopy, index);
		}

		for (int x = 0; x < arrayLength; x++) {	// makes a note of where each value has been moved, creates a map
			for (int y = 0; y < arrayLength; y++) {
				// if matched, add location to order array
				if (arraySorted[y] == arrayCarAvailability[x]) {
					arrayOrder[y] = x + 1;	// prevents ZERO by adding ONE
				}
			}
		}
		break; // must use break, or will continue to next case.

	case _carSales: // Car Sales

		for (int index = 0; index < arrayLength; index++) { // duplicate array
			arrayCopy[index] = arrayCarSales[index];
		}

		for (int index = 0; index < arrayLength; index++) { // send each value to be sorted
			arraySorted[index] = sortArray(arrayCopy, index);
		}

		for (int x = 0; x < arrayLength; x++) {	// makes a note of where each value has been moved, creates a map
			for (int y = 0; y < arrayLength; y++) {
				// if matched, add location to order array
				if (arraySorted[y] == arrayCarSales[x]) {
					arrayOrder[y] = x + 1;	// prevents ZERO by adding ONE
				}
			}
		}
		break; // must use break, or will continue to next case.
	}
}


// ! - Gets User's Input for Selecting Menu
char showMenu() {

	// ! - Introduction Page
	printf("! Welcome to the ACME Car Sales Service !"); // nice little ASCII artwork
	printf("\n                _______");
	printf("\n               //  || \\\\");
	printf("\n         _____//___||__\\\\___");
	printf("\n         )  _     >    _    \\");
	printf("\n         |_/ \\________/ \\___|O o º");
	printf("\n     ______\\_/________\\_/___________");

	// ! - Displays Menu Options
	printf("\n\n\n How may we help you today?\n");
	printf("\n 1 - View Cars");
	printf("\n 2 - Buy Cars");
	printf("\n 3 - View Sales");
	printf("\n Q - Quit Program");

	char inputMenuSelect = consoleGetChar("\n\n Enter Answer Here: "); // gets user input
	inputMenuSelect = toupper(inputMenuSelect); // capitalises the input, in case of a 'q'
	printf("\n");
	return inputMenuSelect;
}


// ! - View Cars Menu
void menuViewCars() {

	prepareToSort(_carAvailability); // sorts availability array by number remaining cars left for each model in descending order


	// ! - Listing Unavailable Cars
	printf("\n\n The following Cars are Unavailable:\n ");
	printf("\n %s -|- %s", "ID", "Car Model");
	for (int index = 0; index < 9; index++) { // loops nine times for nine cars
		if (arrayCarAvailability[index] == 0) {
			printf("\n  %i  | %s", arrayCarID[index], arrayCarName[index]);
		}
	}


	// ! - Listing Available Cars
	printf("\n\n\n The following Cars are Available:\n");
	int zeroAvailabilityCheck = arrayLength;
	printf("\n %s -|- %s -|- %s", "ID", "Price", "Car Model (Remaining)");
	for (int index = 0; index < arrayLength; index++) { // loops nine times for nine cars
		int order = arrayOrder[index] - 1;
		if (arrayCarAvailability[order] > 1) { // if car is available, print the id and value and model
			printf("\n  %i  |  %d  | %s (%i)", arrayCarID[order], arrayCarValue[order], arrayCarName[order], arrayCarAvailability[order]);
		}
		else { // else minus one from the availability count
			zeroAvailabilityCheck -= 1;
		}
	}

	// ! - If Zero cars are available, say so and break.
	if (zeroAvailabilityCheck == 0) {
		printf("\n Sorry, there are no cars currently available.");
		printf("\n\n Restarting program.");
	}

	returnToMenu(2);
}


// ! - Buy Cars Menu
void menuBuyCars() {

	// ! - Fetch CarID from User Input in Console						Question			 MIN MAX YesToTimeout
	int inputCarID = consoleGetNumberValidated(" Which car would you like to purchase?\n ID: ", 1, 9, FALSE);
	if (inputCarID == 0) { // if value is 0, then three errors have occured, and must quit function.
		return; // quit function.
	}

	// ! - Making sure this Car Choice are Available
	if (arrayCarAvailability[inputCarID - 1] == 0) { // if true, no cars are available, so say so and quit to menu
		printf("\n We apologise, the %s is currently not available.\n", arrayCarName[inputCarID - 1]);
		returnToMenu(1);
		return; // quit function
	}

	// ! - Print number of available cars left.
	printf("\n\n There are %i %s(s) available.", arrayCarAvailability[inputCarID - 1], arrayCarName[inputCarID - 1]); // prints total cars available
	int inputCarAmount = consoleGetNumberValidated("\n How many cars would you like to purchase?:\n Enter Answer Here: ", 1, arrayCarAvailability[inputCarID - 1], FALSE);
	if (inputCarAmount == 0) { // if value is 0, then three errors have occured, and must quit function.
		return; // quit function.
	}


	// ! - Calculating Total Price
	int totalPrice = inputCarAmount * arrayCarValue[inputCarID - 1];

	// ! - Update Car Availability Count
	arrayCarAvailability[inputCarID - 1] -= inputCarAmount;

	// ! - Add Sales to Array
	arrayCarSales[inputCarID - 1] += inputCarAmount;


	// ! - Fetch Customer's Age from User Input in Console		Question   MIN  MAX  YesToTimeout
	int customerAge = consoleGetNumberValidated("\n Please enter your age:\n ", 16, 100, FALSE);
	if (customerAge == 0) { // if value is 0, then three errors have occured, and must quit function.
		return; // quit function.
	}

	// ! - Check Discount Eligability
	int giveDiscount = FALSE;
	if (customerAge >= _discountMinimumAge && customerAge <= _discountMaximumAge) { // boolean algebra, checks customer is eligable for discount

		// ! - - If TRUE, change value of giveDiscount
		giveDiscount = TRUE;

		// ! - - Calculate Discount
		totalPrice *= (1 - _discountPercentage);
	}

	// ! - Add Profits to the Array
	arrayCarProfits[inputCarID - 1] += totalPrice;

	// ! - Tell the Customer about their Discount
	switch (giveDiscount) {

	case TRUE:		// if TRUE, discount.
		printf("\n You qualify for a discount.");
		break; 		// must use break, or will continue to next case.

	case FALSE: 	// if FALSE, no discount.
		printf("\n No discount was given.");
		break; 		// must use break, or will continue to next case.
	}

	// ! - Displaying Calculations and Output
	printf("\n\n Thank you for your order.");
	printf("\n You have purchased %d car(s).", inputCarAmount);
	printf("\n Total cost will be %d GBP.", totalPrice);

	printf("\n\n\n Thank You for using our Service,");
	printf("\n Have a Nice Day!");
	printf("\n\n");

	returnToMenu(1);
}


// ! - View Sales Menu
void menuViewSales() {

	prepareToSort(_carSales); // sorts sales array by number of sales for each model in descending order

	// ! - Nice little banner, cute
	printf("\n      ____   ___     __    __   _____   __    _ ");
	printf("\n     / _  | |  _ \\  |  \\  /  | |_   _| |  \\  | |");
	printf("\n    / / | | | | \\ \\ |   \\/   |   | |   |   \\ | |");
	printf("\n   / /__| | | | | | | |\\  /| |   | |   | |\\ \\| |");
	printf("\n  / ____  | | |_/ / | | \\/ | |  _| |_  | | \\   |");
	printf("\n /_/    |_| |____/  |_|    |_| |_____| |_|  \\__|");

	int user = FALSE;
	int password = FALSE;
	char inputAdminUsername[201];
	char inputAdminPassword[201];

	// ! - Gets users username input
	printf("\n\n\n Please Enter your Username Below:\n ");
	scanf("%c", &inputAdminUsername);

	// ! - Checks if the Username Matches
	if (strcmp(&inputAdminUsername, _adminUsername) == FALSE) {   // If the Usernames match, set 'u' variable to True
		user = TRUE; // username is correct
	}
	else { // else it's wrong
		incorrectPorU("Username"); // display error text
		return;
	}

	int pCount = 3; // how many trys to enter the right password
	// ! - Checks if the Password Matches
	while (pCount > 0) {
		printf("\n Please Enter your Password Below:\n ");
		scanf("%c", &inputAdminPassword);

		if (strcmp(&inputAdminPassword, _adminPassword) == FALSE) {
			password = TRUE; // password is Correct
			pCount = 0; // escape While loop
		}
		else if (pCount > 1) { // try again
			printf("\n\n Incorrect Password, try again.\n");
		}

		pCount -= 1; // validation count -1
	}


	// ! - Checks if Password is Correct
	if (password == TRUE) { // if TRUE then password is correct, run rest of program

		// ! - Checks if any cars have been sold
		int zeroSalesCheck = 0;
		for (int index = 0; index < 9; index++) {
			zeroSalesCheck += arrayCarSales[index]; // adds every array value to total
		}


		if (zeroSalesCheck == 0) { // then there are no sold cars and no sales to look at

			printf("\n Sorry, there were no cars sales yet today.");
			printf("\n Restarting program.\n\n\n");
			returnToMenu(2);
			return;
		}


		// !! - Listing Sold Cars
		printf("\n %s -|- %s -|- %s -|- %s %s", "ID", "Profit", "Sold", "Car Model", "(Car Price)");
		for (int index = 0; index < 9; index++) {
			int order = arrayOrder[index] - 1; // the SortedArray's value equals the Car's ID, so minus ONE to match Index's
			if (arrayCarProfits[order] > 0) { // if ZERO, value
				printf("\n %2i  | %7d  |   %.2i   | %s (%d)", arrayCarID[order], arrayCarProfits[order], arrayCarSales[order], arrayCarName[order], arrayCarValue[order]);
			}
		}


		// !! - Print the Total
		int totalProfit = 0;
		for (int index = 0; index < 9; index++) { // adds every value to a total
			totalProfit += arrayCarProfits[index];
		}
		printf("\n ================================================");
		printf("\n Total Profit: %i GBP.", totalProfit);

		returnToMenu(2);
		return;

	}
	else { // else password is Incorrect, show error and return
		incorrectPorU("Password"); // display error text
		return;
	}
}


// ! - Quit Program, Say Farewell
void menuQuitProgram() {

	printf("\n Thank You for using our Service,");
	printf("\n Have a Nice Day!");
	printf("\n\n\n Press Return to Exit the Program...");

	getchar(); // Duo-GetChar() so program waits for user input
	getchar(); // this is needed when ran though Command Prompt on Windows
}


// ! - Beginning of the Program, Show Menu Options, Run Corresponding Function. Loop.
int main() {

	getDataFromFile();		// open the file and update arrays with file data

	char inputMenuSelect;

	do { // do/while superloop used for the menu selection

		clearScreen(); // clear screen
		inputMenuSelect = showMenu(); // display the menu to show options to the user

		switch (inputMenuSelect) { // Menu selector using Switch-Case function

		case _viewCars: 	// View Cars

			clearScreen(); 		// clear screen
			menuViewCars(); 	// run View Cars Menu
			break; 				// must use break, or will continue to next case.

		case _buyCars:		// Buy Cars

			clearScreen(); 		// clear screen
			menuBuyCars(); 		// run Buy Cars Menu
			break; 				// must use break, or will continue to next case.

		case _viewSales: 	// View Sales

			clearScreen(); 		// clear screen
			menuViewSales(); 	// run View Sales Menu
			break; 				// must use break, or will continue to next case.

		}
	} while (inputMenuSelect != _quitProgram); // the loop is broken, finishes the program.

	clearScreen();	 	// clear screen
	menuQuitProgram(); 	// run Quit Program Menu
	saveDataToFile();	// save data to the file
}
