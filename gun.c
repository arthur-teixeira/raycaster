#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "game.h"
#include "actors.h"

// PRIVATE METHODS

// Assumes sprites are already sorted since this logic is executed after the
// frame rendering
static int hitGuard(int x, int y) {
  for (int i = 0; i < numActors; i++) {
    Actor s = actors[i];

    // TODO: its a bit imprecise, since the sprites do not occupate the entire
    // tile. This is especially problematic for long distance shots
    if (s.state->texture >= TEXTURE_GUARD_STILL && s.state->texture <= TEXTURE_GUARD_W4 &&
        floor(s.x) == x && floor(s.y) == y) {
      return i;
    }
  }

  return -1;
}

// PUBLIC METHODS

// Returns if the shot hit someone
int Shoot() {
  PlaySound(game.pistol_sfx);
  game.pistol_frame = 1;

  int mapX = (int)posX;
  int mapY = (int)posY;

  Vector2 deltaDist = {
      .x = fabs(1 / dirX),
      .y = fabs(1 / dirY),
  };

  Vector2 sideDist;

  Vector2 step;

  if (dirX < 0) {
    step.x = -1;
    sideDist.x = (posX - mapX) * deltaDist.x;
  } else {
    step.x = 1;
    sideDist.x = (mapX + 1.0 - posX) * deltaDist.x;
  }

  if (dirY < 0) {
    step.y = -1;
    sideDist.y = (posY - mapY) * deltaDist.y;
  } else {
    step.y = 1;
    sideDist.y = (mapY + 1.0 - posY) * deltaDist.y;
  }

  bool hitWall = false;
  SideHit side;

  int hit = -1;

  while (!hitWall && hit < 0) {
    if (sideDist.x < sideDist.y) {
      sideDist.x += deltaDist.x;
      mapX += (int)step.x;
      side = SIDE_WE;
    } else {
      sideDist.y += deltaDist.y;
      mapY += (int)step.y;
      side = SIDE_NS;
    }
    int tile = worldMap[mapX][mapY];

    if (tile == 0) {
      hit = hitGuard(mapX, mapY);
      continue;
    }

    hitWall = true;

    if (tile == DOOR) {
      if (game.doors[mapX][mapY].state == DOOR_OPEN) {
        hitWall = false;
        continue;
      }

      if (side == SIDE_NS) {
        double offsetY = 0.5 * step.y;

        double perpendicularWallDistance =
            (mapY - posY + offsetY + (1 - step.y) / 2) / dirY;

        double wall_x = posX + perpendicularWallDistance * dirY;
        wall_x -= floor(wall_x);

        // If ray hits adjacent wall
        if (sideDist.y - (deltaDist.y / 2) >= sideDist.x) {
          mapX += step.x;
        } else if (1.0 - wall_x <= game.doors[mapX][mapY].timer) {
          hitWall = false;
        }
      } else {
        double offsetX = 0.5 * step.x;
        double perpendicularWallDistance =
            (mapX - posX + offsetX + (1 - step.x) / 2) / dirX;

        double wall_x = posY + perpendicularWallDistance * dirY;
        wall_x -= floor(wall_x);

        if (sideDist.x - (deltaDist.x / 2) >= sideDist.y) {
          mapY += step.y;
        } else if (1.0 - wall_x <= game.doors[mapX][mapY].timer) {
          hitWall = false;
        }
      }
    }
  }

  return hit;
}

void RenderGun() {
  // TODO: scale sprite by screen height
  for (int y = 0; y < gunHeight; y++) {
    for (int x = 0; x < gunWidth; x++) {

      int offset_x = (screenWidth - gunWidth) / 2;
      int offset_y = (screenHeight - gunHeight);

      Color pixel =
          game.pistol_textures[game.pistol_frame / 2][y * gunWidth + x];

      if (pixel.a > 0) {
        game.screen_buffer[(offset_y + y) * screenWidth + (offset_x + x)] =
            pixel;
      }
    }
  }
}

void AnimateGun() {
  if (game.pistol_frame > 0) {
    game.pistol_frame = (game.pistol_frame + 1) % (NUM_PISTOL_FRAMES * 2);
  }
}
