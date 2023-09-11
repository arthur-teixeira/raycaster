#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>
#include <stddef.h>

#define LIMIT_FPS 1
#define FULLSCREEN 0
#define DEBUG 1
#define MUTED 0

#if FULLSCREEN
#define screenWidth 1920
#define screenHeight 1080
#else
#define screenWidth 640
#define screenHeight 480
#endif

#define numSprites 19
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

#define gunWidth 360
#define gunHeight 360

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

#define DOOR 12
#define DOOR_FRAME 13

#define NUM_IMAGES 13
#define NUM_PISTOL_FRAMES 5

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

typedef struct {
  double x;
  double y;
  int texture;
} Sprite;

typedef enum {
  SIDE_WE,
  SIDE_NS,
} SideHit;

extern int worldMap[mapWidth][mapHeight];
extern Sprite sprites[numSprites];

typedef struct {
} PistolTexture;

typedef struct {
  DoorInfo doors[mapWidth][mapHeight];
  Color screen_buffer[screenHeight * screenWidth];
  double ZBuffer[screenWidth];
  int spriteOrder[numSprites];
  double spriteDistance[numSprites];
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
