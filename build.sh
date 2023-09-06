#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -ggdb `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lm"

mkdir -p ./bin/

clang $CFLAGS -o ./bin/raycaster  ./main.c $LIBS -L./bin/
