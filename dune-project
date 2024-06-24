(lang dune 3.16)
(name chess)
(license GPL-3.0)
(authors "Larry D. Lee Jr. <llee454@gmail.com>")
(maintainers "Larry D. Lee Jr. <llee454@gmail.com>")
(source (github llee454/chess))
(version 0.00.1)
(use_standard_c_and_cxx_flags true)
(generate_opam_files)
(package
 (name chess)
 (synopsis "This package is an OCaml wrapper for Small Chess Library.")
 (description "This package is an OCaml wrapper for Small Chess Library.")
 (depends
  ocaml-lsp-server
  ocamlformat
  ocaml_math
  ppx_jane
  ppx_blob
  core
  lwt
  lwt_ppx
  yojson
  ppx_deriving_yojson
  getopt
 )
)