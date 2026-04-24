#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#define EMPTY ' '
#define PAWN 'p'
#define KNIGHT 'k'
#define ROOK 'R'
#define BISHOP 'B'
#define QUEEN 'Q'
#define KING 'K'

#define NULL_SQUARE (Square){-1, -1}

typedef struct Chessboard {
	char blacks[8][8];
	char whites[8][8];
} Chessboard;

typedef struct Square {
	int x;
	int y;
} Square;

typedef struct Move {
	Square from;
	Square to;
} Move;

// stack node
struct MoveNode {
	Move move;
	struct MoveNode *next;
};
typedef struct MoveNode MoveNode;

Chessboard init_chessboard();

int perform_move(Chessboard *board, Move move);
int is_valid_move(Chessboard *board, Move move);
int is_valid_square(Square square);
int square_has_enemy(Chessboard *board, Square square, int is_black_player);
int square_has_my_piece(Chessboard *board, Square square, int is_black_player);
int square_has_any_piece(Chessboard *board, Square square);

// functions for creating a stack (lifo) of moves
void free_moves(MoveNode *node);
void push_move(MoveNode **node, Move move);
void push_valid_moves_raycast(MoveNode **node, Chessboard *board, Square square, int direction_x, int direction_y);
MoveNode *generate_valid_moves_at_square(Chessboard *board, Square square);

#endif
