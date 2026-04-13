#include "raylib.h"
#include "chessboard.h"

#ifndef DRAWING_H
#define DRAWING_H

typedef struct BoardDimensions {
	int x; // top-left corner
	int y; //
	int w; // width
	int h; // height
} BoardDimensions;

void draw_board_2D(BoardDimensions bd, Chessboard *board);
void highlight_square(Square square, BoardDimensions bd, Color color);

#endif
