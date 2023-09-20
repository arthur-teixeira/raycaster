#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>
#include <stddef.h>

#define LIMIT_FPS 1
#define FULLSCREEN 0
#define DEBUG 1
#define MUTED 1

#if FULLSCREEN
#define screenWidth 1920
#define screenHeight 1080
#else
#define screenWidth 640
#define screenHeight 480
#endif

#define numActors 20
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

#define gunWidth 360
#define gunHeight 360

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

#define DOOR 12
#define DOOR_FRAME 13

#define NUM_IMAGES 23
#define NUM_PISTOL_FRAMES 5

#define LAMP 10

typedef enum {
  TEXTURE_EAGLE,
  TEXTURE_REDBRICK,
  TEXTURE_PURPLESTONE,
  TEXTURE_GREYSTONE,
  TEXTURE_BLUESTONE,
  TEXTURE_MOSSY,
  TEXTURE_WOOD,
  TEXTURE_COLORSTONE,
  TEXTURE_BARREL,
  TEXTURE_PILLAR,
  TEXTURE_GREENLIGHT,
  TEXTURE_DOOR,
  TEXTURE_DOORFRAME,
  TEXTURE_GUARD_STILL,
  TEXTURE_GUARD_W1,
  TEXTURE_GUARD_W2,
  TEXTURE_GUARD_W3,
  TEXTURE_GUARD_W4,
  TEXTURE_GUARD_D1,
  TEXTURE_GUARD_D2,
  TEXTURE_GUARD_D3,
  TEXTURE_GUARD_D4,
  TEXTURE_GUARD_D5,
  TEXTURE_COUNT,
} TextureIndex;

typedef enum {
  DOOR_CLOSED,
  DOOR_OPENING,
  DOOR_OPEN,
  DOOR_CLOSING,
} DoorState;

typedef struct {
  DoorState state;
  double timer;
} DoorInfo;

typedef enum {
  SIDE_WE,
  SIDE_NS,
} SideHit;

typedef enum {
  GUARD_STOPPED,
  GUARD_WALKING,
  GUARD_DYING,
  GUARD_DEAD,
} GuardState;

extern int worldMap[mapWidth][mapHeight];

// #define numGuards 1
// extern Guard guards[numGuards];

typedef struct {
  DoorInfo doors[mapWidth][mapHeight];
  Color screen_buffer[screenHeight * screenWidth];
  double ZBuffer[screenWidth];
  int spriteOrder[numActors];
  double spriteDistance[numActors];
  double posX, posY;
  double dirX, dirY;
  double planeX, planeY;
  Sound door_sfx;
  Sound pistol_sfx;
  char *images[NUM_IMAGES];
  Color *image_textures[NUM_IMAGES];
  Music soundtrack;
  Color *pistol_textures[NUM_PISTOL_FRAMES];
  int pistol_frame;
} Game;

extern Game game;

extern double posX;
extern double posY;
extern double dirX;
extern double dirY;
extern double planeX;
extern double planeY;

void InitGame();
void EndGame();

#endif // GLOBALS_H
