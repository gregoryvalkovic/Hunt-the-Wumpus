/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#include "huntWump.h"


typedef enum {
	InvalidInput, ValidInput	
} InputValidity;

void printMenu();


int main() {
   /**
    * Here's the main function. You can write the "main menu" loop/code
    * here or you can make separate functions - up to you.
    */
    
    char input[10] = "";
    char prompt[] = "Please enter your choice: ";
    int inputResult;
    
    /* Menu Loop */
   	while(TRUE) {
		printMenu();
   		inputResult = getInput(prompt, input, 10);
   		
   		if (inputResult == ReadInputResultSuccess) {
   			if(input[0] == '1') {
   				game_PlayGame();
   			}
   			else if(input[0] == '2') {
   				showStudentInformation();
   			}
   			else if(input[0] == '3') {
   				break;
   			}
			else {
				printInvalidInput();
				continue;
			}
   		}
   	}
   	printf("Good bye! \n\n");
   	return EXIT_SUCCESS;
}


void showStudentInformation() {
  	printLine(34);
	printf("Name: %s\nStudent ID: %s\nEmail: %s\n",
   		STUDENT_NAME, STUDENT_ID, STUDENT_EMAIL);
	printLine(34);
}


void printMenu() {
	printf("\nWelcome to Hunt the Wumpus\n");
	printLine(26);
   	printf("1. Play game\n2. Show student information\n3. Quit\n\n");
}
