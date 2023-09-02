#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapWidth][mapHeight] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

Color getColor(int location) {
  switch (location) {
  case 1:
    return RED;
  case 2:
    return GREEN;
  case 3:
    return BLUE;
  case 4:
    return WHITE;
  default:
    return YELLOW;
  }
}

int main(void) {
  double posX = 3, posY = 3;
  double dirX = -1, dirY = 0;
  double planeX = 0, planeY = 0.66f;

  InitWindow(screenWidth, screenHeight, "Raycaster");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    for (int x = 0; x < screenWidth; x++) {
      double cameraX =
          2 * x / (double)screenWidth - 1; // The x coordinate in camera space
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      // Which box of the map we are currently in.
      int mapX = (int)posX;
      int mapY = (int)posY;

      // Length of the ray from the current position to the next x-y side
      double sideDistX;
      double sideDistY;

      // Distance from one x-y side to the next x-y side
      double deltaDistX = (rayDirX == 0) ? INFINITY : fabs(1 / rayDirX);
      double deltaDistY = (rayDirY == 0) ? INFINITY : fabs(1 / rayDirY);

      // What direction to step in each axis
      int stepX;
      int stepY;

      if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }

      if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

      bool hit = false;
      int side; // North-South or East-West

      // Perform DDA
      while (!hit) {
        // Jump to next square, either in the x or y direction
        if (sideDistX < sideDistY) {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        } else {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }

        hit = (worldMap[mapX][mapY] > 0);
      }

      double perpendicularWallDistance;
      if (side == 0) { // Hit on x axis
        perpendicularWallDistance = (sideDistX - deltaDistX);
      } else { // Hit on Y axis
        perpendicularWallDistance = (sideDistY - deltaDistY);
      }

      // Height of line to draw on the screen
      int lineHeight = (int)(screenHeight / perpendicularWallDistance);

      int drawStart = (-lineHeight + screenHeight) / 2;
      if (drawStart < 0)
        drawStart = 0;

      int drawEnd = (lineHeight + screenHeight) / 2;
      if (drawEnd < 0)
        drawEnd = 0;

      Color color = getColor(worldMap[mapX][mapY]);
      if (side == 1) {
        color.a /= 2; // If the wall was hit sideways, show it darker
      }

      DrawLine(x, drawStart, x, drawEnd, color);
    }

    double frameTime = GetFrameTime();

    double moveSpeed = frameTime;
    double rotSpeed = frameTime *0.75f;

    ClearBackground(BLACK);
    EndDrawing();

    if (IsKeyDown(KEY_UP)) {
      if (worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == 0) {
        posX += dirX * moveSpeed;
      }
      if (worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == 0) {
        posY += dirY * moveSpeed;
      }
    }
    if (IsKeyDown(KEY_DOWN)) {
      if (worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == 0) {
        posX -= dirX * moveSpeed;
      }
      if (worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0) {
        posY -= dirY * moveSpeed;
      }
    }
    if (IsKeyDown(KEY_RIGHT)) {
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (IsKeyDown(KEY_LEFT)) {
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
  }

  return 0;
}
