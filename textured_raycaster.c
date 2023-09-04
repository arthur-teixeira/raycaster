#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define screenWidth 640
#define screenHeight 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

int worldMap[mapWidth][mapHeight] = {
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
    {4, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 0, 7, 7, 7, 7, 7},
    {4, 0, 5, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 6, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
    {4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 8, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
    {4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
    {4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 1},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 6, 0, 6, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
    {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 2},
    {4, 0, 0, 5, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
    {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
    {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3}};

Color screen_buffer[screenHeight][screenWidth];

void draw_buffer(Color buffer[screenHeight][screenWidth]) {
  for (int y = 0; y < screenHeight; y++) {
    for (int x = 0; x < screenWidth; x++) {
      DrawPixel(x, y, buffer[y][x]);
    }
  }
}

void clear_buffer(Color buffer[screenHeight][screenWidth]) {
  for (int y = 0; y < screenHeight; y++) {
    for (int x = 0; x < screenWidth; x++) {
      buffer[y][x] = BLACK;
    }
  }
}

void LoadImageTextures(char **images, size_t n, Color **out) {
  for (size_t i = 0; i < n; i++) {
    Image img = LoadImage(images[i]);
    out[i] = LoadImageColors(img);
    UnloadImage(img);
  }
}

void LoadTextures(char **images, size_t n, Texture2D *out) {
  for (size_t i = 0; i < n; i++) {
    out[i] = LoadTexture(images[i]);
  }
}

int main(void) {
  double posX = 22.0, posY = 11.5;
  double dirX = -1, dirY = 0;
  double planeX = 0, planeY = 0.66f;

  InitWindow(screenWidth, screenHeight, "Raycaster");
  SetTargetFPS(60);

  char *images[] = {
      "./assets/eagle.png",       "./assets/redbrick.png",
      "./assets/purplestone.png", "./assets/greystone.png",
      "./assets/bluestone.png",   "./assets/mossy.png",
      "./assets/wood.png",        "./assets/colorstone.png",
  };
  Color *image_textures[ARRAY_LEN(images)];
  LoadImageTextures(images, ARRAY_LEN(images), image_textures);

  Texture rl_textures[ARRAY_LEN(images)];
  LoadTextures(images, ARRAY_LEN(images), rl_textures);

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
      if (drawEnd >= screenHeight)
        drawEnd = screenHeight - 1;

      // -1 so that texture 0 can be used
      int texture_num = worldMap[mapX][mapY] - 1;
      Color *tex = image_textures[texture_num];

      double wall_x; // Where exactly the wall was hit
      if (side == 0) {
        wall_x = posY + perpendicularWallDistance * rayDirY;
      } else {
        wall_x = posX + perpendicularWallDistance * rayDirX;
      }
      wall_x -= floor(wall_x);

      int tex_x = (int)(wall_x * (double)(texWidth));
      if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0)) {
        tex_x = texWidth - tex_x - 1;
      }

      // How much to increase texture coordinate per screen pixel
      double step = (double)texHeight / lineHeight;

      double texPos =
          (drawStart - ((double)screenHeight + lineHeight) / 2) * step;

      for (int y = drawStart; y < drawEnd; y++) {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1)
        // in case of overflow
        int tex_y = (int)texPos & (texHeight - 1);

        texPos += step;

        Color color = tex[tex_y * texWidth + (tex_x % texWidth)];
        if (side == 1) {
          color.a /= 2;
        }

        screen_buffer[y][x] = color;
      }
    }

    draw_buffer(screen_buffer);
    clear_buffer(screen_buffer);
    double frameTime = GetFrameTime();

    char fps_buf[10];
    double fps = 1 / frameTime;
    sprintf(fps_buf, "%d\n", (int)ceil(fps));

    DrawText(fps_buf, 0, 0, 10, RED);

    double moveSpeed = frameTime * 2.0f;
    double rotSpeed = frameTime * 0.75f;

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
}
