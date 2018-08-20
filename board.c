/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#include "board.h"

Board BOARD_1 =
{
   	{ board_EMPTY,  board_EMPTY,  board_EMPTY,  board_PIT,    board_EMPTY},
	{ board_EMPTY,  board_EMPTY,  board_EMPTY,  board_EMPTY,  board_EMPTY},
	{ board_BATS,   board_EMPTY,  board_EMPTY,  board_WUMPUS, board_EMPTY},
	{ board_EMPTY,  board_PIT,    board_EMPTY,  board_EMPTY,  board_EMPTY},
	{ board_EMPTY,  board_EMPTY,  board_BATS,   board_EMPTY,  board_EMPTY}
};

Board BOARD_2 =
{
	{ board_PIT,    board_EMPTY,  board_PIT,    board_EMPTY,  board_EMPTY},
	{ board_EMPTY,  board_EMPTY,  board_EMPTY,  board_EMPTY,  board_EMPTY},
	{ board_EMPTY,  board_EMPTY,  board_BATS,   board_EMPTY,  board_EMPTY},
	{ board_EMPTY,  board_EMPTY,  board_EMPTY,  board_EMPTY,  board_BATS },
	{ board_EMPTY,  board_WUMPUS, board_EMPTY,  board_EMPTY,  board_EMPTY},
};


void printHeader();
void printBlank(int length);
Boolean isInRange(Position p1, Position p2);


void board_Load(Board board, Board boardToLoad) {
	int i, j;
	for (i=0; i < BOARD_HEIGHT; i++) {
   	   for (j=0; j < BOARD_WIDTH; j++) {
   	   		board[i][j] = boardToLoad[i][j];   
   	   }
   }
}


Boolean board_PlacePlayer(Board board, Position position) {
	if (board[position.y][position.x] == board_EMPTY) {
		board[position.y][position.x] = board_PLAYER;
		return TRUE;
	}
	else {
		return FALSE;
	}

}


PlayerMove board_MovePlayer(Board board, Position playerPosition, 
			    Position nextPosition) {
   Cell nextCell = board[nextPosition.y][nextPosition.x];
	
	if (nextPosition.x < 0 || nextPosition.x > 4 ||
		nextPosition.y < 0 || nextPosition.y > 4) 
	{
		return board_OUTSIDE_BOUNDS;	
	}
	
	else if (nextCell == board_WUMPUS || nextCell == board_PIT) {
		return board_PLAYER_KILLED;
	}
	
	else if (nextCell == board_BATS) {
		return board_BAT_CELL;
	}
	else {
		board[playerPosition.y][playerPosition.x] = board_TRAVERSED;
		board[nextPosition.y][nextPosition.x] = board_PLAYER;
		return board_PLAYER_MOVED;
	}
}


ArrowHit board_FireArrow(Board board, Position position) {
	ArrowHit arrowHit;
	if (board_InBounds(position)) {
   	   if (board[position.y][position.x] == board_WUMPUS) {
   	   	   arrowHit = board_WUMPUS_KILLED;
   	   }
   	   else {
   	   	   arrowHit = board_ARROW_MISSED;
   	   }
   }
   else {
   	   arrowHit = board_ARROW_OUTSIDE_BOUNDS;
   }
   return arrowHit;
}


void board_Display(Board board) {
	int i, j;
	printHeader();
	for (i=0; i < BOARD_HEIGHT; i++) {
		printf("%d |", i);
		
		for (j=0; j < BOARD_WIDTH; j++) {
			Cell currentCell = board[i][j];
			if(currentCell == board_PLAYER) {
				printf(PLAYER_OUTPUT);
			}else if(currentCell == board_TRAVERSED) {
				printf(TRAVERSED_OUTPUT);
			}else {
				printf(EMPTY_OUTPUT);	
			}
			printf("|");
		}
		printf("\n  ");
		printLine(BOARD_LINE_LENGTH);
	}
}


/* Prints the first row of the board */
void printHeader() {
	int i;
	printBlank(2);
	for (i=0; i < BOARD_WIDTH; i++) {
		printf("  %d", i);   
	}
	printf("\n  ");
	printLine(BOARD_LINE_LENGTH);
}


void board_DisplayWarnings(Board board, Position position){
   	int i, j;
	Boolean printedBats, printedPit, printedWumpus;
	Position currPosition;
	Cell cell;
	
	printedBats = FALSE;
	printedPit = FALSE;
	printedWumpus = FALSE;

	for (i=0; i < BOARD_HEIGHT; i++) {
		for (j=0; j < BOARD_WIDTH; j++) {
			currPosition.x = i;
			currPosition.y = j;
			if(isInRange(currPosition, position)) {
				cell = board[currPosition.y][currPosition.x];

				if (cell == board_BATS && !printedBats) {
					printf("You hear flapping!\n");
					printedBats = TRUE;					
				}
				if (cell == board_PIT && !printedPit) {
					printf("You feel a breeze!\n");
					printedPit = TRUE;
				}
				if (cell == board_WUMPUS && !printedWumpus) {
					printf("You smell a wumpus!\n");
					printedWumpus = TRUE;
				}
			}
		}
	}
	printf("\n");
}


/* 	Takes two positions. Returns TRUE if the two positions are adjacent to each
	other on the board (including diagonally adjacent) */
Boolean isInRange(Position p1, Position p2) {
	if (p1.x == p2.x - 1 || p1.x == p2.x || p1.x == p2.x + 1) {
		if (p1.y == p2.y - 1 || p1.y == p2.y || p1.y == p2.y + 1) {
			return TRUE;
		}
	}
	return FALSE;
}


/* 	Takes a position. If both coordinates are in range of the board
	return TRUE, else return FALSE */
Boolean board_InBounds(Position p) {
	if (p.x < 0 || p.x > BOARD_WIDTH - 1 || p.y < 0 || p.y > BOARD_HEIGHT - 1){
   		return FALSE;
   	}
   	else return TRUE;
}