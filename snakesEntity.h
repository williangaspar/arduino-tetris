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
  Body()
      : array(&Blob::grid[0][0]), size(0) {}  // Converting the 2D grid into a 1D array. Why? 'cause we gangsta!
  void addToFrame(GGrid frame) override;
  void reset() override;
  bool addNewSegment(int8_t x, int8_t y);
  void move(int8_t x, int8_t y);
  bool isColidingWith(int8_t x, int8_t y);
  uint8_t getSize();

 private:
  uint8_t* array;
  static const int arraySize = BLOB_W * BLOB_H;
  int size;
};

class Snake : public LiveEntity {
 public:
  Direction direction;
  Snake()
      : direction(Direction::RIGHT), LiveEntity() {};
  bool move();
  bool eat(LiveEntity& food);
  bool getIsDigesting();
  void reset();
  void addToFrame(GGrid frame) override;
  uint8_t getSize();
  bool isColidingWith(int8_t x, int8_t y);

 private:
  Body body;
  bool isDigesting;
};

#endif  // SNAKES_ENTITY_H

// OBS: everybody is gangster until the segfault shows up.