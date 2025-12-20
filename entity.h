#ifndef ENTITY_H
#define ENTITY_H

#include <Arduino.h>

#include "config.h"

class Entity {
 public:
  virtual void addToFrame(GGrid frame) {};
};

class LiveEntity : public Entity {
 public:
  int8_t x;
  int8_t y;
  int8_t color;
  LiveEntity()
      : x(0), y(0) {};
};

class Blob : public Entity {
 public:
  virtual void reset();

 protected:
  static GGrid grid;
};

#endif