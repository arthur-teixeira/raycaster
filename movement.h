#ifndef MOVEMENT_H
#define MOVEMENT_H

void UpdatePosition(float frameTime);

void Move(double moveSpeed, signed char factor);

void MoveEx(double *posX, double *posY, double dirX, double dirY,
            double moveSpeed, signed char factor);

#endif // MOVEMENT_H
