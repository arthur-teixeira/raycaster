#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "game.h"

// https://www.reddit.com/r/raylib/comments/hcglzh/comment/g212jbl/?utm_source=share&utm_medium=web2x&context=3
void DrawFrame(Texture2D frame_tex) {
  UpdateTexture(frame_tex, game.screen_buffer);
  DrawTexture(frame_tex, 0, 0, WHITE);
}

void DrawFrameCounter(float frameTime) {
  DrawText(TextFormat("%d", (int)(1 / frameTime)), 0, 0, 10, RED);
}

void Move(double moveSpeed, signed char factor) {
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

void Rotate(double rotSpeed, signed char factor) {
  double oldDirX = dirX;
  rotSpeed = factor * rotSpeed;

  dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
  dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

  double oldPlaneX = planeX;
  planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
  planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
}

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

void OpenDoor(int x, int y) {
  DoorInfo door = game.doors[x][y];

  // TODO: Don't close the door if the player is on the tile
  switch (door.state) {
  case DOOR_CLOSED:
    PlaySound(game.door_sfx);
    game.doors[x][y].state = DOOR_OPENING;
    break;
  case DOOR_OPEN:
    PlaySound(game.door_sfx);
    game.doors[x][y].state = DOOR_CLOSING;
    break;
  default:
    break;
  }
}

void Interact(float frameTime) {
  double moveSpeed = frameTime * 2.0f;

  // TODO: open door from 2 tiles away
  if (IsKeyPressed(KEY_SPACE)) {
    int tileX = worldMap[(int)(posX + (dirX * moveSpeed))][(int)posY];
    int tileY = worldMap[(int)posX][(int)(posY + (dirY * moveSpeed))];

    if (tileX == DOOR) {
      OpenDoor(posX + (dirX * moveSpeed), posY);
    } else if (tileY == DOOR) {
      OpenDoor(posX, posY + (dirY * moveSpeed));
    }
  }
  if (IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL)) {
    PlaySound(game.pistol_sfx);
    game.pistol_frame = 1;
  }
}

void AnimatePistol(float frameTime) {
  if (game.pistol_frame > 0) {
    game.pistol_frame = (game.pistol_frame + 1) % (NUM_PISTOL_FRAMES * 2);
  }
}

void MoveDoors(float frameTime) {
  for (int y = 0; y < mapWidth; y++) {
    for (int x = 0; x < mapHeight; x++) {
      if (worldMap[x][y] == DOOR) {
        DoorInfo door = game.doors[x][y];

        if (door.state == DOOR_OPENING) { // Timer goes from 0 to 1.
          game.doors[x][y].timer += frameTime;
          if (game.doors[x][y].timer >= 1) {
            game.doors[x][y].state = DOOR_OPEN;
            game.doors[x][y].timer = 1;
          }
        } else if (door.state == DOOR_CLOSING) { // Timer goes from 1 to 0.
          game.doors[x][y].timer -= frameTime;
          if (game.doors[x][y].timer <= 0) {
            game.doors[x][y].state = DOOR_CLOSED;
            game.doors[x][y].timer = 0;
          }
        }
      }
    }
  }
}

void RenderFloorAndCeiling() {
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

void RenderWalls() {
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

int partition(int *order, double *dist, int p, int r) {
  double x = dist[p];
  int i = p - 1;
  int j = r + 1;

  while (true) {
    while (true) {
      j -= 1;
      if (dist[j] <= x) {
        break;
      }
    }

    while (true) {
      i += 1;
      if (dist[i] >= x) {
        break;
      }
    }

    if (i < j) {
      int temp = order[i];
      order[i] = order[j];
      order[j] = temp;

      double dtemp = dist[i];
      dist[i] = dist[j];
      dist[j] = dtemp;
    } else {
      return j;
    }
  }
}

void quicksort(int *order, double *dist, int p, int r) {
  if (p < r) {
    int q = partition(order, dist, p, r);
    quicksort(order, dist, p, q);
    quicksort(order, dist, q + 1, r);
  }
}

void sortSprites(int *order, double *dist, int amount) {
  quicksort(order, dist, 0, amount - 1);

  for (int i = 0; i < amount / 2; i++) {
    int order_temp = order[i];
    order[i] = order[amount - i - 1];
    order[amount - i - 1] = order_temp;

    double dist_temp = dist[i];
    dist[i] = dist[amount - i - 1];
    dist[amount - i - 1] = dist_temp;
  }
}

void RenderSprites() {
  for (int i = 0; i < numSprites; i++) {
    game.spriteOrder[i] = i;
    double xComponent = posX - sprites[i].x;
    double yComponent = posY - sprites[i].y;
    game.spriteDistance[i] =
        (xComponent * xComponent) + (yComponent * yComponent);
  }
  sortSprites(game.spriteOrder, game.spriteDistance, numSprites);

  for (int i = 0; i < numSprites; i++) {
    // Translate position to relative to camera
    Vector2 sprite = {
        .x = sprites[game.spriteOrder[i]].x - posX,
        .y = sprites[game.spriteOrder[i]].y - posY,
    };

    // Transform sprite with the inverse camera matrix
    // [ planeX  dirX ] ^-1               1                [ dirY   -dirX   ]
    // [              ]    = ----------------------------- [                ]
    // [ planeY  dirY ]      planeX * dirY - dirX * planeY [ -planeY planeX ]

    double invDet = 1 / (planeX * dirY - dirX * planeY);

    Vector2 transform = {
        .x = invDet * (dirY * sprite.x - dirX * sprite.y),
        .y = invDet * (-planeY * sprite.x + planeX * sprite.y),
    };

    int spriteScreenX =
        (int)((screenWidth / 2) * (1 + transform.x / transform.y));

    // Using the y component of the transformed vector prevents fisheye
    int spriteHeight = (int)floor(fabs(screenHeight / transform.y));

    // Calculate lowest and highest pixels
    int drawStartY = -spriteHeight / 2 + screenHeight / 2;
    if (drawStartY < 0) {
      drawStartY = 0;
    }
    int drawEndY = spriteHeight / 2 + screenHeight / 2;
    if (drawEndY >= screenHeight) {
      drawEndY = screenHeight - 1;
    }

    // Calculate sprite width
    int spriteWidth = spriteHeight;
    int drawStartX = -spriteWidth / 2 + spriteScreenX;

    if (drawStartX < 0) {
      drawStartX = 0;
    }
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    if (drawEndX >= screenWidth) {
      drawEndX = screenWidth - 1;
    }

    // Loop through vertical stripes of the sprite on screen
    for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
      int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) *
                       texWidth / spriteWidth) /
                 256;

      // If
      // 1) it is in front of the camera plane
      // 2) it is on the screen (left)
      // 3) it is on the screen (right)
      // 4) ZBuffer with perpendicular distance
      if ((transform.y > 0) && (stripe > 0) && (stripe < screenWidth) &&
          (transform.y < game.ZBuffer[stripe])) {

        for (int y = drawStartY; y < drawEndY; y++) {
          // 256 and 128 factors to avoid floats
          int d = y * 256 - screenHeight * 128 + spriteHeight * 128;
          int texY = (d * texHeight) / (spriteHeight * 256);
          Color color =
              game.image_textures[sprites[game.spriteOrder[i]].texture]
                                 [texWidth * texY + texX];
          if (color.r > 0 || color.g > 0 || color.b > 0) {
            game.screen_buffer[y * screenWidth + stripe] = color;
          }
        }
      }
    }
  }
}

Texture2D LoadInitialFrame() {
  Image initial_frame = {
      .data = game.screen_buffer,
      .width = screenWidth,
      .height = screenHeight,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };
  return LoadTextureFromImage(initial_frame);
}

void RenderGun() {
  // TODO: scale sprite by screen height
  for (int y = 0; y < gunHeight; y++) {
    for (int x = 0; x < gunWidth; x++) {

      int offset_x = (screenWidth - gunWidth) / 2;
      int offset_y = (screenHeight - gunHeight);

      Color pixel = game.pistol_textures[game.pistol_frame / 2][y * gunWidth + x];

      if (pixel.a > 0) {
        game.screen_buffer[(offset_y + y) * screenWidth + (offset_x + x)] =
            pixel;
      }
    }
  }
}

int main(void) {
  InitWindow(screenWidth, screenHeight, "Raycaster");
  InitAudioDevice();
#if LIMIT_FPS
  SetTargetFPS(60);
#endif
  InitGame();

  Texture2D frame = LoadInitialFrame();

  while (!WindowShouldClose()) {
#if !MUTED
    UpdateMusicStream(game.soundtrack);
#endif

    BeginDrawing();
    ClearBackground(BLANK);

    RenderFloorAndCeiling();
    RenderWalls();
    RenderSprites();
    RenderGun();

    DrawFrame(frame);

    double frameTime = GetFrameTime();

#if DEBUG
    DrawFrameCounter(frameTime);
#endif

    EndDrawing();

    UpdatePosition(frameTime);
    Interact(frameTime);
    MoveDoors(frameTime);
    AnimatePistol(frameTime);
  }

  UnloadTexture(frame);

  CloseAudioDevice();
  CloseWindow();

  return 0;
}
