#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FULLSCREEN 0
#define DEBUG 1

#if FULLSCREEN
#define screenWidth 1440
#define screenHeight 1080
#else
#define screenWidth 640
#define screenHeight 480
#endif
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24


#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

int worldMap[mapWidth][mapHeight] = {
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {8, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 4, 6},
    {8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 0, 0, 6},
    {7, 7, 7, 7, 0, 7, 7, 7, 7, 0, 8, 0, 8, 0, 8, 0, 8, 4, 0, 4, 0, 6, 0, 6},
    {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 0, 0, 0, 0, 0, 6},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 4},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 6, 0, 6, 0, 6},
    {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 4, 6, 0, 6, 6, 6},
    {7, 7, 7, 7, 0, 7, 7, 7, 7, 8, 8, 4, 0, 6, 8, 4, 8, 3, 3, 3, 0, 3, 3, 3},
    {2, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 4, 0, 0, 6, 0, 6, 3, 0, 0, 0, 0, 0, 3},
    {2, 2, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 6, 0, 6, 3, 3, 0, 0, 0, 3, 3},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 6, 6, 0, 0, 5, 0, 5, 0, 5},
    {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
    {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
    {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5},
};

Color screen_buffer[screenHeight * screenWidth];

char *images[] = {
    "./assets/eagle.png",       "./assets/redbrick.png",
    "./assets/purplestone.png", "./assets/greystone.png",
    "./assets/bluestone.png",   "./assets/mossy.png",
    "./assets/wood.png",        "./assets/colorstone.png",
};

Color *image_textures[ARRAY_LEN(images)];

double posX = 22.0, posY = 11.5;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66f;

// https://www.reddit.com/r/raylib/comments/hcglzh/comment/g212jbl/?utm_source=share&utm_medium=web2x&context=3
void DrawFrame(Texture2D frame_tex) {
  UpdateTexture(frame_tex, screen_buffer);
  DrawTexture(frame_tex, 0, 0, WHITE);
}

void LoadImageTextures() {
  for (size_t i = 0; i < ARRAY_LEN(images); i++) {
    Image img = LoadImage(images[i]);
    image_textures[i] = LoadImageColors(img);
    UnloadImage(img);
  }
}

void UnloadColors() {
  for (size_t i = 0; i < ARRAY_LEN(image_textures); i++) {
    UnloadImageColors(image_textures[i]);
  }
}

void DrawFrameCounter(float frameTime) {
  DrawText(TextFormat("%d", (int)(1 / frameTime)), 0, 0, 10, RED);
}

void UpdatePosition(float frameTime) {
  double moveSpeed = frameTime * 2.0f;
  double rotSpeed = frameTime * 0.75f;
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
      color = image_textures[floorTexture][texWidth * ty + tx];
      color.a /= 2;
      screen_buffer[y * screenWidth + x] = color;

      // Ceiling
      color = image_textures[ceilTexture][texWidth * ty + tx];
      color.a /= 2;
      screen_buffer[(screenHeight - y - 1) * screenWidth + x] = color;
    }
  }
}

void RenderWalls() {
  for (int x = 0; x < screenWidth; x++) {
    // The x coordinate in camera space
    double cameraX = 2 * x / (double)screenWidth - 1;

    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;

    // Which box of the map we are currently in.
    int mapX = (int)posX;
    int mapY = (int)posY;

    // Length of the ray from the current position to the next x-y side
    Vector2 sideDist;

    // Distance from one x-y side to the next x-y side
    Vector2 deltaDist = {
        .x = (rayDirX == 0) ? INFINITY : fabs(1 / rayDirX),
        .y = (rayDirY == 0) ? INFINITY : fabs(1 / rayDirY),
    };

    // What direction to step in each axis
    Vector2 step;

    if (rayDirX < 0) {
      step.x = -1;
      sideDist.x = (posX - mapX) * deltaDist.x;
    } else {
      step.x = 1;
      sideDist.x = (mapX + 1.0 - posX) * deltaDist.x;
    }

    if (rayDirY < 0) {
      step.y = -1;
      sideDist.y = (posY - mapY) * deltaDist.y;
    } else {
      step.y = 1;
      sideDist.y = (mapY + 1.0 - posY) * deltaDist.y;
    }

    bool hit = false;
    int side; // North-South or East-West

    // Perform DDA
    while (!hit) {
      // Jump to next square, either in the x or y direction
      if (sideDist.x < sideDist.y) {
        sideDist.x += deltaDist.x;
        mapX += (int)step.x;
        side = 0;
      } else {
        sideDist.y += deltaDist.y;
        mapY += (int)step.y;
        side = 1;
      }

      hit = (worldMap[mapX][mapY] > 0);
    }

    double perpendicularWallDistance;
    if (side == 0) { // Hit on x axis
      perpendicularWallDistance = (sideDist.x - deltaDist.x);
    } else { // Hit on Y axis
      perpendicularWallDistance = (sideDist.y - deltaDist.y);
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

      screen_buffer[y * screenWidth + x] = color;
    }
  }
}

Texture2D LoadInitialFrame() {
  Image initial_frame = {
      .data = screen_buffer,
      .width = screenWidth,
      .height = screenHeight,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };
  return LoadTextureFromImage(initial_frame);
}

int main(void) {
  InitWindow(screenWidth, screenHeight, "Raycaster");
  SetTargetFPS(60);

  LoadImageTextures();

  Texture2D frame = LoadInitialFrame();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);

    RenderFloorAndCeiling();
    RenderWalls();

    DrawFrame(frame);

    double frameTime = GetFrameTime();

#if DEBUG
    DrawFrameCounter(frameTime);
#endif

    EndDrawing();

    UpdatePosition(frameTime);
  }

  UnloadColors();
  UnloadTexture(frame);

  CloseWindow();

  return 0;
}
