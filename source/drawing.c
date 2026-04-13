#include "raylib.h"
#include "chessboard.h"
#include "drawing.h"

void draw_board_2D(BoardDimensions bd, Chessboard *board) {
	Color color_A = BLUE;
	Color color_B = DARKGRAY;
	Color color_blacks = BLACK;
	Color color_whites = WHITE;
	
	int square_width = bd.w / 8;
	int square_height = bd.h / 8;
	
	int square_id = 0;
	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 8; y++){
			Color current_color;
			if (square_id++ % 2)
				current_color = color_A;
			else
				current_color = color_B;
			DrawRectangle(x*square_width+bd.x, y*square_height+bd.y, square_width, square_height, current_color);
			
			int text_size = square_width > square_height ? square_height : square_width;
			if (board->blacks[x][y] != EMPTY)
				DrawText(TextFormat("%c", board->blacks[x][y]), x*square_width+bd.x, y*square_height+bd.y, text_size, color_blacks);
			else if (board->whites[x][y] != EMPTY)
				DrawText(TextFormat("%c", board->whites[x][y]), x*square_width+bd.x, y*square_height+bd.y, text_size, color_whites);
		}
		square_id--;
	}
}

void highlight_square(Square square, BoardDimensions bd, Color color) {
	int square_width = bd.w / 8;
	int square_height = bd.h / 8;
	int x = bd.x + (square.x * square_width);
	int y = bd.y + (square.y * square_height);
	Rectangle r = {x, y, square_width, square_height};
	
	DrawRectangleLinesEx (r, 3.0, color);
}
