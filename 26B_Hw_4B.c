/*********************************************************************************
** CIS 26B                                                            Summer, 2017
** Advanced C
******************
**
** Homework 4:
**        Bit Manipulation
**
**********************************************************************************

   This program allows the user to manage stage lights. The stage lights are
   represented by an unsigned short. The program allows you to turn on and off
   the bits represeting the lights in different ways. You can also overlay
   a custom bit pattern over a section of the lights to change it.

   Save the output as a comment at the end of the program!
**********************************************************
**
**  Written By: Patrick Collins          // <-- write your name here
**
**  Date: 3/6/18                 // <-- write the date here
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ON_MASK(num,start) (~(((unsigned short)~0) << num)) << start
#define OFF_MASK(num,start) ~((~(((unsigned short) ~0) << num)) << start)
#define FLUSH while(getchar() != '\n')
#define DELIM "\040\t\n"

enum MENU_STATE { MENU, ALL_ON, CENTER_ON, LEFT_ON, RIGHT_ON, ALL_OFF, CENTER_OFF, LEFT_OFF, RIGHT_OFF, PATTERN, QUIT, ERROR};

void lightManager();
void printHeader();
void printMenu();
void printbits(unsigned short wrd);
unsigned short overlayBitPattern(unsigned short wrd, unsigned short pattern, int startBit);
unsigned short turnOnBitRange(unsigned short wrd, int flag);
unsigned short turnOffBitRange(unsigned short wrd, int flag);
void patternManager(unsigned short *wrd);
int getPattern(unsigned short *pattern, int *startBit, int *numBits);
long getChoice();

int main( void )
{

	printHeader();
	lightManager();

	system("pause");
    return 0;
}

/******************************* printHeader ******************************
 * prints the header displaying information about the program
 *		PRE: none
 *		POST: none
 ***************************************************************************/
void printHeader()
{
	printf("---------------------------- Stage Light Manager ----------------------------\n"
		   "\tWelcome! This program lets you manage stage lights. There are \n"
		   "\t16  lights with 5 lights on the left stage and right stage, \n"
		   "\tand 6 lights on the center stage. You can control each set of \n"
		   "\tlights with their respective menu option. For option number 9\n"
		   "\tyou can enter a binary pattern along with the starting light\n"
		   "\to overlay a specific light pattern on the current configuration.\n"
		   "-----------------------------------------------------------------------------\n");
}

/******************************* printMenu ******************************
 * prints the menu to the screen.
 *		PRE: none
 *		POST: none
 ************************************************************************/
void printMenu()
{
	printf("\t1) turn on all lights\n"
		   "\t2) turn on center stage lights (lights 5-10)\n"
		   "\t3) turn on left stage lights (lights 11-15)\n"
		   "\t4) turn on right stage lights (lights 0-4)\n"
		   "\t5) turn off all lights\n"
		   "\t6) turn off center stage lights\n"
		   "\t7) turn off left stage lights\n"
		   "\t8) turn off right stage lights\n"
		   "\t9) enter on/off pattern\n"
		   "\t10)quit the menu\n");
}

/******************************* lightManager ******************************
 * manages the user input for handling the user input. Calls the appropriate
 * functions to modify the stage lights based on the input.
 *		PRE: none
 *		POST: none
 ****************************************************************************/
void lightManager()
{
	enum MENU_STATE state;
	unsigned short stageLights = 0;

	do
	{
		printMenu();
		state = getChoice();

		switch (state)
		{
			case ALL_ON:
				stageLights |= ((unsigned short) ~0);
				printbits(stageLights);
				break;
			case CENTER_ON:
			case LEFT_ON:
			case RIGHT_ON:
				stageLights = turnOnBitRange(stageLights, state);
				printbits(stageLights);
				break;
			case ALL_OFF:
				stageLights &= 0;
				printbits(stageLights);
				break;
			case CENTER_OFF:
			case LEFT_OFF:
			case RIGHT_OFF:
				stageLights = turnOffBitRange(stageLights, state);
				printbits(stageLights);
				break;
			case PATTERN:
				patternManager(&stageLights);
				break;
			case QUIT:
				break;
			case ERROR:
				break;
			default:
				printf("Illegal menu selection! Try again!\n");
				break;
		} 
	} while (state != QUIT);
}

/******************************* getChoice ******************************
 * gets the menu choice from the user
 *		PRE: none
 *		POST: long - user choice
 ************************************************************************/
long getChoice()
{
	char *input[3], *check;
	long choice;
	printf("Enter menu choice: ");
	fgets(input, sizeof(input), stdin);
	choice = strtol(input, &check, 10);
	
	if (*check == '\040')
	{
		printf("Only one menu selection allowed! Try again!\n");
		return ERROR;
	}
	else if (*check != '\n')
	{
		printf("Illegal menu selection! Try again!\n");
		return ERROR;
	} 

	return choice;
}

/************************** turnOnBitRange ******************************
 * turns on the bits in the passed word based on the given flag.
 *		PRE: wrd - the word that will be modified
 *			 flag - determines which range of bits are set
 *		POST: the bits in the range determined by the flag are turned
 *			  on
 ************************************************************************/
unsigned short turnOnBitRange(unsigned short wrd, int flag)
{
	switch (flag)
	{
		case CENTER_ON:
			wrd |= ON_MASK(6, 5);
			break;
		case LEFT_ON:
			wrd |= ON_MASK(5, 11);
			break;
		case RIGHT_ON:
			wrd |= ON_MASK(5, 0);
			break;
		default:
			break;
	}
	return wrd;
}

/************************** turnOffBitRange ******************************
* turns off the bits in the passed word based on the given flag.
*		PRE: wrd - the word that will be modified
*			 flag - determines which range of bits are set
*		POST: the bits in the range determined by the flag are turned
*			  off
************************************************************************/
unsigned short turnOffBitRange(unsigned short wrd, int flag)
{
	switch (flag)
	{
	case CENTER_OFF:
		wrd &= OFF_MASK(6, 5);
		break;
	case LEFT_OFF:
		wrd &= OFF_MASK(5, 11);
		break;
	case RIGHT_OFF:
		wrd &= OFF_MASK(5, 0);
		break;
	default:
		break;
	}
	return wrd;
}

/*************************** patternManager *******************************
 * manages the application of the user input pattern to the current
 * stage light configuration.
 *		PRE: wrd - pointer to the stage light configuration
 *		POST: the pattern is applied to the stage lights
 *************************************************************************/
void patternManager(unsigned short *wrd)
{
	unsigned short pattern;
	int offset, numBits, isValid = -1;

	while (isValid == -1)
		isValid = getPattern(&pattern, &offset, &numBits);
	if (isValid)
	{
		*wrd = overlayBitPattern(*wrd, pattern, offset);
		printbits(*wrd);
	}
}

/************************** getPattern *************************************
 *  gets and validates the pattern the user inputs
 *		PRE: pattern - pointer to the pattern the user has inputed
 *			 startBit - pointer to the bit position the pattern will start
 *			 numBits - pointer to the size of the pattern
 *		POST: pattern, startBit, and numBits assigned if input is valid.
 *			  0 - if no input
			  1 - if input is valid
 *			 -1 - if input is invalid
 ***************************************************************************/
int getPattern(unsigned short *pattern, int *startBit, int *numBits)
{
	char input[20], *token, *pos;
	printf("Enter starting light number and on/off pattern: ");
	fgets(input, 20, stdin);
	token = strtok(input, DELIM);

	if (!token)
		return 0;

	*startBit = strtol(token, &pos, 10);
	if (strlen(pos) > 0 || *startBit < 0 || *startBit > 15)
	{
		printf("Illegal starting light number! Try again!\n");
		return -1;
	}
	
	token = strtok(NULL, DELIM);
	if (!token)
	{
		printf("You did not enter a bit pattern! Try again!\n");
		return -1;
	}

	if (strlen(token) > 16)
	{
		printf("Bit pattern is too large! Try again!\n");
		return -1;
	}

	*pattern = strtol(token, &pos, 2);
	if (strlen(pos) > 0 || *pattern > ((unsigned short)~0))
	{
		printf("Illegal bit pattern! Try again!\n");
		return -1;
	}

	*numBits = strlen(token);
	if (strtok(NULL, DELIM))
	{
		printf("Too many entries on line! Try again!\n");
		return -1;
	}
	return 1;
}

/*********************** overlayBitPattern ***************************
 * overlays the passed bit pattern onto the word
 *		PRE: wrd - the word that will have the pattern applied
 *			 pattern - the pattern to be applied
 *			 startBit - the bit to start the shifting
 *		POST: the word with the pattern overlayed
 *********************************************************************/
unsigned short overlayBitPattern(unsigned short wrd, unsigned short pattern, int startBit)
{
	wrd &= OFF_MASK(startBit - 1, startBit);
	return wrd |= pattern << startBit;
}

/*************************** testbit **********************************
 * Return 1 if bit_to_test is set and 0 if it is unset
 *		PRE: wrd - the word of data to test
 *			 bit_to_test - the bit to check
 *		POST: 1 if the bit is set
 *			  0 if not set
 ***********************************************************************/
int  testbit(unsigned short wrd, int bit_to_test)
{
	wrd >>= bit_to_test;
	wrd &= 1;
	return wrd;
}

/*************************** printBits *********************************
* prints the bits of the passed word in the pattern xxxxx xxxxxx xxxxx
*		PRE: wrd - the word that will be printed	 
*		POST: none
************************************************************************/

void printbits(unsigned short wrd)
{
	int  testbit(unsigned short wrd, int bit_to_test);
	int i;

	for (i = 15; i >= 0; i--)
	{
		printf("%1d", testbit(wrd, i));
		if (i == 5 || i == 11)
			printf(" ");
	}
	printf("\n");
	return;
}

/*	===================== Output ===================== */
/*	Results:
 ---------------------------- Stage Light Manager ----------------------------
        Welcome! This program lets you manage stage lights. There are
        16  lights with 5 lights on the left stage and right stage,
        and 6 lights on the center stage. You can control each set of
        lights with their respective menu option. For option number 9
        you can enter a binary pattern along with the starting light
        o overlay a specific light pattern on the current configuration.
-----------------------------------------------------------------------------
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 1 3
Only one menu selection allowed! Try again!
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 3x
Illegal menu selection! Try again!
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 1
11111 111111 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 6
11111 000000 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 9
Enter starting light number and on/off pattern: 6 9
Illegal bit pattern! Try again!
Enter starting light number and on/off pattern: 6
You did not enter a bit pattern! Try again!
Enter starting light number and on/off pattern: 6 9 foo
Illegal bit pattern! Try again!
Enter starting light number and on/off pattern: 6 101010
11111 010100 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 9
Enter starting light number and on/off pattern:
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 2
11111 111111 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 7
00000 111111 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 8
00000 111111 00000
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 3
11111 111111 00000
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 4
11111 111111 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 4
11111 111111 11111
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 5
00000 000000 00000
        1) turn on all lights
        2) turn on center stage lights (lights 5-10)
        3) turn on left stage lights (lights 11-15)
        4) turn on right stage lights (lights 0-4)
        5) turn off all lights
        6) turn off center stage lights
        7) turn off left stage lights
        8) turn off right stage lights
        9) enter on/off pattern
        10)quit the menu
Enter menu choice: 10
Press any key to continue . . .
 
 */