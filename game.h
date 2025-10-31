#ifndef GAME_H
#define GAME_H

#include "shape.h"

#define SHAPE_LIST_SIZE 6

namespace Game {

extern Shape shapes[SHAPE_LIST_SIZE];
extern int32_t score;

Shape* getRandomShape();
void start();
void gameOver();

}  // namespace Game

#endif