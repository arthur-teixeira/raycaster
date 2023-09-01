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

void calculateStepAndSideDist(int rayDir, int pos, int map, int deltaDist,
                              int *step, double *sideDist) {
  if (rayDir < 0) {
    *step = 1;
    *sideDist = (pos - map) * deltaDist;
  } else {
    *step = 1;
    *sideDist = (map + 1.0f - pos) * deltaDist;
  }
}

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
  double posX = 22, posY = 12;
  double dirX = -1, dirY = 0;
  double planeX = 0, planeY = 0.66f;

  double time = 0;
  double oldTime = 0;

  InitWindow(screenWidth, screenHeight, "Raycaster");
  while (!WindowShouldClose()) {
    BeginDrawing();

    for (int x = 0; x < screenWidth; x++) {
      double cameraX =
          2 * x / (double)screenWidth - 1; // The x coordinate in camera space
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      // Which box of the map we are currently in.
      int currentMapSquareX = (int)posX;
      int currentMapSquareY = (int)posY;

      // Length of the ray from the current position to the next x-y side
      double sideDistX;
      double sideDistY;

      // Distance from one x-y side to the next x-y side
      double deltaDistX = (rayDirX == 0) ? INFINITY : fabs(1 / rayDirX);
      double deltaDistY = (rayDirY == 0) ? INFINITY : fabs(1 / rayDirY);

      // What direction to step in each axis
      int stepX;
      int stepY;

      calculateStepAndSideDist(rayDirX, posX, currentMapSquareX, deltaDistX,
                               &stepX, &sideDistX);
      calculateStepAndSideDist(rayDirY, posY, currentMapSquareY, deltaDistY,
                               &stepY, &sideDistY);

      bool hit = false;
      int side; // North-South or East-West

      // Perform DDA
      while (!hit) {
        // Jump to next square, either in the x or y direction
        if (sideDistX < sideDistY) {
          sideDistX += deltaDistX;
          currentMapSquareX += stepX;
          side = 0;
        } else {
          sideDistY += deltaDistY;
          currentMapSquareY += stepY;
        }

        hit = (worldMap[currentMapSquareX][currentMapSquareY] > 0);
      }

      double perpendicularWallDistance;
      if (side == 0) { // Hit on x axis
        perpendicularWallDistance = (sideDistX - deltaDistX);
      } else {
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

      Color color = getColor(worldMap[currentMapSquareX][currentMapSquareY]);
      if (side == 1) {
        color.a /= 2; // If the wall was hit sideways, show it darker
      }

      DrawLine(x, drawStart, x, drawEnd, color);
    }

    oldTime = time;
    time = GetTime();
    double frameTime = time - oldTime; // GetFrameTime()?

    double moveSpeed = frameTime * 5.0f;
    double rotSpeed = frameTime * 3.0f;
    if (IsKeyPressed(KEY_UP)) {
        if (worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == 0) {
            posX += dirX *moveSpeed;
        }
        if (worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == 0) {
            posY += dirY *moveSpeed;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        if (worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == 0) {
            posX += dirX *moveSpeed;
        }
        if (worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0) {
            posY += dirY *moveSpeed;
        }
    }
    if (IsKeyPressed(KEY_LEFT)) {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }


    EndDrawing();
  }

  return 0;
}
