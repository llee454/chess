open! Core
open! Lwt.Syntax
open! Lwt.Infix
open! Chess
open! Ocaml_math

let () =
  Lwt_main.run
  @@ begin
       let pgn =
         {pgn|
[Event ""]
[Site ""]
[Date "????.??.??"]
[Round ""]
[White ""]
[Black ""]
[Result "*"]
[eBoard "DGT 24264"]

1. Nf3 d5 2. g3 Nf6 3. Bg2 Nc6 4. d4 Bf5 5. Rf1 e6 6. Na3 Ng4 7. e3 Be7 8. Rb1
Nb4 9. h3 Nf6 10. Rh1 Nxa2 11. Ne5 Bxa3 12. bxa3 Nc3 13. Nxf7 Kxf7 14. g4 Nxd1
15. gxf5 Nc3 16. fxe6+ Kxe6 17. Rb4 Na2 18. Kd2 Nxb4 19. axb4 Ne4+ 20. Ke2 Qh4
21. Bxe4 Qxe4 22. Rf1 Rhf8 23. Kd2 Rf3 24. Kd1 Raf8 25. Bb2 Rxf2 26. Rxf2 Rxf2
27. Bc1 Qxc2+ 28. Ke1 Qe2# *
|pgn}
       in
       let record = Record.from_pgn pgn in
       let board = Record.apply record 28 in
       (* let board = boardInit () in *)
       (* let fen = "8/8/4pk2/b4pp1/2B5/4P3/r4KPP/8 w - - 0 40" in *)
       (* let fen = "brnbqknr/pppppppp/8/8/8/8/PPPPPPPP/BRNBQKNR w HBhb - 0 1" in *)
       (* let fen = "1rr5/6k1/1p2p2p/2p1b1n1/P5pP/2NRP1P1/1P3P2/3RB1K1 b - h3 0 28" in *)
       (* let () = board_from_fen_exn board fen in *)
       (* let* () = Lwt_io.printlf "initial chess board: %s\n" board in *)
       (* let game = Game.create board in *)
       (* let board0 = Game.get_board game in *)
       (* let* () = Lwt_io.printlf "chess board 0: %s\n" board0 in *)
       (* let* () = Lwt_io.printlf "square number: %d" (get_square_num 'b' 1) in *)
       (* let _ = move board ~from:(get_square_num 'a' 2) ~to_:(get_square_num 'a' 4) ~promote:'q' in *)
       (* let* () = Lwt_io.printlf "was that a legal move? %b\n"
          (is_move_legal board ~from:(get_square_num 'a' 2) ~to_:(get_square_num 'a' 5)) in *)
       let* () = Lwt_io.printlf "chess board: %s\n" board in
       Lwt.return_unit
     end
