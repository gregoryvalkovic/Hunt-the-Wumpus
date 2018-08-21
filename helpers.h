/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef enum boolean
{
	FALSE = 0,
	TRUE
} Boolean;

#define NEW_LINE_SPACE 1
#define NULL_SPACE 1

/**
 * This is used to compensate for the extra character spaces taken up by
 * the '\n' and '\0' when input is read through fgets().
 **/
#define EXTRA_SPACES (NEW_LINE_SPACE + NULL_SPACE)

#define EMPTY_STRING ""

/**
 * Call this function whenever you detect buffer overflow but only call this
 * function when this has happened.
 **/
void readRestOfLine();

/* TODO not provided */
typedef enum
{
	ReadInputResultNull, ReadInputResultOverflow, ReadInputResultSuccess
} ReadInputResult;

ReadInputResult readInput(char * str, int num, FILE * stream);
ReadInputResult getInput(char * prompt, char * input, int inputSize);
void printInvalidInput();
void enableWindowsCompatibility();
void printLine(int length);
void printBlank(int length);

#endif
