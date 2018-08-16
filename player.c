/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#include "player.h"

void player_Initialise(Player * player, Position position){
   player->position = position;
   player->numArrows = INIT_ARROW_COUNT;
}


Position player_GetNextPosition(Position position, Direction direction){
   Position newPosition;
   
	if (direction == player_NORTH) {
		newPosition.x = position.x;
		newPosition.y = position.y - 1;
	}
	if (direction == player_SOUTH) {
		newPosition.x = position.x;
		newPosition.y = position.y + 1;
	}
	if (direction == player_WEST) {
		newPosition.x = position.x - 1;
		newPosition.y = position.y;
	}
	if (direction == player_EAST) {
		newPosition.x = position.x + 1;
		newPosition.y = position.y;
	}
	return newPosition;
}

void player_UpdatePosition(Player * player, Position position) {
   player->position.x = position.x;
   player->position.y = position.y;
}

