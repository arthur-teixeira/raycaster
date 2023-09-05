#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -ggdb -O3 `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lm"

mkdir -p ./bin/

clang $CFLAGS -o ./bin/raycaster  ./untextured_raycaster.c $LIBS -L./bin/
clang $CFLAGS -o ./bin/textured_raycaster  ./textured_raycaster.c $LIBS -L./bin/
