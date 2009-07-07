#!/bin/bash

BIN_DIR='/usr/local/bin'
MOD_DIR='/usr/local/lib/punt'

make_punt() {
  echo 'Building interpreter...'

  gccflags="-Wall -D MOD_DIR=\"$MOD_DIR\" -fPIC"

  echo '  atom' &&
  gcc -c atom.c -o build/punt/obj/atom.o $gccflags &&

  echo '  tokenizer' &&
  gcc -c tokenizer.c -o build/punt/obj/tokenizer.o $gccflags &&

  echo '  engine' &&
  gcc -c engine.c -o build/punt/obj/engine.o $gccflags -l dl &&

  echo '  string' &&
  gcc -c string.c -o build/punt/obj/string.o $gccflags &&

  echo '  builtins' &&
  gcc -c builtins.c -o build/punt/obj/builtins.o $gccflags &&

  echo '  helpers' &&
  gcc -c helpers.c -o build/punt/obj/helpers.o $gccflags &&

  gcc main.c build/punt/obj/*.o -o build/punt/punt -l dl
}

make_modules() {
  echo 'Building modules...'
  
  export OBJ_DIR="$PWD/build/punt/obj"
  for mod in modules/*; do
    export MOD_NAME=$(basename "$mod")

    if test -f disabled-modules && 
        grep -q "^$MOD_NAME\$" disabled-modules; then
      echo "  - $MOD_NAME"
      continue
    fi

    echo "  + $MOD_NAME"
    oldwd=$(pwd)

    cd "$mod" &&
    bash ./make
    cd "$oldwd"

    mv "$mod/$MOD_NAME.so" build/modules
  done
}

make_clean() {
  find build -type f -exec rm \{\} \;
}

make_install() {
  mkdir -p "$BIN_DIR" &&
  mkdir -p "$MOD_DIR" &&

  cp build/punt/punt "$BIN_DIR" &&
  cp build/modules/*.so "$MOD_DIR"
}

make_enmod() {
  test -z "$1" && return
  touch disabled-modules

  sed "/$1/d" disabled-modules -i
}

make_dismod() {
  { test -z "$1" || grep -q "^$1\$" disabled-modules ;} && return
  touch disabled-modules

  echo "$1" >> disabled-modules
}

make_help() {
  cat <<EOF
./make [action] [args]
  (default)          -- build interpreter and modules
  ./make clean       -- remove build products
  ./make install     -- install everything
  ./make punt        -- build punt interpreter
  ./make modules     -- build modules
  ./make enmod \$mod  -- enable building of module \$mod
  ./make dismod \$mod -- disable building of module \$mod
EOF
}

mkdir -p build/{,punt/{,obj},modules/}
case "$1" in
  clean)
    make_clean
    ;;
  install)
    make_install
    ;;
  punt)
    make_punt
    ;;
  modules)
    make_modules
    ;;
  enmod)
    make_enmod "$2"
    ;;
  dismod)
    make_dismod "$2"
    ;;
  help)
    make_help
    ;;
  *)
    make_punt &&
    make_modules
    ;;
esac

