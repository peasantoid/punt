#!/bin/bash

BIN_DIR="/usr/local/bin"
MOD_DIR="/usr/local/lib/punt"

make_punt() {
  gccflags="-Wall -D MOD_DIR=\"$MOD_DIR\""
  
  echo 'main' &&
  gcc -c main.c -o build/punt/obj/main.o $gccflags &&

  echo 'atom' &&
  gcc -c atom.c -o build/punt/obj/atom.o $gccflags &&

  echo 'tokenizer' &&
  gcc -c tokenizer.c -o build/punt/obj/tokenizer.o $gccflags &&

  echo 'engine' &&
  gcc -c engine.c -o build/punt/obj/engine.o $gccflags &&

  echo 'string' &&
  gcc -c string.c -o build/punt/obj/string.o $gccflags &&

  echo 'builtins' &&
  gcc -c builtins.c -o build/punt/obj/builtins.o $gccflags &&

  gcc build/punt/obj/*.o -o build/punt/punt
}

make_clean() {
  find build -type f -exec rm \{\} \;
}

make_install() {
  cp build/punt/punt "$BIN_DIR" &&
  cp build/modules/*.so "$MOD_DIR"
}

mkdir -p build/{,punt/{,obj},modules/{,obj}}
case "$1" in
  clean)
    make_clean
    ;;
  install)
    make_install
    ;;
  *)
    make_punt
    ;;
esac
