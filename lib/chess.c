#include <stdio.h>
#include <string.h> // for memcpy

#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h> // CAMLreturn
#include <caml/alloc.h> // caml_copy
#include <caml/signals.h> // control the runtime
#include <caml/fail.h>
#include <caml/custom.h>

#include "../smallchesslib/smallchesslib.h"

struct custom_operations move_undo_ops = {
  .identifier = "http://larrylee.tech/chess/move_undo",
  .finalize = custom_finalize_default,
  .compare = custom_compare_default,
  .compare_ext = custom_compare_ext_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

struct custom_operations game_ops = {
  .identifier = "http://larrylee.tech/chess/game",
  .finalize = custom_finalize_default,
  .compare = custom_compare_default,
  .compare_ext = custom_compare_ext_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

struct custom_operations record_ops = {
  .identifier = "http://larrylee.tech/chess/record",
  .finalize = custom_finalize_default,
  .compare = custom_compare_default,
  .compare_ext = custom_compare_ext_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

CAMLprim value boardInit () {
  CAMLparam0();
  SCL_Board board;
  SCL_boardInit (board);
  CAMLreturn (caml_copy_string (board));
}

CAMLprim value boardInit960 (value position) {
  CAMLparam1 (position);
  SCL_Board board;
  SCL_boardInit960 (board, Int_val (position));
  CAMLreturn (caml_copy_string (board));
}

CAMLprim void board_from_fen (value board, value fen) {
  CAMLparam2 (board, fen);
  uint8_t status = SCL_boardFromFEN ((char*) String_val (board), String_val (fen));
  if (status == 0) {
    caml_failwith ("Error: an error occured while trying to parse an FEN string.");
  }
  CAMLreturn0;
}

CAMLprim value get_square_num (value column, value row) {
  CAMLparam2 (column, row);
  CAMLreturn (Val_int (SCL_SQUARE (Int_val (column), Int_val (row))));
}

CAMLprim value move (value board, value from, value to, value promote) {
  CAMLparam4 (board, from, to, promote);
  CAMLlocal1 (res);
  res = caml_alloc_custom (&move_undo_ops, sizeof(SCL_MoveUndo), 0, 1);
  *((SCL_MoveUndo*) Data_custom_val (res)) =
  SCL_boardMakeMove((char*) String_val (board), Int_val (from), Int_val (to), Int_val (promote));
  CAMLreturn (res);
}

// TODO: test.
CAMLprim value is_move_legal (value board, value from, value to) {
  CAMLparam3 (board, from, to);
  CAMLreturn (Bool_val (SCL_boardMoveIsLegal ((char*) String_val (board), Int_val (from), Int_val (to))));
}

CAMLprim value get_moves (value board, value from) {
  CAMLparam2 (board, from);
  CAMLlocal2 (res, square);
  SCL_SquareSet xs = SCL_SQUARE_SET_EMPTY;
  SCL_boardGetMoves ((char*) String_val (board), Int_val (from), (uint8_t*) &xs);

  uint8_t num_squares = 0;
  value squares[64];
  for (uint8_t row = 0; row < 8; row ++) {
    for (uint8_t col = 0; col < 8; col ++) {
      if ((xs [row] >> col) % 2) {
        square = caml_alloc (2, 0);
        Store_field (square, 0, Val_int (row));
        Store_field (square, 1, Val_int (col));
        squares[num_squares] = square;
        num_squares ++;
      }
    }
  }
  res = caml_alloc (num_squares, 0);
  for (uint8_t i = 0; i < num_squares; i ++) {
    Store_field (res, i, squares[i]);
  }
  CAMLreturn (res);
}

CAMLprim value game_init (value board) {
  CAMLparam1 (board);
  CAMLlocal1 (res);
  res = caml_alloc_custom (&game_ops, sizeof (SCL_Game), 0, 1);
  SCL_gameInit (((SCL_Game*) Data_custom_val (res)), String_val (board));
  CAMLreturn (res);
}

CAMLprim value game_get_board (value game) {
  CAMLparam1 (game);
  CAMLreturn (caml_copy_string (((SCL_Game*) Data_custom_val (game))->board));
}

CAMLprim void game_make_move (value game, value from, value to, value promote) {
  CAMLparam4 (game, from, to, promote);
  SCL_gameMakeMove (((SCL_Game*) Data_custom_val (game)), Int_val (from), Int_val (to), Int_val (promote)); 
  CAMLreturn0;
}

CAMLprim value record_init () {
  CAMLparam0 ();
  CAMLlocal1 (res);
  res = caml_alloc_custom (&record_ops, sizeof (SCL_Record), 0, 1);
  SCL_recordInit (((SCL_Record*) Data_custom_val (res)));
  CAMLreturn (res);
}

CAMLprim value record_from_pgn (value pgn) {
  CAMLparam1 (pgn);
  CAMLlocal1 (res);
  res = caml_alloc_custom (&record_ops, sizeof (SCL_Record), 0, 1);
  SCL_recordFromPGN (((SCL_Record*) Data_custom_val (res)), String_val (pgn));
  CAMLreturn (res);
}

CAMLprim value record_length (value record) {
  CAMLparam1 (record);
  CAMLreturn (Int_val (SCL_recordLength (*((SCL_Record*) Data_custom_val (record)))));
}

CAMLprim void record_add (value record, value from, value to, value promote, value end) {
  CAMLparam5 (record, from, to, promote, end);
  SCL_recordAdd (((SCL_Record*) Data_custom_val (record)), Int_val (from), Int_val (to), Int_val (promote), Int_val (end)); 
  CAMLreturn0;
}

CAMLprim value record_apply (value record, value num_moves) {
  CAMLparam2 (record, num_moves);
  CAMLlocal1 (res);
  SCL_Board board;
  SCL_recordApply (*(((SCL_Record*) Data_custom_val (record))), board, Int_val (num_moves));
  CAMLreturn (caml_copy_string (board));
}