#include "chessboard.h"

#ifndef MINMAX_H
#define MINMAX_H

int evaluate_board_for_black(Chessboard *board);
Move minmax_driver(Chessboard *board, int depth, int is_black_player);

#endif
