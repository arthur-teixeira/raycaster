#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#include "actors.h"
#include "game.h"
#include "movement.h"

ActorState greenlight = {
    .texture = TEXTURE_GREENLIGHT,
    .frameDuration = -1,
};

ActorState pillar = {
    .texture = TEXTURE_PILLAR,
    .frameDuration = -1,
};

ActorState barrel = {
    .texture = TEXTURE_BARREL,
    .frameDuration = -1,
};

ActorState guard_still;
ActorState guard_walking1;
ActorState guard_walking2;
ActorState guard_walking3;
ActorState guard_walking4;

ActorState guard_dying1;
ActorState guard_dying2;
ActorState guard_dying3;
ActorState guard_dying4;
ActorState guard_dead;

ActorState guard_still = {
    .texture = TEXTURE_GUARD_STILL,
    .frameDuration = 1, 
    .next = &guard_walking1,
};

ActorState guard_walking1 = {
    .texture = TEXTURE_GUARD_W1,
    .frameDuration = 0.2,
    .next = &guard_walking2,
    .think = Path,
};

ActorState guard_walking2 = {
    .texture = TEXTURE_GUARD_W2,
    .frameDuration = 0.2,
    .next = &guard_walking3,
    .think = Path,
};

ActorState guard_walking3 = {
    .texture = TEXTURE_GUARD_W3,
    .frameDuration = 0.2,
    .next = &guard_walking4,
    .think = Path,
};

ActorState guard_walking4 = {
    .texture = TEXTURE_GUARD_W4,
    .frameDuration = 0.2,
    .next = &guard_walking1,
    .think = Path,
};

ActorState guard_dying1 = {
  .texture = TEXTURE_GUARD_D1,
  .frameDuration = 0.2 / 3,
  .next = &guard_dying2,
};

ActorState guard_dying2 = {
  .texture = TEXTURE_GUARD_D2,
  .frameDuration = 0.2 / 3,
  .next = &guard_dying3,
};

ActorState guard_dying3 = {
  .texture = TEXTURE_GUARD_D3,
  .frameDuration = 0.2 / 3,
  .next = &guard_dying4,
};

ActorState guard_dying4 = {
  .texture = TEXTURE_GUARD_D4,
  .frameDuration = 0.2 / 3,
  .next = &guard_dead,
};

ActorState guard_dead = {
  .texture = TEXTURE_GUARD_D5,
  .frameDuration = -1,
};

Actor actors[numActors] = {
    // green light in front of playerstart
    {20.5, 11.5, &greenlight, 0, 0, 0},
    // green lights in every room
    {18.5, 4.5, &greenlight, 0, 0, 0},
    {10.0, 4.5, &greenlight, 0, 0, 0},
    {10.0, 12.5, &greenlight, 0, 0, 0},
    {3.5, 6.5, &greenlight, 0, 0, 0},
    {3.5, 20.5, &greenlight, 0, 0, 0},
    {3.5, 14.5, &greenlight, 0, 0, 0},
    {14.5, 20.5, &greenlight, 0, 0, 0},

    // row of pillars in front of wall: fisheye test
    {18.5, 10.5, &pillar, 0, 0, 0},
    {18.5, 11.5, &pillar, 0, 0, 0},
    {18.5, 12.5, &pillar, 0, 0, 0},

    // some barrels around the map
    {21.5, 1.5, &barrel, 0, 0, 0},
    {15.5, 1.5, &barrel, 0, 0, 0},
    {16.0, 1.8, &barrel, 0, 0, 0},
    {16.2, 1.2, &barrel, 0, 0, 0},
    {9.5, 15.5, &barrel, 0, 0, 0},
    {10.0, 15.1, &barrel, 0, 0, 0},
    {10.5, 15.8, &barrel, 0, 0, 0},

    // actors
    {3.5, 2.5, &guard_still, 1, 1, 0},
    {4.5, 2.5, &guard_still, 0.1, 0, 0},
};

// PRIVATE METHODS

static int partition(int *order, double *dist, int p, int r) {
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

static void quicksort(int *order, double *dist, int p, int r) {
  if (p < r) {
    int q = partition(order, dist, p, r);
    quicksort(order, dist, p, q);
    quicksort(order, dist, q + 1, r);
  }
}

static void sortSprites(int *order, double *dist, int amount) {
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

void DoActor(Actor *actor, float frameTime) {
  void (*think)(Actor *);

  if (actor->state->frameDuration < 0) { // Static actor
    return;
  }

  actor->frameTime -= frameTime;
  think = actor->state->think;


  if (actor->frameTime <= 0) {
    printf("should think\n");
    if (actor->state->think) {
      think(actor);
    }

    actor->state = actor->state->next;
    actor->frameTime = actor->state->frameDuration;
  }
}

// PUBLIC METHODS

void RenderActors() {
  for (int i = 0; i < numActors; i++) {
    game.spriteOrder[i] = i;
    double xComponent = posX - actors[i].x;
    double yComponent = posY - actors[i].y;
    game.spriteDistance[i] =
        (xComponent * xComponent) + (yComponent * yComponent);
  }

  // TODO: change sorting to insertion sort. Although quicksort is generally
  // faster, this array is small and it will already be sorted most of the
  // times, so quicksort doesn't make much sense here.
  sortSprites(game.spriteOrder, game.spriteDistance, numActors);

  for (int i = 0; i < numActors; i++) {
    Actor s = actors[game.spriteOrder[i]];

    // Translate position to relative to camera
    Vector2 sprite = {
        .x = s.x - posX,
        .y = s.y - posY,
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
        (int)(((float)screenWidth / 2) * (1 + transform.x / transform.y));

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
              game.image_textures[actors[game.spriteOrder[i]].state->texture]
                                 [texWidth * texY + texX];
          if (color.a > 0) {
            game.screen_buffer[y * screenWidth + stripe] = color;
          }
        }
      }
    }
  }
}

void DoActors(float frameTime) {
  for (int i = 0; i < numActors; i++) {
    DoActor(&actors[i], frameTime);
  }
}

void KillActor(int i) {
  Actor *g = &actors[i];

  g->state = &guard_dying1;
  g->frameTime = g->state->frameDuration;
}

void Path(Actor *actor) {
  // TODO: make walking smoother and choose path correctly
  MoveEx(&actor->x, &actor->y, actor->dirX, actor->dirY, frameTime * 5, 1);
}
