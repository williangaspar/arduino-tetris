#ifndef TETRIS_H
#define TETRIS_H

#include "game.h"
#include "tetrisEntity.h"

class Tetris : public Game {
public:
  Tetris(int8_t id, const char* name)  // I shouldn't have to do this. It should just grab the parent's constructor.
    : Game(id, name) {}                // But cpp stupid, so here we are.

  void start() override;
  void addEntitiesToFrame(int8_t frame[BLOB_W][BLOB_H]) override;
  Events& tick(Input userInput) override;
  static uint16_t getNextShape();
  static int8_t getNextColor();
};

#endif