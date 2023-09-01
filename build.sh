#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lm"

mkdir -p ./bin/

clang $CFLAGS  -o ./bin/raycaster  ./untextured_raycaster.c $LIBS -L./bin/
