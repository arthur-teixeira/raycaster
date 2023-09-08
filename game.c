#include "game.h"
#include <raylib.h>
#include <stddef.h>

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

Sprite sprites[numSprites] = {
    {20.5, 11.5, 10}, // green light in front of playerstart
    // green lights in every room
    {18.5, 4.5, 10},
    {10.0, 4.5, 10},
    {10.0, 12.5, 10},
    {3.5, 6.5, 10},
    {3.5, 20.5, 10},
    {3.5, 14.5, 10},
    {14.5, 20.5, 10},

    // row of pillars in front of wall: fisheye test
    {18.5, 10.5, 9},
    {18.5, 11.5, 9},
    {18.5, 12.5, 9},

    // some barrels around the map
    {21.5, 1.5, 8},
    {15.5, 1.5, 8},
    {16.0, 1.8, 8},
    {16.2, 1.2, 8},
    {3.5, 2.5, 8},
    {9.5, 15.5, 8},
    {10.0, 15.1, 8},
    {10.5, 15.8, 8},
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
        },
    .image_textures = {0},
};

double posX = 1.6;
double posY = 1.5;
double dirX = -1;
double dirY = 0;
double planeX = 0;
double planeY = 0.66f;

void LoadImageTextures() {
  for (size_t i = 0; i < NUM_IMAGES; i++) {
    Image img = LoadImage(game.images[i]);
    game.image_textures[i] = LoadImageColors(img);
    UnloadImage(img);
  }
}

void InitializeDoors() {
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

void InitGame() {
  LoadImageTextures();
  InitializeDoors();
  game.door_sfx = LoadSound("./assets/sound-effects/Door.wav");
#if !MUTED
  game.soundtrack = LoadMusicStream("./assets/soundtrack/ost.wav");
  PlayMusicStream(game.soundtrack);
#endif

}

void UnloadColors() {
  for (size_t i = 0; i < NUM_IMAGES; i++) {
    UnloadImageColors(game.image_textures[i]);
  }
}

void EndGame() {
  UnloadColors();
#if !MUTED
  UnloadMusicStream(game.soundtrack);
#endif
  UnloadSound(game.door_sfx);

}

