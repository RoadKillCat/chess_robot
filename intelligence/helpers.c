#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "common.h"
#include "helpers.h"

static char piece_chars[7] = {' ','p','r','n','b','q','k'};

void print_state(state_t *state){
    //white pieces are uppper case, black pieces lower case
    printf("------------------\n");
    for (int8_t i = 7; i > -1; i--){
        printf("%d|", i);
        for (int8_t j = 0; j < 8; j++){
            if (state->pieces[i][j] > 0){
                printf("%c|", UPPER(piece_chars[state->pieces[i][j]]));
            } else {
                printf("%c|", piece_chars[-state->pieces[i][j]]);
            }
        }
        printf("\n------------------\n");
    }
    printf(" |0|1|2|3|4|5|6|7|\n");
}


void print_move(move_t *move){
    printf("move [%d,%d] => [%d,%d]\n", move->from[0], move->from[1], move->to[0], move->to[1]);
}

void make_move(state_t *state, move_t *move){
    //does not insure move is legal!
    int8_t side = state->pieces[MOVE_FROM] > 0 ? WHITE : BLACK;
    state->last_taken = state->pieces[MOVE_TO];
    if (move->castle){
        //special case: castling (note: move->from must target the right kind for side)
        //move king...
        state->pieces[side==WHITE?0:7][move->castle==KINGSIDE?6:2] = side*KING;
        state->pieces[side==WHITE?0:7][4] = 0;
        //move castle...
        state->pieces[side==WHITE?0:7][move->castle==KINGSIDE?5:3] = side*ROOK;
        state->pieces[side==WHITE?0:7][move->castle==KINGSIDE?7:0] = 0;
    } else if (move->is_pawn_promotion){
        //special case: pawn promotion (auto to queen :)
        state->pieces[MOVE_TO] = side*QUEEN;
        state->pieces[MOVE_FROM] = 0;
    } else {
        //normal move...
        state->pieces[MOVE_TO] = state->pieces[MOVE_FROM];
        state->pieces[MOVE_FROM] = 0;
    }
}

void inverse_move(state_t *state, move_t *move){
    //does the move from to->from
    int8_t side = state->pieces[MOVE_TO] > 0 ? WHITE : BLACK;
    if (move->castle){
        //special case: castling
        //move king...
        state->pieces[side==WHITE?0:7][move->castle==KINGSIDE?6:2] = 0;
        state->pieces[side==WHITE?0:7][4] = side*KING;
        //move castle...
        state->pieces[side==WHITE?0:7][move->castle==KINGSIDE?5:3] = 0;
        state->pieces[side==WHITE?0:7][move->castle==KINGSIDE?7:0] = side*ROOK;
    } else if (move->is_pawn_promotion){
        //special case: pawn promotion (auto to queen :)
        state->pieces[MOVE_TO] = state->last_taken;
        state->pieces[MOVE_FROM] = side*PAWN;
    } else {
        //normal move...
        state->pieces[MOVE_FROM] = state->pieces[MOVE_TO];
        state->pieces[MOVE_TO] = state->last_taken;
    }

}