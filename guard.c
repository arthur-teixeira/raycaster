#include "game.h"

static void MoveGuard(double *posX, double *posY, Vector2 dir, double moveSpeed,
                      signed char factor) {
  int x = *posX + (factor * dir.x * moveSpeed);
  int y = *posY;
  int curTile = worldMap[x][y];

  if (curTile == 0 ||
      (curTile == DOOR && game.doors[x][y].state == DOOR_OPEN)) {
    *posX += factor * dir.x * moveSpeed;
  }

  x = *posX;
  y = *posY + (factor * dir.y * moveSpeed);
  curTile = worldMap[x][y];

  if (curTile == 0 ||
      (curTile == DOOR && game.doors[x][y].state == DOOR_OPEN)) {
    *posY += factor * dir.y * moveSpeed;
  }
}

static void AnimateGuard(int i, int initialFrame, int finalFrame,
                         float frameTime, bool loops) {
  Guard g = guards[i];

  guards[i].frameCounter -= frameTime;
  if (guards[i].frameCounter <= 0) {
    if (sprites[g.spriteIndex].texture == finalFrame) {
      if (loops) {
        sprites[g.spriteIndex].texture = initialFrame;
      }
    } else {
      sprites[g.spriteIndex].texture += 1;
    }
    guards[i].frameCounter = guards[i].frameDuration;
  }
}

void MoveGuards(float frameTime) {
  for (int i = 0; i < numGuards; i++) {
    Guard g = guards[i];

#if DEBUG
    if (IsKeyPressed(KEY_SPACE)) {
      if (g.state == GUARD_STOPPED) {
        guards[i].state = GUARD_WALKING;
      } else if (g.state == GUARD_WALKING) {
        guards[i].state = GUARD_STOPPED;
      }
    }
#endif

    if (g.state == GUARD_WALKING) {
      MoveGuard(&sprites[g.spriteIndex].x, &sprites[g.spriteIndex].y, g.dir,
                frameTime / 2, 1);

      int initialFrame = TEXTURE_GUARD_W1;
      int finalFrame = TEXTURE_GUARD_W4;
      AnimateGuard(i, initialFrame, finalFrame, frameTime, true);
    }
  }
}

void AnimateDyingGuards(float frameTime) {
  for (int i = 0; i < numGuards; i++) {
    if (guards[i].state == GUARD_DYING) {
      AnimateGuard(i, TEXTURE_GUARD_D1, TEXTURE_GUARD_D5, frameTime, false);
    }
  }
}

void KillGuard(int guard) {
  int i = 0;
  for (; i < numGuards; i++) {
    if (guards[i].spriteIndex == guard) {
      break;
    }
  }

  guards[i].state = GUARD_DYING;
  guards[i].frameDuration = 0.2 / 3;
  guards[i].frameCounter = 0.2 / 3;
  sprites[guards[i].spriteIndex].texture = TEXTURE_GUARD_D1;
}
