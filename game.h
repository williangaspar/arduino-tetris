#ifndef GAME_H
#define GAME_H

#include "shape.h"

namespace Game {

enum class Input {
  Up,
  Down,
  Left,
  Right,
  Pause,
  None
};

struct Events {
  bool isNewShape;
  bool isNewPoints;
  bool isGameOver;

  void reset();
};

Blob &getBlob();
Shape &getShape();
Shape &getNextShape();

Events &tick(Input userInput);

void start();
void gameOver();
int getScore();

}  // namespace Game

#endif