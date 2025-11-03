#ifndef GAME_H
#define GAME_H

#include "shape.h"

namespace Game {

extern Shape shapes[SHAPE_COUNT];
extern int32_t score;

Shape* getRandomShape();
Blob& getBlob();
void start();
void gameOver();
void tick(Shape* shape);

}  // namespace Game

#endif