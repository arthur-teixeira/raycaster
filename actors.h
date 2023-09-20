#ifndef SPRITES_H
#define SPRITES_H

#include "game.h"

typedef struct ActorState {
  // a negative frame duration means that the actor is static
  float frameDuration;
  TextureIndex texture;
  void (*think)();
  void (*action)();
  struct ActorState *next;
} ActorState;

typedef struct {
  float x;
  float y;
  ActorState *state;
  float dirX;
  float dirY;
  float frameTime;
} Actor;

extern Actor actors[numActors];

void RenderActors();
// void MoveActors(float frameTime);
void DoActors(float frameTime);
void KillActor(int i);

#endif // SPRITES_H
