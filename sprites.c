#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#include "game.h"

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

// PUBLIC METHODS

void RenderSprites() {
  for (int i = 0; i < numSprites; i++) {
    game.spriteOrder[i] = i;
    double xComponent = posX - sprites[i].x;
    double yComponent = posY - sprites[i].y;
    game.spriteDistance[i] =
        (xComponent * xComponent) + (yComponent * yComponent);
  }

  // TODO: change sorting to insertion sort. Although quicksort is generally
  // faster, this array is small and it will already be sorted most of the
  // times, so quicksort doesn't make much sense here.
  sortSprites(game.spriteOrder, game.spriteDistance, numSprites);

  for (int i = 0; i < numSprites; i++) {
    Sprite s = sprites[game.spriteOrder[i]];

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
              game.image_textures[sprites[game.spriteOrder[i]].texture]
                                 [texWidth * texY + texX];
          if (color.a > 0) {
            game.screen_buffer[y * screenWidth + stripe] = color;
          }
        }
      }
    }
  }
}
