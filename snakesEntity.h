#ifndef SNAKES_ENTITY_H
#define SNAKES_ENTITY_H

#include <Arduino.h>

#include "config.h"
#include "entity.h"

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Body : public Blob {
public:
  void addToFrame(GGrid frame) override;
  Body()
    : array(&Blob::grid[0][0]), tailIdx(0) {}  // Converting the 2D grid into a 1D array. Why? 'cause we gangsta!

private:
  int8_t* array;
  const static int8_t arraySize = BLOB_W * BLOB_H;
  int8_t tailIdx;
};

class Snake : public LiveEntity {
public:
  Direction direction;
  Snake()
    : direction(Direction::RIGHT), LiveEntity(){};
  void eat(LiveEntity& food);
  bool getIsDigesting();
  void finishDigestion();
  void addToFrame(GGrid frame) override;

private:
  Body body;
  bool isDigesting;
};

#endif  // SNAKES_ENTITY_H

// OBS: everybody is gangster until the segfault shows up.