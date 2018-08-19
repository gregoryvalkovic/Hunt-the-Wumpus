/*
 * COSC1076 Assignment 1, Semester 2 2018
 * Student name  : Gregory Valkovic
 * Student number: 3689338
 */
#include "game.h"
#define INPUT_SIZE 20
#define INPUT_PROMPT "Please enter your choice: "

/* Function prototypes */
void printFirstMenu();
Status loadBoardLoop(Board board);
Status initPlayerLoop(Board board, Player *player);
Status moveShootLoop(Board board, Player *player);
Status processBoardToken(Board board, char *str);

ArrowHit processShoot(Board board, Player *player, Position arrowTarget);
PlayerMove processMove(Board board, Player *player, Direction direction);

void batReposition(Board board, Player *player);
Direction convertToDirection(char *command);

Boolean isNull(char *str);
Boolean isDirection(char *command);
Boolean isQuit(char *command);
Boolean isShoot(char *command);
Boolean isLoad(char *str);
Boolean isInit(char *str);
Boolean hasArrows(Player *player);
Boolean inBounds(int x, int y);


void game_PlayGame(){
	Board board;
	Player player;
	
	srand(0);
	printFirstMenu();
	if (loadBoardLoop(board) == STATUS_QUIT) {
		return;
	}
	if (initPlayerLoop(board, &player) == STATUS_QUIT) {
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
	
	/* Loop that waits for player to press enter */
	while(input != '\r' && input != '\n') {
		/*getchar();	 Clears newline chars from previous input */
		input = getchar();
	}
}


Status loadBoardLoop(Board board) {
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
				return STATUS_QUIT;
			}
			/* Loads board if input is valid */
			if (isLoad(command) && !isNull(boardToken = strtok(NULL, " "))) {
				if (processBoardToken(board, boardToken) == STATUS_CONTINUE) {
					printf("Board successfully loaded\n\n");	
					return STATUS_CONTINUE;
				}
			}
		}
		printInvalidInput();
		continue;
	}
}


Status initPlayerLoop(Board board, Player *player) {
	char *command, *xYPair, *xToken, *yToken;
	long int xValue, yValue;
	char input[INPUT_SIZE];
	int inputResult;
	struct position playerPosition;	

	while(TRUE) {
		/* Prompt for input and read into inputResult */
		printf("At this stage of the program only two commands are acceptable:"
		"\n%s <x>,<y>\n%s\n\n", COMMAND_INIT, COMMAND_QUIT);
		inputResult = getInput(INPUT_PROMPT, input, INPUT_SIZE);
		
		/* Process first input token */
		if (inputResult == ReadInputResultSuccess) {
			if (isNull(command = strtok(input, " "))) {
				printInvalidInput();
				continue;
			}
			if (isQuit(command)) {
				return STATUS_QUIT;
			}
			/* Grab x and y token, reloop if either are null */
			if (isNull(xYPair = strtok(NULL, " "))) {
				printInvalidInput();
				continue;
			}
			xToken = strtok(xYPair, ",");
			yToken = strtok(NULL, ",");
			if (isNull(xToken) || isNull(yToken)) {
				printInvalidInput();
				continue;
			}
			/* Places the player if valid position is given */
			if (isInit(command)) {	
				xValue = strtol(xToken, NULL, 10);
				yValue = strtol(yToken, NULL, 10);
				
				if (inBounds(xValue, yValue)) {
					playerPosition.x = xValue;
					playerPosition.y = yValue;
					if (board_PlacePlayer(board, playerPosition)) {
						player_Initialise(player, playerPosition);
						return STATUS_CONTINUE;
					}
					else {
						printf("Unable to place player at that position\n\n");
						continue;
					}
				}
			}
		}
		printInvalidInput();
		continue;
	}
}


Boolean inBounds(int x, int y) {
	if (x >= 0 && x <= BOARD_WIDTH - 1 && y >= 0 && y <= BOARD_HEIGHT - 1) {
		return TRUE;
	}
	return FALSE;	
}

Status moveShootLoop(Board board, Player *player) {
	char prompt[] = "Please enter your choice: ";
	int inputResult;
	char input[INPUT_SIZE];
	char *commandToken, *directionToken;
	Position arrowTarget;
	PlayerMove moveResult;
	Direction direction;
	ArrowHit arrowHit;
	
	while(TRUE) {
		board_Display(board);
		board_DisplayWarnings(board, player->position);
		printf("At this stage of the program only three commands are "
			"acceptable:\n <direction>\n shoot <direction>\n quit\n"
			"Where <direction is one of: north,n,south,s,east,e,west,w\n\n");
		
		/* Read input and quit if appropriate */
		inputResult = getInput(prompt, input, INPUT_SIZE);
		if (inputResult == ReadInputResultSuccess) {
			commandToken = strtok(input, " ");
			if (isNull(commandToken)) {
				printInvalidInput();
				continue;
			}
			if (isQuit(commandToken)) {
				return STATUS_QUIT;
			}
			
			/* Process appropriately if given a move command */
			if (isDirection(commandToken)) {
				direction = convertToDirection(commandToken);
				moveResult = processMove(board, player, direction);
				if (moveResult == board_PLAYER_KILLED) {
					return STATUS_QUIT;
				}
				else {
					continue;
				}
			}
			/* Check if shoot command and that player has enough arrows */
			if (isShoot(commandToken)) {
				directionToken = strtok(NULL,  " ");
				if (isDirection(directionToken)) {
					if (hasArrows(player)) {
						direction = convertToDirection(directionToken);
						arrowTarget = player_GetNextPosition(player->position,
						direction);
						
						arrowHit = processShoot(board, player, arrowTarget);
						if (arrowHit == board_WUMPUS_KILLED) {
							return STATUS_QUIT;
						}
						continue;
					}
					/* If player has no arrows */
					else {
						printf("You don't have any arrows to fire\n");	
						continue;
					}
				}
				/* If the second token input isn't a direction */
				else {
					printInvalidInput();
					continue;
				}
			}
		}
		else {
			printInvalidInput();
			continue;
		}
	}
}


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


Direction convertToDirection(char *command) {
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


Position genRandomPosition() {
	Position position;
	position.x = rand() % BOARD_WIDTH;
	position.y = rand() % BOARD_HEIGHT;
	return position;
}


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


/* Procceses the output of board_MovePlayer */
PlayerMove processMove(Board board, Player *player, Direction direction) {
	Position newPosition = player_GetNextPosition(player->position, direction);
	PlayerMove moveResult = board_MovePlayer(board, player->position,
		newPosition);
	
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
		board[player->position.y][player->position.x] = board_TRAVERSED;
		board[newPosition.y][newPosition.x] = board_PLAYER;
		player_UpdatePosition(player, newPosition);
	}
	return moveResult;
}


ArrowHit processShoot(Board board, Player *player, Position arrowTarget) {
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


Status processBoardToken(Board board, char *str) {
	Status status;
	
	if (strcmp(str, "1") == 0) {
		board_Load(board, BOARD_1);
		status = STATUS_CONTINUE;
	}
	else if (strcmp(str, "2") == 0) {
		board_Load(board, BOARD_2);
		status = STATUS_CONTINUE;
	}
	else {
		status = STATUS_QUIT;	
	}
	return status;
}


Boolean isQuit(char *command) {
	Boolean isQuit;
	if(strcmp(command, COMMAND_QUIT) == 0) {
		isQuit = TRUE;
	}
	else {
		isQuit = FALSE;	
	}
	return isQuit;
}


Boolean isShoot(char *command) {
	Boolean isShoot;
	if (strcmp(command, COMMAND_SHOOT) == 0) {
		isShoot = TRUE;
	}
	else {
		isShoot = FALSE;	
	}
	return isShoot;
}


Boolean hasArrows(Player *player) {
	Boolean hasArrows;
	if (player->numArrows > 0) {
		hasArrows = TRUE;
	}
	else {
		hasArrows = FALSE;
	}
	return hasArrows;
}


Boolean isNull(char *str) {
	Boolean isNull;
	if (str == NULL) {
		isNull = TRUE;
	}
	else {
		isNull = FALSE;	
	}
	return isNull;
}


Boolean isLoad(char *str) {
	Boolean isLoad;
	if (strcmp(str, COMMAND_LOAD) == 0) {
		isLoad = TRUE;
	}
	else {
		isLoad = FALSE;	
	}
	return isLoad;
}


Boolean isInit(char *str) {
	Boolean isInit;
	if (strcmp(str, COMMAND_INIT) == 0) {
		isInit = TRUE;
	}
	else {
		isInit = FALSE;	
	}
	return isInit;
}