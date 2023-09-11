#include "game.h"

void OpenDoor(int x, int y) {
  DoorInfo door = game.doors[x][y];

  // TODO: Don't close the door if the player is on the tile
  switch (door.state) {
    case DOOR_CLOSED:
      PlaySound(game.door_sfx);
      game.doors[x][y].state = DOOR_OPENING;
      break;
    case DOOR_OPEN:
      PlaySound(game.door_sfx);
      game.doors[x][y].state = DOOR_CLOSING;
      break;
    default:
      break;
  }
}


void MoveDoors(float frameTime) {
  for (int y = 0; y < mapWidth; y++) {
    for (int x = 0; x < mapHeight; x++) {
      if (worldMap[x][y] == DOOR) {
        DoorInfo door = game.doors[x][y];

        if (door.state == DOOR_OPENING) { // Timer goes from 0 to 1.
          game.doors[x][y].timer += frameTime;
          if (game.doors[x][y].timer >= 1) {
            game.doors[x][y].state = DOOR_OPEN;
            game.doors[x][y].timer = 1;
          }
        } else if (door.state == DOOR_CLOSING) { // Timer goes from 1 to 0.
          game.doors[x][y].timer -= frameTime;
          if (game.doors[x][y].timer <= 0) {
            game.doors[x][y].state = DOOR_CLOSED;
            game.doors[x][y].timer = 0;
          }
        }
      }
    }
  }
}

