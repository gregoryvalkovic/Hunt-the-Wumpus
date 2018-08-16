/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#ifndef PLAYER_H
#define PLAYER_H

#include "helpers.h"

#define INIT_ARROW_COUNT 5

typedef enum direction
{
	player_NORTH,
	player_EAST,
	player_SOUTH,
	player_WEST
} Direction;

typedef struct position
{
	int x;
	int y;
} Position;

typedef struct player
{
	Position position;
	unsigned numArrows;
} Player;

/* Note that 'player' does not use any functions from board, game or
 * huntWump.
 */


/**
 * Initialise the player - set the player position to the provided value
 * and set the number of arrows to INIT_ARROW_COUNT.
 */
void player_Initialise(Player * player, Position position);


/**
 * Gets the new position as if the player moved from the player's
 * current position in the specified direction.
 *
 * Note the new position can be outside the bounds of the board and could be on
 * cell that would kill the player. Also note that you should not modify 
 * the current position of the player.
 */
Position player_GetNextPosition(Position position, Direction direction);

/**
 * Set the player's position to the provided position. 
 */
void player_UpdatePosition(Player * player, Position position);

#endif
