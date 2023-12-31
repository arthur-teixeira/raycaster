#include "game.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

int worldMap[mapWidth][mapHeight] = {
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {8, 0, 0, 0, 1, 12, 2, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {8, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {8, 0, 0, 0, 2, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
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

Game game = {
    .doors = {0},
    .screen_buffer = {0},
    .ZBuffer = {0},
    .spriteOrder = {0},
    .spriteDistance = {0},
    .images =
        {
            "./assets/eagle.png",
            "./assets/redbrick.png",
            "./assets/purplestone.png",
            "./assets/greystone.png",
            "./assets/bluestone.png",
            "./assets/mossy.png",
            "./assets/wood.png",
            "./assets/colorstone.png",
            "./assets/barrel.png",
            "./assets/pillar.png",
            "./assets/greenlight.png",
            "./assets/door.png",
            "./assets/doorframe.png",
            "./assets/guard/walking/frame1.png",
            "./assets/guard/walking/frame2.png",
            "./assets/guard/walking/frame3.png",
            "./assets/guard/walking/frame4.png",
            "./assets/guard/walking/frame5.png",
            "./assets/guard/dying/frame1.png",
            "./assets/guard/dying/frame2.png",
            "./assets/guard/dying/frame3.png",
            "./assets/guard/dying/frame4.png",
            "./assets/guard/dead.png",
        },
    .image_textures = {0},
};

double posX = 1.6;
double posY = 1.5;
double dirX = -1;
double dirY = 0;
double planeX = 0;
double planeY = 0.66f;

double frameTime = 0;

// PRIVATE METHODS

static void LoadImageTextures() {
  for (size_t i = 0; i < NUM_IMAGES; i++) {
    Image img = LoadImage(game.images[i]);
    game.image_textures[i] = LoadImageColors(img);
    UnloadImage(img);
  }
}

static void InitializeDoors() {
  for (int y = 0; y < mapWidth; y++) {
    for (int x = 0; x < mapHeight; x++) {
      if (worldMap[x][y] == DOOR) {
        game.doors[x][y] = (DoorInfo){
            .state = DOOR_CLOSED,
            .timer = 0,
        };
      }
    }
  }
}

static void LoadPistolTextures() {
  for (int i = 0; i < NUM_PISTOL_FRAMES; i++) {
    char path[100];
    sprintf(path, "./assets/pistol/frame%d.png", i + 1);
    Image img = LoadImage(path);
    game.pistol_textures[i] = LoadImageColors(img);
    UnloadImage(img);
  }
}

static void UnloadColors(Color **c, size_t n) {
  for (size_t i = 0; i < n; i++) {
    UnloadImageColors(c[i]);
  }
}

// PUBLIC METHODS

void InitGame() {
  LoadImageTextures();
  InitializeDoors();
  LoadPistolTextures();
  game.door_sfx = LoadSound("./assets/sound-effects/Door.wav");
  game.pistol_sfx = LoadSound("./assets/sound-effects/Pistol.wav");
  game.pistol_frame = 0;
#if !MUTED
  game.soundtrack = LoadMusicStream("./assets/soundtrack/ost.wav");
  PlayMusicStream(game.soundtrack);
#endif
}

void EndGame() {
  UnloadColors(game.image_textures, NUM_IMAGES);
  UnloadColors(game.pistol_textures, NUM_PISTOL_FRAMES);
#if !MUTED
  UnloadMusicStream(game.soundtrack);
#endif
  UnloadSound(game.door_sfx);
  UnloadSound(game.pistol_sfx);
}
