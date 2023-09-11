#include "game.h"

void RenderGun() {
  // TODO: scale sprite by screen height
  for (int y = 0; y < gunHeight; y++) {
    for (int x = 0; x < gunWidth; x++) {

      int offset_x = (screenWidth - gunWidth) / 2;
      int offset_y = (screenHeight - gunHeight);

      Color pixel = game.pistol_textures[game.pistol_frame / 2][y * gunWidth + x];

      if (pixel.a > 0) {
        game.screen_buffer[(offset_y + y) * screenWidth + (offset_x + x)] =
            pixel;
      }
    }
  }
}

void AnimateGun() {
  if (game.pistol_frame > 0) {
    game.pistol_frame = (game.pistol_frame + 1) % (NUM_PISTOL_FRAMES * 2);
  }
}

