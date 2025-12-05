#ifndef TETRIS_H
#define TETRIS_H

#include "game.h"
#include "tetrisEntity.h"

class Tetris : public Game {
public:
  static void start();
  static void addEntitiesToFrame(int8_t frame[BLOB_W][BLOB_H]);
  static uint16_t getNextShape();
  static int8_t getNextColor();
  static Events& tick(Input userInput);
};

#endif