#ifndef SNAKES_ENTITY_H
#define SNAKES_ENTITY_H

#include <Arduino.h>

#include "config.h"
#include "entity.h"

struct Position {
  int8_t x;
  int8_t y;
};

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Body : public Blob {
public:
  Position begin;
  Position end;  // We could call this tail, but calling begin a "head" would be confusing.
private:
  int8_t (*array)[];
};

class Snake : public LiveEntity {
public:
  Direction direction;
  void eat(LiveEntity &food);
  bool getIsDigesting();
  void finishDigestion();
  Snake()
    : direction(Direction::RIGHT), LiveEntity() {};

private:
  Body body;
  bool isDigesting;
};

#endif  // SNAKES_ENTITY_H