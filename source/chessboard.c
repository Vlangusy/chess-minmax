#include <stdlib.h>
#include <stdio.h>

#include "chessboard.h"

Chessboard init_chessboard() {
	Chessboard new_chessboard;
	
	for(int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			new_chessboard.blacks[x][y] = EMPTY;
			new_chessboard.whites[x][y] = EMPTY;
		}
	}
	
	for(int x = 0; x < 8; x++) {
		new_chessboard.blacks[x][1] = PAWN;
		new_chessboard.whites[x][6] = PAWN;
	}

	new_chessboard.blacks[0][0] = ROOK;
	new_chessboard.blacks[1][0] = KNIGHT;
	new_chessboard.blacks[2][0] = BISHOP;
	new_chessboard.blacks[3][0] = KING;
	new_chessboard.blacks[4][0] = QUEEN;
	new_chessboard.blacks[5][0] = BISHOP;
	new_chessboard.blacks[6][0] = KNIGHT;
	new_chessboard.blacks[7][0] = ROOK;

	new_chessboard.whites[0][7] = ROOK;
	new_chessboard.whites[1][7] = KNIGHT;
	new_chessboard.whites[2][7] = BISHOP;
	new_chessboard.whites[3][7] = KING;
	new_chessboard.whites[4][7] = QUEEN;
	new_chessboard.whites[5][7] = BISHOP;
	new_chessboard.whites[6][7] = KNIGHT;
	new_chessboard.whites[7][7] = ROOK;
	
	return new_chessboard;
}

int is_valid_move(Chessboard *board, Move move) {
	MoveNode *valid_moves = generate_valid_moves_at_square(board, move.from);
	MoveNode *current_node = valid_moves;
	
	int is_valid = 0;	
	while(current_node != 0) {
		if(move.to.x == current_node->move.to.x && move.to.y == current_node->move.to.y) {
			is_valid = 1;
			break;
		}
		current_node = current_node->next;
	}
	
	free_moves(valid_moves);
	return is_valid;
}

int is_valid_square(Square square) {
	return square.x >= 0 && square.x < 8 && square.y >= 0 && square.y < 8;
}

int perform_move(Chessboard *board, Move move) {
	int game_continues = 1;
	int is_black = 0;
	if(board->blacks[move.from.x][move.from.y] != EMPTY)
		is_black = 1;
	
	if(is_black) {
		if(board->whites[move.to.x][move.to.y] == KING)
			game_continues = 0;
		
		if(board->blacks[move.from.x][move.from.y] == PAWN && move.to.y == 7)
			board->blacks[move.to.x][move.to.y] = QUEEN; // promotion: forced queening for simplicity
		else
			board->blacks[move.to.x][move.to.y] = board->blacks[move.from.x][move.from.y];
		
		board->blacks[move.from.x][move.from.y] = EMPTY;
		board->whites[move.to.x][move.to.y] = EMPTY;
	}
	else {
		if(board->blacks[move.to.x][move.to.y] == KING)
			game_continues = 0;
		
		if(board->whites[move.from.x][move.from.y] == PAWN && move.to.y == 0)
			board->whites[move.to.x][move.to.y] = QUEEN;
		else
			board->whites[move.to.x][move.to.y] = board->whites[move.from.x][move.from.y];
		
		board->whites[move.from.x][move.from.y] = EMPTY;
		board->blacks[move.to.x][move.to.y] = EMPTY;
	}
	
	return game_continues;
}

int square_has_enemy(Chessboard *board, Square square, int is_black_player) {
	if(is_black_player && board->whites[square.x][square.y] != EMPTY)
		return 1;
	if(!is_black_player && board->blacks[square.x][square.y] != EMPTY)
		return 1;	
	return 0;
}

int square_has_my_piece(Chessboard *board, Square square, int is_black_player) {
	if(is_black_player && board->blacks[square.x][square.y] != EMPTY)
		return 1;
	if(!is_black_player && board->whites[square.x][square.y] != EMPTY)
		return 1;	
	return 0;
}

int square_has_any_piece(Chessboard *board, Square square) {
	if(board->blacks[square.x][square.y] != EMPTY || board->whites[square.x][square.y] != EMPTY)
		return 1;
	return 0;
}

void free_moves(MoveNode *node) {
	MoveNode *next_node;
	
	while(node) {
		next_node = node->next;
		free(node);
		node = next_node;
	}
}

void push_move(MoveNode **node, Move move) {
	if(*node == 0) {
		*node = malloc(sizeof(MoveNode));
		(*node)->next = 0;
		(*node)->move = move;
	}
	else {
		MoveNode *new_node = malloc(sizeof(MoveNode));
		new_node->next = (*node);
		new_node->move = move;
		*node = new_node;
	}
}

void push_valid_moves_raycast(MoveNode **node, Chessboard *board, Square square, int direction_x, int direction_y) {
	int is_black = 0;
	if(board->blacks[square.x][square.y] != EMPTY)
		is_black = 1;
	
	// normalise move vector
	direction_x = (direction_x > 0) - (direction_x < 0);
	direction_y = (direction_y > 0) - (direction_y < 0);
	
	Move move = {square, square};
	square.x += direction_x;
	square.y += direction_y;
	
	while(is_valid_square(square)) {
		if(square_has_my_piece(board, square, is_black))
			break;
		
		move.to = square;
		push_move(node, move);
		
		if(square_has_enemy(board, square, is_black))
			break;
		
		square.x += direction_x;
		square.y += direction_y;
	}
}

MoveNode *generate_valid_moves_at_square(Chessboard *board, Square square) {
	int is_black = 0;
	char piece;
	
	if(board->blacks[square.x][square.y] != EMPTY) {
		is_black = 1;
		piece = board->blacks[square.x][square.y];
	}
	else
		piece = board->whites[square.x][square.y];
	
	MoveNode *first_node = 0;
	Move move = {square, square};
	
	int direction = 1;
	if(!is_black)
		direction = -1;
	
	switch(piece) {
		case PAWN:
			// forward
			move.to.y += direction;
			if(is_valid_square(move.to) && !square_has_any_piece(board, move.to)) {
				push_move(&first_node, move);
				if(is_black && square.y == 1 || !is_black && square.y == 6) {
					move.to.y += direction;
					if(!square_has_any_piece(board, move.to))
						push_move(&first_node, move);
				}
			}
			// capture
			move.to = square;
			move.to.x -= 1; move.to.y += direction;
			if(is_valid_square(move.to) && square_has_enemy(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.x += 2;
			if(is_valid_square(move.to) && square_has_enemy(board, move.to, is_black))
				push_move(&first_node, move);
			
			break;
		case KNIGHT:
			move.to.x += 1; move.to.y -= 2;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.x += 1; move.to.y += 1;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.y += 2;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.x -= 1; move.to.y += 1;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.x -= 2;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.x -= 1; move.to.y -= 1;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.y -= 2;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			move.to.x += 1; move.to.y -= 1;
			if(is_valid_square(move.to) && !square_has_my_piece(board, move.to, is_black))
				push_move(&first_node, move);
			
			break;
		case ROOK:
			push_valid_moves_raycast(&first_node, board, square, 0, -1);
			push_valid_moves_raycast(&first_node, board, square, 1, 0);
			push_valid_moves_raycast(&first_node, board, square, 0, 1);
			push_valid_moves_raycast(&first_node, board, square, -1, 0);
			
			break;
		case BISHOP:
			push_valid_moves_raycast(&first_node, board, square, 1, -1);
			push_valid_moves_raycast(&first_node, board, square, 1, 1);
			push_valid_moves_raycast(&first_node, board, square, -1, 1);
			push_valid_moves_raycast(&first_node, board, square, -1, -1);
			
			break;
		case KING:
			for(int x = -1; x <= 1; x++)
				for(int y = -1; y <= 1; y++) {
					if(x == 0 && y == 0)
						continue;
					Square square_to_check = square;
					square_to_check.x += x;
					square_to_check.y += y;
					if(is_valid_square(square_to_check) && !square_has_my_piece(board, square_to_check, is_black)) {
						move.to = square_to_check;
						push_move(&first_node, move);
					}
				}
			
			break;
		case QUEEN:
			push_valid_moves_raycast(&first_node, board, square, 0, -1);
			push_valid_moves_raycast(&first_node, board, square, 1, 0);
			push_valid_moves_raycast(&first_node, board, square, 0, 1);
			push_valid_moves_raycast(&first_node, board, square, -1, 0);
			push_valid_moves_raycast(&first_node, board, square, 1, -1);
			push_valid_moves_raycast(&first_node, board, square, 1, 1);
			push_valid_moves_raycast(&first_node, board, square, -1, 1);
			push_valid_moves_raycast(&first_node, board, square, -1, -1);
			
			break;
	}
	
	return first_node;
}
