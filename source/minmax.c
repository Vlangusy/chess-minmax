#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

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
		return value;
	}	
	
	int highest_value = -9999;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			if((is_black_player && board->blacks[x][y] != EMPTY) || (!is_black_player && board->whites[x][y] != EMPTY)) {
				MoveNode *first_node = generate_valid_moves_at_square(board, (Square){x, y});
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
	Move optimal_move = (Move){NULL_SQUARE, NULL_SQUARE};
	int highest_value = -9999;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			if((is_black_player && board->blacks[x][y] != EMPTY) || (!is_black_player && board->whites[x][y] != EMPTY)) {
				MoveNode *first_node = generate_valid_moves_at_square(board, (Square){x, y});
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
	printf("choice: %d:%d -> %d:%d\n", optimal_move.from.x, optimal_move.from.y, optimal_move.to.x, optimal_move.to.y);
	printf("---\n");
	return optimal_move;
}

typedef struct minmax_args {
	Chessboard board;
	int depth;
	int is_black_player;
	int *return_value;
} minmax_args;

static void minmax_multithreaded(minmax_args *args) {	
	if(args->depth <= 0) {
		int value = evaluate_board_for_black(&args->board);
		if(!args->is_black_player)
			value *= -1;
		*(args->return_value) = value;
		return;
	}	
	
	int highest_value = -9999;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			if((args->is_black_player && args->board.blacks[x][y] != EMPTY) || (!args->is_black_player && args->board.whites[x][y] != EMPTY)) {
				MoveNode *first_node = generate_valid_moves_at_square(&args->board, (Square){x, y});
				MoveNode *current_node = first_node;
				int end = 0;
				
				while(current_node != 0) {
					Chessboard next_board = args->board;
					if(!perform_move(&next_board, current_node->move)) {
						highest_value = 9999;
						end = 1;
						break;
					}
					int next_board_value = minmax(&next_board, args->depth - 1, !args->is_black_player) * -1;
					if(next_board_value > highest_value || (next_board_value == highest_value && rand() % 4 == 0))
						highest_value = next_board_value;
					current_node = current_node->next;
				}
				
				free_moves(first_node);
				if(end) {
					*(args->return_value) = highest_value;
					return;
				}
			}
		}
	
	*(args->return_value) = highest_value;
	return;
}

Move minmax_multithreaded_driver(Chessboard *board, int depth, int is_black_player) {
	uintptr_t threads[432]; // 16 queens * 27 moves from the central squares (super-pessimistic)
	minmax_args args[432];
	Move moves[432];
	int values[432];
	int threads_number = 0;
	
	for(int x = 0; x <= 7; x++)
		for(int y = 0; y <= 7; y++) {
			if((is_black_player && board->blacks[x][y] != EMPTY) || (!is_black_player && board->whites[x][y] != EMPTY)) {
				MoveNode *first_node = generate_valid_moves_at_square(board, (Square){x, y});
				MoveNode *current_node = first_node;
				Move winning_move;
				int end = 0;
				
				while(current_node != 0) {
					Chessboard next_board = *board;
					if(!perform_move(&next_board, current_node->move)) {
						winning_move = current_node->move;
						printf("%d:%d -> %d:%d : winning move\n---\n", winning_move.from.x, winning_move.from.y, winning_move.to.x, winning_move.to.y);
						end = 1;
						break;
					}
					
					moves[threads_number] = current_node->move;
					args[threads_number].board = next_board;
					args[threads_number].depth = depth - 1;
					args[threads_number].is_black_player = !is_black_player;
					args[threads_number].return_value = &values[threads_number];
					threads[threads_number] = _beginthread(minmax_multithreaded, 0, &args[threads_number]);
					threads_number++;
					
					current_node = current_node->next;
				}
				
				free_moves(first_node);
				if(end) {
					while(threads_number) {
						threads_number--;
						//TerminateThread((HANDLE)threads[threads_number], 0); // by doing so we don't allow the threads to free their heaps
						WaitForSingleObject((HANDLE)threads[threads_number], INFINITE);
					}
					return winning_move;
				}
			}
		}
	
	int total_threads_number = threads_number;
	printf("%d threads launched\n", total_threads_number);
	while(threads_number) {
		threads_number--;
		printf("waiting for thread %d\n", threads_number);
		WaitForSingleObject((HANDLE)threads[threads_number], INFINITE);
	}
	
	int highest_value = -9999;
	int highest_value_index = 0;
	for(int i = 0; i < total_threads_number; i++) {
		if((values[i] * -1) > highest_value || ((values[i] * -1) == highest_value && rand() % 4 == 0)) {
			highest_value = values[i] * -1;
			highest_value_index = i;
		}
		printf("%d:%d -> %d:%d : %d\n", moves[i].from.x, moves[i].from.y, moves[i].to.x, moves[i].to.y, values[i] * -1);
	}
	printf("---\n");
	printf("choice: %d:%d -> %d:%d\n", moves[highest_value_index].from.x, moves[highest_value_index].from.y, moves[highest_value_index].to.x, moves[highest_value_index].to.y);
	printf("---\n");
	
	return moves[highest_value_index];
}