README
======

Chess is an OCaml wrapper library around Small Chess, a minimalistic chess library in C.

https://codeberg.org/drummyfish/smallchesslib

Initializing the Build Environment
----------------------------------

Note: the following are a hack solution to fix the fact that brew and opam are
out of sync and opam's owl-plplot library requires library versions that can no
longer be installed with brew.

```bash
opam switch create . 5.1.1+options --no-install
eval $(opam env)
opam update
opam upgrade
dune build chess.opam
opam install --deps-only . -y
dune build
dune runtest
dune exec src/main.exe
```

Once you have compiled Amina and tested any new changes you can install Amina to your local OPAM switch environment using:

```bash
dune install
```