#ifndef ENTITY_H
#define ENTITY_H

#include <Arduino.h>
#include "config.h"

class Entity {
public:
  virtual void addToFrame(int8_t frame[BLOB_W][BLOB_H]) {};
};

class LiveEntity : public Entity {
public:
  int8_t x;
  int8_t y;
  int8_t color;
  LiveEntity()
    : x(0), y(0){};
};

class Blob : public Entity {
public:
  void reset();

protected:
  static int8_t grid[BLOB_W][BLOB_H];
};

#endif