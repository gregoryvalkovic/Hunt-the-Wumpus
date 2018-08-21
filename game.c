/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#include "game.h"
#define INPUT_SIZE 20
#define INPUT_PROMPT "Please enter your choice: "

/* -------------------------------------------------------------------------- */
                           /* Function Prototypes */
/* -------------------------------------------------------------------------- */
void printFirstMenu();
void printMoveShootPrompt();
void printInitPlayerPrompt();

Boolean loadBoardLoop(Board board);
Boolean processBoardToken(Board board, char *str);

Boolean initPlayerLoop(Board board, Player *player);
Boolean processInit(Board board, Player *player, char *command);

Boolean moveShootLoop(Board board, Player *player);
ArrowHit processShoot(Board board, Player *player, char *command);
PlayerMove processMoveResult(Board board, Player *player,
							 PlayerMove moveResult);
PlayerMove processMoveCommand (Board board, Player *player, char *command);

void batReposition(Board board, Player *player);
Direction createDirection(char *command);

Boolean isNull(char *str);
Boolean isDirection(char *command);
Boolean isQuit(char *command);
Boolean isShoot(char *command);
Boolean isLoad(char *str);
Boolean isInit(char *str);
Boolean hasArrows(Player *player);

/* -------------------------------------------------------------------------- */
							/* Main game loop */
/* -------------------------------------------------------------------------- */
							
void game_PlayGame(){
	Board board;
	Player player;
	
	srand(0);
	printFirstMenu();
	if (loadBoardLoop(board) == FALSE) {
		return;
	}
	if (initPlayerLoop(board, &player) == FALSE) {
		return;	
	}
	moveShootLoop(board, &player);
}


void printFirstMenu() {
	char input = 0;
	
	printf("\nYou can use the following commands to play the game:\n\n");
	printf("%s <g>\n g: name of the game board to load (either 1 or 2)\n",
		COMMAND_LOAD);
	printf("%s <x>, <y>\n", COMMAND_INIT);
	printf(" x: horizontal position of the player on the board (between 0 & 4)"
	"\n y: vertical position of the player on the board (between 0 & 4)\n");
	printf("%s (or %s)\n%s (or %s)\n%s (or %s)\n%s (or %s)\n",
		COMMAND_NORTH,
		COMMAND_NORTH_SHORTCUT,
		COMMAND_SOUTH,
		COMMAND_SOUTH_SHORTCUT,
		COMMAND_EAST,
		COMMAND_EAST_SHORTCUT,
		COMMAND_WEST,
		COMMAND_WEST_SHORTCUT);
	printf("%s\n\n", COMMAND_QUIT);
	printf("Press enter to continue...\n");
	
/* 	Loop that waits for player to press enter. Adapted from stackoverflow
	https://stackoverflow.com/questions/1406421/press-enter-to-continue-in-c */
	while(input != '\r' && input != '\n') {
		input = getchar();
	}
}
/* -------------------------------------------------------------------------- */
							/* Load Board Loop */
/* -------------------------------------------------------------------------- */
							

Boolean loadBoardLoop(Board board) {
	char input[INPUT_SIZE];
	int inputResult;
	char *command, *boardToken;
	
	while (TRUE) {
		printf("At this stage of the program only two commands are acceptable:"
				"\n%s <g>\n%s\n\n", COMMAND_LOAD, COMMAND_QUIT);
		inputResult = getInput(INPUT_PROMPT, input, INPUT_SIZE);
		
		/* Loops if input is NULL */
		if (inputResult == ReadInputResultSuccess) {
			if (isNull(command = strtok(input, " "))) {
				printInvalidInput();
				continue;
			}
			if (isQuit(command)) {
				return FALSE;
			}
			/* Loads board if input is valid */
			if (isLoad(command) && !isNull(boardToken = strtok(NULL, " "))) {
				if (processBoardToken(board, boardToken) == TRUE) {
					printf("Board successfully loaded\n\n");	
					return TRUE;
				}
			}
		}
		printInvalidInput();
	}
}


/* 	Takes board and a string indicating which board to load. Proceeds to load
	the board returning TRUE if successful and FALSE if not. Includes input
	validation */
Boolean processBoardToken(Board board, char *str) {
	if (strcmp(str, "1") == 0) {
		board_Load(board, BOARD_1);
		return TRUE;
	}
	else if (strcmp(str, "2") == 0) {
		board_Load(board, BOARD_2);
		return TRUE;
	}
	return FALSE;
}
/* -------------------------------------------------------------------------- */
							/* Init Player Loop */
/* -------------------------------------------------------------------------- */
							
Boolean initPlayerLoop(Board board, Player *player) {
	char *command;
	char input[INPUT_SIZE];
	int inputResult;

	while(TRUE) {
		printInitPlayerPrompt();
		inputResult = getInput(INPUT_PROMPT, input, INPUT_SIZE);
		
		/* Process first input token */
		if (inputResult == ReadInputResultSuccess) {
			if (isNull(command = strtok(input, " "))) {
				printInvalidInput();
				continue;
			}
			if (isQuit(command)) {
				return FALSE;
			}
			if (processInit(board, player, command) == TRUE) {
				return TRUE;
			}
			continue;
		}
		printInvalidInput();
	}
}


void printInitPlayerPrompt() {
	printf("At this stage of the program only two commands are acceptable:"
			"\n%s <x>,<y>\n%s\n\n", COMMAND_INIT, COMMAND_QUIT);
}


/* Takes a command and processes it appropriately if it is 'init' */
Boolean processInit(Board board, Player *player, char *command) {
	char *xYPair, *xToken, *yToken;
	long int xValue, yValue;
	Position playerPosition;
	
	if (isInit(command)) {
		/* Grab x and y token, reloop if either are null */
		if (isNull(xYPair = strtok(NULL, " "))) {
			printInvalidInput();
			return FALSE;
		}
		xToken = strtok(xYPair, ",");
		yToken = strtok(NULL, ",");
		if (isNull(xToken) || isNull(yToken)) {
			printInvalidInput();
			return FALSE;
		}
		/* Places the player if valid position is given */	
		xValue = strtol(xToken, NULL, 10);
		yValue = strtol(yToken, NULL, 10);
		playerPosition.x = xValue;
		playerPosition.y = yValue;
		/* inBounds function found in helpers.c */
		if (board_InBounds(playerPosition)) {
			if (board_PlacePlayer(board, playerPosition)) {
				player_Initialise(player, playerPosition);
				return TRUE;
			}
			printf("Unable to place player at that position\n\n");
			return FALSE;
		}
	}
	printInvalidInput();
	return FALSE;
}
/* -------------------------------------------------------------------------- */
							/* Move Shoot Loop */
/* -------------------------------------------------------------------------- */

void printMoveShootPrompt() {
	printf("At this stage of the program only three commands are "
			"acceptable:\n <direction>\n shoot <direction>\n quit\n"
			"Where <direction is one of: north,n,south,s,east,e,west,w\n\n");
}


Boolean moveShootLoop(Board board, Player *player) {
	int inputResult;
	char input[INPUT_SIZE];
	char *command, *directionToken;
	PlayerMove moveResult;
	ArrowHit arrowHit;
	
	while(TRUE) {
		board_Display(board);
		board_DisplayWarnings(board, player->position);
		printMoveShootPrompt();
		
		/* Read first input token and quit if appropriate */
		inputResult = getInput(INPUT_PROMPT, input, INPUT_SIZE);
		if (inputResult == ReadInputResultSuccess) {
			command = strtok(input, " ");
			if (isNull(command)) {
				printInvalidInput();
				continue;
			}
			if (isQuit(command)) return FALSE;
			
			/* Process a move command */
			if (isDirection(command)) {
				moveResult = processMoveCommand(board, player, command);
				if (moveResult == board_PLAYER_KILLED) {
					return FALSE;
				}
				else continue;
			}
			/* 	Process shoot command if player has enough arrows and valid 
				input is given */
			if (isShoot(command)) {
				directionToken = strtok(NULL, " ");
				if (isDirection(directionToken)) {
					if (hasArrows(player)) {
						arrowHit = processShoot(board, player, directionToken);
						if (arrowHit == board_WUMPUS_KILLED) {
							return FALSE;	
						}
						else continue;
					}
					/* If player has no arrows */
					else {
						printf("You don't have any arrows to fire\n");	
						continue;
					}
				}
			}
		}
		printInvalidInput();
	}
}


/* 	Takes a move command str and attempts to move the player. The resulting 
	PlayerMove value is then processed appropriately */
PlayerMove processMoveCommand (Board board, Player *player, char *command) {
	Direction direction = createDirection(command);
	Position nextPosition = player_GetNextPosition(player->position, direction);
	PlayerMove moveResult = board_MovePlayer(board, player->position,
											 nextPosition);
	
	if (moveResult == board_OUTSIDE_BOUNDS) {
		printf("Unable to move - outside bounds\n\n");
	}
	if (moveResult == board_PLAYER_KILLED) {
		printf("Player killed!");
	}
	if (moveResult == board_BAT_CELL) {
		batReposition(board, player);
	}
	if (moveResult == board_PLAYER_MOVED) {
		player->position = nextPosition;
	}
	return moveResult;
}


/* 	Takes board, player and a string indicating a direction to shoot. Calls 
	board_FireArrow() and processes the resulting ArrowHit appropriately
	Finally returns the ArrowHit value. */

ArrowHit processShoot(Board board, Player *player, char *directionCommand) {
	Direction direction = createDirection(directionCommand);
	Position arrowTarget = player_GetNextPosition(player->position, direction);
	ArrowHit arrowHit = board_FireArrow(board, arrowTarget);

	if (arrowHit == board_ARROW_OUTSIDE_BOUNDS) {
		printf("Unable to fire arrow in that direction\n");
	}
	if (arrowHit == board_WUMPUS_KILLED) {
		printf("You killed the Wumpus!\n");
	}
	if (arrowHit == board_ARROW_MISSED) {
		player->numArrows--;
		printf("Missed. You now have %d arrows\n", player->numArrows);
	}
	return arrowHit;
}


/* Returns a random position that is in bounds */
Position genRandomPosition() {
	Position position;
	position.x = rand() % BOARD_WIDTH;
	position.y = rand() % BOARD_HEIGHT;
	return position;
}


/* 	Moves the player to a random position on the board. It can only move players
	to a cell with board_EMPTY or board_TRAVERSED */
void batReposition(Board board, Player *player) {
	Position randomPosition;
	
	while(TRUE) {
		randomPosition = genRandomPosition();
		if (board[randomPosition.y][randomPosition.x] == board_EMPTY ||
			board[randomPosition.y][randomPosition.x] == board_TRAVERSED)
		{
			board[randomPosition.y][randomPosition.x] = board_PLAYER;
			board[player->position.y][player->position.x] = board_TRAVERSED;
			player_UpdatePosition(player, randomPosition);
			return;
		}
	}
}


/* 	Takes a string that indicates a direction and returns the corresponding
	Direction value. Does not validate that input is a direction,this must be 
	done before calling this function */
Direction createDirection(char *command) {
	Direction direction;
	
	if (strcmp(command, "n") == 0 || strcmp(command, "north") == 0) {
		direction = player_NORTH;
	}
	if(strcmp(command, "s") == 0 || strcmp(command, "south") == 0) {
		direction = player_SOUTH;
	}
	if(strcmp(command, "w") == 0 || strcmp(command, "west") == 0) {
		direction = player_WEST;
	}
	if(strcmp(command, "e") == 0 || strcmp(command, "east") == 0) {
		direction = player_EAST;
	}
	return direction;
}

/* -------------------------------------------------------------------------- */
						/* General Functions */
/* -------------------------------------------------------------------------- */

/* 	Takes a string and returns TRUE if the string is a valid direction.
	Otherwise returns FALSE */
Boolean isDirection(char *command) {
	Boolean isDirection;
	
	if (isNull(command)) {
		isDirection = FALSE;
	}
	else if (strcmp(command, "n") == 0 || strcmp(command, "north") == 0 ||
		strcmp(command, "s") == 0 || strcmp(command, "south") == 0 ||
		strcmp(command, "w") == 0 || strcmp(command, "west") == 0 ||
		strcmp(command, "e") == 0 || strcmp(command, "east") == 0) 
	{	
		isDirection = TRUE;
	}
	else {
		isDirection = FALSE;
	}
	return isDirection;
}


Boolean hasArrows(Player *player) {
	if (player->numArrows > 0) return TRUE;
	else return FALSE;
}

/* 	The following functions generally serve the purpose of checking if a string 
	matches a certain command, returning TRUE if it does and FALSE if it doesn't
*/

Boolean isQuit(char *command) {
	if(strcmp(command, COMMAND_QUIT) == 0) return TRUE;
	else return FALSE;
}


Boolean isShoot(char *command) {
	if (strcmp(command, COMMAND_SHOOT) == 0) return TRUE;
	else return FALSE;
}

Boolean isNull(char *str) {
	if (str == NULL) return TRUE;
	else return FALSE;
}


Boolean isLoad(char *str) {
	if (strcmp(str, COMMAND_LOAD) == 0) return TRUE;
	else return FALSE;
}

Boolean isInit(char *str) {
	if (strcmp(str, COMMAND_INIT) == 0) return TRUE;
	else return FALSE;
}