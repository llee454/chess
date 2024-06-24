open! Core

type board = string

type move_undo

module Square = struct
  type t = {
    row: int;
    col: int;
  }
  [@@deriving sexp]

  let col_to_char = Array.get [| 'a'; 'b'; 'c'; 'd'; 'e'; 'f'; 'g'; 'h' |]

  let to_string x = sprintf "%c%d" (col_to_char x.col) (succ x.row)
end

external boardInit : unit -> board = "boardInit"

external boardInit960 : int -> board = "boardInit960"

(**
  Accepts a board and an FEN string and sets the board to match
  the position described by the FEN string.
*)
external board_from_fen_exn : board -> string -> unit = "board_from_fen"

(**
  We represent every square on the chess board using a unique
  number. This function takes a square coordinate such as ('a' 8)
  and returns the associated square number.
*)
external get_square_num_aux : char -> int -> int = "get_square_num"

(**
  We represent every square on the chess board using a unique
  number.
*)
let get_square_num = Fn.compose get_square_num_aux Square.col_to_char

(**
  Accepts four arguments: the current board state; the square that
  you are moving a piece from; the square that you are moving the
  piece to; and the type of piece that you will promote to (such
  as a 'q') if applicable; and updates the board to reflect the
  given move.

  WARNING: this function does not verify that the move is legal.
*)
external move : board -> from:int -> to_:int -> promote:char -> move_undo = "move"

(**
  Accepts three argumets: the current board state; the square that
  you are moving a piece from; the square that you are moving to;
  and returns true iff the move is legal.
*)
external is_move_legal : board -> from:int -> to_:int -> bool = "is_move_legal"

(**
  Accepts two arguments: the current board state and a square;
  and returns the set of possible moves in an array.

  Note: some of the entries in the array may equal 0. These do not
  represent a move and should be ignored. If the array is entirely
  composed of 0s, the piece does not have any possible moves.
*)
external get_moves : board -> int -> Square.t array = "get_moves"

module Game = struct
  type t

  external create : board -> t = "game_init"

  external get_board : t -> board = "game_get_board"

  external move : t -> from:int -> to_:int -> promote:char -> unit = "game_make_move"
end

module Record = struct
  type t

  external init : unit -> t = "record_init"

  (** WARNING: this function only works for standard chess. Fisher Random Chess is not supported using PGNs. *)
  external from_pgn : string -> t = "record_from_pgn"

  external length : t -> int = "record_length"

  external add : t -> from:int -> to_:int -> promote:char -> end_state:int -> unit = "record_add"

  external apply : t -> int -> board = "record_apply"
end