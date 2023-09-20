#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "actors.h"
#include "doors.h"
#include "game.h"
#include "gun.h"
#include "movement.h"
#include "raycast.h"


// https://www.reddit.com/r/raylib/comments/hcglzh/comment/g212jbl/?utm_source=share&utm_medium=web2x&context=3
void DrawFrame(Texture2D frame_tex) {
  UpdateTexture(frame_tex, game.screen_buffer);
  DrawTexture(frame_tex, 0, 0, WHITE);
}

void DrawFrameCounter(float frameTime) {
  DrawText(TextFormat("%d", (int)(1 / frameTime)), 0, 0, 10, RED);
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
    // TODO: implement fire rate
    if (game.pistol_frame <= 0) {
      int r = Shoot();
      if (r >= 0) {
        KillActor(r);
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

int main(void) {
#if DEBUG
  assert(TEXTURE_COUNT == NUM_IMAGES);
#endif

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

    CastFloorAndCeiling();
    CastWalls();
    RenderActors();
    RenderGun();

    DrawFrame(frame);

    frameTime = GetFrameTime();

#if DEBUG
    DrawFrameCounter(frameTime);
#endif

    EndDrawing();

    UpdatePosition(frameTime);
    Interact(frameTime);
    MoveDoors(frameTime);
    DoActors(frameTime);
    AnimateGun();
  }

  UnloadTexture(frame);
  EndGame();

  CloseAudioDevice();
  CloseWindow();

  return 0;
}
