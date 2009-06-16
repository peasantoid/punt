#!/bin/bash

BIN_DIR="/usr/local/bin"
MOD_DIR="/usr/local/lib/punt"

make_punt() {
  gccflags="-Wall -D MOD_DIR=\"$MOD_DIR\""

  gcc -c main.c -o build/punt/obj/main.o $gccflags

  gcc build/punt/obj/*.o -o build/punt/punt
}

make_clean() {
  find build -type f -exec rm \{\} \;
}

make_install() {
  cp build/punt/punt "$BIN_DIR"
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

