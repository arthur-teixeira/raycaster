#include <math.h>

#include "game.h"

// PRIVATE METHODS

static void Move(double moveSpeed, signed char factor) {
  int x = posX + (factor * dirX * moveSpeed);
  int y = posY;
  int curTile = worldMap[x][y];

  if (curTile == 0 ||
      (curTile == DOOR && game.doors[x][y].state == DOOR_OPEN)) {
    posX += factor * dirX * moveSpeed;
  }

  x = posX;
  y = posY + (factor * dirY * moveSpeed);
  curTile = worldMap[x][y];

  if (curTile == 0 ||
      (curTile == DOOR && game.doors[x][y].state == DOOR_OPEN)) {
    posY += factor * dirY * moveSpeed;
  }
}

static void Rotate(double rotSpeed, signed char factor) {
  double oldDirX = dirX;
  rotSpeed = factor * rotSpeed;

  dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
  dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

  double oldPlaneX = planeX;
  planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
  planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
}

// PUBLIC METHODS

void UpdatePosition(float frameTime) {
  double moveSpeed = frameTime * 2.0f;
  double rotSpeed = frameTime * 2.0f;

  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
    Move(moveSpeed, 1);
  }
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
    Move(moveSpeed, -1);
  }
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
    Rotate(rotSpeed, -1);
  }
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
    Rotate(rotSpeed, 1);
  }
}
