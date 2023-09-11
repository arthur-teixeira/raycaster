#include "game.h"
#include <math.h>
#include "raycast.h"
#include "game.h"

void CastFloorAndCeiling() {
  // Vertical position of the camera
  float posZ = (float)screenHeight / 2;

  for (int y = (int)posZ; y < screenHeight; y++) {
    // Leftmost ray
    Vector2 rayDir0 = {
        .x = dirX - planeX,
        .y = dirY - planeY,
    };

    // Rightmost ray
    Vector2 rayDir1 = {
        .x = dirX + planeX,
        .y = dirY + planeY,
    };

    // current y position compared to the center of the screen
    int p = y - screenHeight / 2;

    // Horizontal distance from the camera to the floor for the current row
    float rowDistance = posZ / p;

    // Calculate the step vector we have to add for each x by linear
    // interpolation
    Vector2 floorStep = {
        .x = rowDistance * (rayDir1.x - rayDir0.x) / screenWidth,
        .y = rowDistance * (rayDir1.y - rayDir0.y) / screenWidth,
    };

    // world coordinates of the leftmost column. Will be updated as we go to
    // the right.
    Vector2 floor = {
        .x = posX + rowDistance * rayDir0.x,
        .y = posY + rowDistance * rayDir0.y,
    };

    for (int x = 0; x < screenWidth; x++) {
      int cellX = (int)floor.x;
      int cellY = (int)floor.y;

      int tx = (int)(texWidth * (floor.x - cellX)) & (texWidth - 1);
      int ty = (int)(texHeight * (floor.y - cellY)) & (texHeight - 1);

      floor.x += floorStep.x;
      floor.y += floorStep.y;

      int floorTexture = 3;
      int ceilTexture = 6;

      Color color;
      // floor
      color = game.image_textures[floorTexture][texWidth * ty + tx];
      color.a /= 2;
      game.screen_buffer[y * screenWidth + x] = color;

      // Ceiling
      color = game.image_textures[ceilTexture][texWidth * ty + tx];
      color.a /= 2;
      game.screen_buffer[(screenHeight - y - 1) * screenWidth + x] = color;
    }
  }
}

void CastWalls() {
  for (int x = 0; x < screenWidth; x++) {
    // The x coordinate in camera space
    double cameraX = 2 * x / (double)screenWidth - 1;

    Vector2 rayDir = {
        .x = dirX + planeX * cameraX,
        .y = dirY + planeY * cameraX,
    };

    // Which box of the map we are currently in.
    int mapX = (int)posX;
    int mapY = (int)posY;

    // Distance from one x-y side to the next x-y side
    Vector2 deltaDist = {
        .x = (rayDir.x == 0) ? INFINITY : fabs(1 / rayDir.x),
        .y = (rayDir.y == 0) ? INFINITY : fabs(1 / rayDir.y),
    };

    // Length of the ray from the current position to the next x-y side
    Vector2 sideDist;

    // What direction to step in each axis
    Vector2 step;

    if (rayDir.x < 0) {
      step.x = -1;
      sideDist.x = (posX - mapX) * deltaDist.x;
    } else {
      step.x = 1;
      sideDist.x = (mapX + 1.0 - posX) * deltaDist.x;
    }

    if (rayDir.y < 0) {
      step.y = -1;
      sideDist.y = (posY - mapY) * deltaDist.y;
    } else {
      step.y = 1;
      sideDist.y = (mapY + 1.0 - posY) * deltaDist.y;
    }

    bool hit = false;
    SideHit side;
    int tile;

    Vector2 wallOffset = {
        .x = 0,
        .y = 0,
    };

    double perpendicularWallDistance;
    double wall_x; // Where exactly the wall was hit

    // Perform DDA
    while (!hit) {
      // Jump to next square, either in the x or y direction
      if (sideDist.x < sideDist.y) {
        sideDist.x += deltaDist.x;
        mapX += (int)step.x;
        side = SIDE_WE;
      } else {
        sideDist.y += deltaDist.y;
        mapY += (int)step.y;
        side = SIDE_NS;
      }
      tile = worldMap[mapX][mapY];

      if (tile == 0) {
        hit = false;
        continue;
      }

      hit = true;

      // https://github.com/almushel/raycast-demo
      if (tile == DOOR) {
        DoorInfo door = game.doors[mapX][mapY];

        if (door.state == DOOR_OPEN) {
          hit = false;
          continue;
        }

        if (side == SIDE_NS) {
          // Offsetting the wall to the middle of the tile
          wallOffset.y = 0.5 * step.y;

          perpendicularWallDistance =
              (mapY - posY + wallOffset.y + (1 - step.y) / 2) / rayDir.y;

          wall_x = posX + perpendicularWallDistance * rayDir.x;
          wall_x -= floor(wall_x);

          // If ray hits adjacent wall
          if (sideDist.y - (deltaDist.y / 2) >= sideDist.x) {
            mapX += step.x;
            tile = DOOR_FRAME;
            side = SIDE_WE;
            wallOffset.y = 0;
          } else {
            if (1.0 - wall_x <= game.doors[mapX][mapY].timer) {
              hit = false;
              wallOffset.y = 0;
            }
          }
        } else {
          wallOffset.x = 0.5 * step.x;
          perpendicularWallDistance =
              (mapX - posX + wallOffset.x + (1 - step.x) / 2) / rayDir.x;

          wall_x = posY + perpendicularWallDistance * rayDir.y;
          wall_x -= floor(wall_x);

          if (sideDist.x - (deltaDist.x / 2) >= sideDist.y) {
            mapY += step.y;
            side = SIDE_NS;
            tile = DOOR_FRAME;
            wallOffset.x = 0;
          } else {
            if (1.0 - wall_x <= game.doors[mapX][mapY].timer) {
              hit = false;
              wallOffset.x = 0;
            }
          }
        }
      } else {
        if (side == SIDE_NS && worldMap[mapX][(int)(mapY - step.y)] == DOOR) {
          tile = DOOR_FRAME;
        } else if (side == SIDE_WE &&
                   worldMap[(int)(mapX - step.x)][mapY] == DOOR) {
          tile = DOOR_FRAME;
        }
      }
    }

    if (side == SIDE_WE) { // Hit on x axis
      perpendicularWallDistance =
          (mapX - posX + wallOffset.x + (1 - step.x) / 2) / rayDir.x;
    } else {
      perpendicularWallDistance =
          (mapY - posY + wallOffset.y + (1 - step.y) / 2) / rayDir.y;
    }

    // Height of line to draw on the screen
    int lineHeight = (int)(screenHeight / perpendicularWallDistance);

    int drawStart = (-lineHeight + screenHeight) / 2;
    if (drawStart < 0)
      drawStart = 0;

    int drawEnd = (lineHeight + screenHeight) / 2;
    if (drawEnd >= screenHeight)
      drawEnd = screenHeight - 1;

    // -1 so that texture 0 can be used
    int texture_num = tile - 1;
    Color *tex = game.image_textures[texture_num];

    if (side == SIDE_WE) {
      wall_x = posY + perpendicularWallDistance * rayDir.y;
    } else {
      wall_x = posX + perpendicularWallDistance * rayDir.x;
    }
    wall_x -= floor(wall_x);

    if (tile == DOOR) {
      wall_x += game.doors[mapX][mapY].timer;
    }

    int tex_x = (int)(wall_x * (double)(texWidth));

    if ((side == 0 && rayDir.x < 0) || (side == 1 && rayDir.y < 0)) {
      // Not inverting the door so the handle is always on the same side
      if (tile != DOOR) {
        tex_x = texWidth - tex_x - 1;
      }
    }

    // How much to increase texture coordinate per screen pixel
    double tex_step = (double)texHeight / lineHeight;

    double texPos =
        (drawStart - ((double)screenHeight + lineHeight) / 2) * tex_step;

    for (int y = drawStart; y < drawEnd; y++) {
      // Cast the texture coordinate to integer, and mask with (texHeight - 1)
      // in case of overflow
      int tex_y = (int)texPos & (texHeight - 1);

      texPos += tex_step;

      Color color = tex[tex_y * texWidth + (tex_x % texWidth)];
      if (side == 1) {
        color.a /= 2;
      }

      game.screen_buffer[y * screenWidth + x] = color;
    }

    // ZBuffer used for sprites
    game.ZBuffer[x] = perpendicularWallDistance;
  }
}

