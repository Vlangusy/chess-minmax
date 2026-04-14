#include <stdio.h>

#include "raylib.h"
#include "chessboard.h"
#include "minmax.h"
#include "drawing.h"
#include "input.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define PLAYER_BLACK 'B'
#define PLAYER_WHITE 'W'
#define PLAYER_BLACK_CPU 1
#define PLAYER_WHITE_CPU 0

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "chess");
	SetTargetFPS(30);
	
	Chessboard current_board = init_chessboard();
	BoardDimensions bd = {40, 40, 720, 720};
	Square from_square = NULL_SQUARE;
	Square to_square = NULL_SQUARE;
	char current_player = PLAYER_WHITE;
	int was_last_action_valid = 1;
	int game_finished = 0;
	
    while(!WindowShouldClose()) {
        // INPUT
		if(IsMouseButtonReleased(0) && !game_finished) {
			if(from_square.x == NULL_SQUARE.x)		// new move
				from_square = mouse_position_to_square(bd);
			else {
				to_square = mouse_position_to_square(bd);
				if(to_square.x == NULL_SQUARE.x) {	// clicked outside, cancel move
					from_square = NULL_SQUARE;
					was_last_action_valid = 1;
				}
				else if (to_square.x == from_square.x && to_square.y == from_square.y) {
													// double click, cancel move
					from_square = NULL_SQUARE;
					to_square = NULL_SQUARE;
					was_last_action_valid = 1;
				}
			}
		}
		
		// LOGIC
		if(current_player == PLAYER_BLACK && PLAYER_BLACK_CPU
		|| current_player == PLAYER_WHITE && PLAYER_WHITE_CPU) {	// cpu actions
			if(perform_move(&current_board, minmax_driver(&current_board, 0, current_player == PLAYER_BLACK))) {
				if(current_player == PLAYER_WHITE)
					current_player = PLAYER_BLACK;
				else
					current_player = PLAYER_WHITE;
			}
			else
				game_finished = 1;
		}

		if(from_square.x != NULL_SQUARE.x) {		// check if the current player's piece has been clicked
			if(current_player == PLAYER_WHITE && current_board.whites[from_square.x][from_square.y] == EMPTY)
				from_square = NULL_SQUARE;
			if(current_player == PLAYER_BLACK && current_board.blacks[from_square.x][from_square.y] == EMPTY)
				from_square = NULL_SQUARE;
		}
		if(to_square.x != NULL_SQUARE.x) {			// a complete move has been declared
			Move move = {from_square, to_square};
			was_last_action_valid = is_valid_move(&current_board, move);
			if(was_last_action_valid) {
				if(perform_move(&current_board, move)) {
					if(current_player == PLAYER_WHITE)
						current_player = PLAYER_BLACK;
					else
						current_player = PLAYER_WHITE;
				}
				else
					game_finished = 1;
			}
			from_square = to_square = NULL_SQUARE;
		}
		
		// DRAWING
		BeginDrawing();
		ClearBackground(BLACK);
		
		DrawText(TextFormat("%c", current_player), 0, 0, 40, GREEN);
		draw_board_2D(bd, &current_board);
		if(from_square.x != NULL_SQUARE.x)
			highlight_square(from_square, bd, RED);
		if(!was_last_action_valid)
			DrawText("action invalid", 400, 0, 40, RED);
		if(game_finished)
			DrawText("we've got a winner!", 400, 0, 40, GREEN);
		
		EndDrawing();
    }
	
    CloseWindow();
    return 0;
}
