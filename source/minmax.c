#include <stdio.h>
#include <stdlib.h>

#include "minmax.h"
#include "chessboard.h"

#define PAWN_VALUE 5
#define KNIGHT_VALUE 25
#define ROOK_VALUE 50
#define BISHOP_VALUE 50
#define QUEEN_VALUE 100
#define KING_VALUE 9999

int evaluate_board_for_black(Chessboard *board) {
	int board_value = 0;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			switch(board->blacks[x][y]) {
				case PAWN:
					board_value += PAWN_VALUE;
					break;
				case KNIGHT:
					board_value += KNIGHT_VALUE;
					break;
				case ROOK:
					board_value += ROOK_VALUE;
					break;
				case BISHOP:
					board_value += BISHOP_VALUE;
					break;
				case QUEEN:
					board_value += QUEEN_VALUE;
					break;
				case KING:
					board_value += KING_VALUE;
					break;
			}
			switch(board->whites[x][y]) {
				case PAWN:
					board_value -= PAWN_VALUE;
					break;
				case KNIGHT:
					board_value -= KNIGHT_VALUE;
					break;
				case ROOK:
					board_value -= ROOK_VALUE;
					break;
				case BISHOP:
					board_value -= BISHOP_VALUE;
					break;
				case QUEEN:
					board_value -= QUEEN_VALUE;
					break;
				case KING:
					board_value -= KING_VALUE;
					break;
			}
		}
	
	return board_value;
}

static int minmax(Chessboard *board, int depth, int is_black_player) {	
	if(depth <= 0) {
		int value = evaluate_board_for_black(board);
		if(!is_black_player)
			value *= -1;
		//printf("leaf: %d\n", value);
		return value;
	}	
	
	MoveNode *first_node = 0;
	int highest_value = -9999;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			if((is_black_player && board->blacks[x][y] != EMPTY) || (!is_black_player && board->whites[x][y] != EMPTY)) {
				first_node = generate_valid_moves_at_square(board, (Square){x, y});
				MoveNode *current_node = first_node;
				int end = 0;
				
				while(current_node != 0) {
					Chessboard next_board = *board;
					if(!perform_move(&next_board, current_node->move)) {
						highest_value = 9999;
						end = 1;
						break;
					}
					int next_board_value = minmax(&next_board, depth - 1, !is_black_player) * -1;
					if(next_board_value > highest_value || (next_board_value == highest_value && rand() % 4 == 0))
						highest_value = next_board_value;
					current_node = current_node->next;
				}
				
				free_moves(first_node);
				if(end)
					return highest_value;
			}
		}
	
	return highest_value;
}

Move minmax_driver(Chessboard *board, int depth, int is_black_player) {
	MoveNode *first_node = 0;
	Move optimal_move = (Move){NULL_SQUARE, NULL_SQUARE};
	int highest_value = -9999;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			if((is_black_player && board->blacks[x][y] != EMPTY) || (!is_black_player && board->whites[x][y] != EMPTY)) {
				first_node = generate_valid_moves_at_square(board, (Square){x, y});
				MoveNode *current_node = first_node;
				int end = 0;
				
				while(current_node != 0) {
					Chessboard next_board = *board;
					if(!perform_move(&next_board, current_node->move)) {
						optimal_move = current_node->move;
						printf("%d:%d -> %d:%d : winning move\n---\n", optimal_move.from.x, optimal_move.from.y, optimal_move.to.x, optimal_move.to.y);
						end = 1;
						break;
					}
					int next_board_value = minmax(&next_board, depth - 1, !is_black_player) * -1;
					printf("%d:%d -> %d:%d : %d\n", current_node->move.from.x, current_node->move.from.y, current_node->move.to.x, current_node->move.to.y, next_board_value);
					if(next_board_value > highest_value || (next_board_value == highest_value && rand() % 4 == 0)) {
						optimal_move = current_node->move;
						highest_value = next_board_value;
					}
					current_node = current_node->next;
				}
				
				free_moves(first_node);
				if(end)
					return optimal_move;
			}
		}
	
	printf("---\n");
	return optimal_move;
}
