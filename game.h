#ifndef GAME_H
#define GAME_H

#include "shape.h"

namespace Game {

enum class Input {
  Up,
  Down,
  Left,
  Right,
  None
};

struct Events {
  bool isNewShape;
  bool isNewPoints;
  bool isGameOver;

  void reset();
};

Blob &getBlob();
Shape *getShape();
Shape *getNextShape();

Events &tick(Input &userInput);

void start();
void gameOver();
int32_t getScore();

}  // namespace Game

#endif