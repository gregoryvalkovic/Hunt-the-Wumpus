/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#ifndef BOARD_H
#define BOARD_H

#include "helpers.h"
#include "player.h"

#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5
#define BOARD_LINE_LENGTH 16

#define NUM_PITS 4
#define NUM_BATS 3
#define NUM_WUMPUS 1

typedef enum cell
{
	board_EMPTY,		
	board_TRAVERSED, /* an empty cell player has previously visited */
	board_BATS,		
	board_PIT,		
	board_WUMPUS,
	board_PLAYER
} Cell;

#define EMPTY_OUTPUT "  "
#define PLAYER_OUTPUT "##"
#define TRAVERSED_OUTPUT "**"

typedef Cell Board[BOARD_HEIGHT][BOARD_WIDTH];

Board BOARD_1, BOARD_2;

typedef enum playerMove
{
	board_PLAYER_MOVED,
	board_PLAYER_KILLED,
	board_BAT_CELL,
	board_OUTSIDE_BOUNDS
} PlayerMove;

typedef enum arrowHit
{
	board_ARROW_MISSED,
	board_WUMPUS_KILLED,
	board_ARROW_OUTSIDE_BOUNDS
} ArrowHit;

/* Note that 'board' does not use any functions from player, game or
 * huntWump and doesn't use the type 'Player' (although it does know 
 * about the type 'Position').
 */

/**
 * Load the board - set all the cells in board to those present in boardToLoad.
 */
void board_Load(Board board, Board boardToLoad);

/**
 * Try to set the cell at the provided position to board_PLAYER.
 * Note that if the position is outside the bounds of the board then you 
 * should return FALSE. Returns FALSE if the position isn't empty 
 * (note empty can be a cell that has been traversed previously).
 * Return TRUE when the cell at the position is successfully set to 
 * board_PLAYER.
 */
Boolean board_PlacePlayer(Board board, Position position);

/**
 * Try to move the player from the existing position to
 * a new position.
 *
 * If this new position is outside the bounds of the board return
 * board_OUTSIDE_BOUNDS. 
 * Note that if board_OUTSIDE_BOUNDS occurs then the board cells and
 * player position are not modified.
 * If the new position is on-top of a PIT or WUMPUS cell then return 
 * board_PLAYER_KILLED. 
 * If the new position is on-top of a BAT, return board_BAT_CELL.
 * If the new position is board_EMPTY or board_TRAVERSED 
 * then remove PLAYER from the current player's position on the board 
 * (set cell to board_TRAVERSED) and set the new position to board_PLAYER.
 * Note that in this case whatever called this function will need to 
 * update the position member of the player structure. 
 */
PlayerMove board_MovePlayer(Board, Position position, Position newPosition);

/**
 * Try to hit something with an arrow at the given position.
 * If this position is outside the bounds of the board return
 * board_ARROW_OUTSIDE_BOUNDS. board_ARROW_MISSED is returned if
 * a valid position didn't contain the wumpus, otherwise return
 * board_WUMPUS_KILLED.
 */
ArrowHit board_FireArrow(Board, Position position);

/**
 * Display the board according to the assignment specification. 
 * Here is an example output:

    0  1  2  3  4 
   ----------------
 0 |**|##|  |  |  |
   ---------------- 
 1 |  |  |  |  |  |
   ----------------  
 2 |  |  |  |  |  |
   ---------------- 
 3 |  |  |  |  |  |  
   ----------------
 4 |  |  |  |  |  |
   ---------------- 
 */
void board_Display(Board board);

/**
 * Display the board warnings according to the assignment specification. 
 */
void board_DisplayWarnings(Board board, Position positon);

#endif
