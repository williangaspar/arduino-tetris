#ifndef GAME_H
#define GAME_H

#include "shape.h"

namespace Game {

struct UserInput {
  bool left;
  bool right;
  bool up;
  bool down;

  void reset();
};

Blob& getBlob();
Shape *getShape();
void start();
void gameOver();
void tick(UserInput &userInput, void (*pointsCallback)());
int32_t getGameScore();

}  // namespace Game

#endif