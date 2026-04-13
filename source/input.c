#include "raylib.h"
#include "chessboard.h"
#include "drawing.h"

Square mouse_position_to_square(BoardDimensions bd) {
	Square selected_square;
	float x = GetMouseX();
	float y = GetMouseY();
	
	if (x < bd.x || x > bd.x + bd.w || y < bd.y || y > bd.y + bd.h)
		selected_square = NULL_SQUARE;
	else {
		selected_square.x = (x - bd.x) / bd.w * 8;
		selected_square.y = (y - bd.y) / bd.h * 8;
	}
	
	return selected_square;
}
