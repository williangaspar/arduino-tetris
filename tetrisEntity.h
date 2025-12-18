#ifndef TETRIS_ENTITY_H
#define TETRIS_ENTITY_H

#include <Arduino.h>

#include "config.h"
#include "entity.h"

class Shape : public LiveEntity {
public:
  Shape(uint16_t (*grid)[4])
    : grid(grid){};

  void setRotation(int8_t index);
  int8_t getRotation();
  void rotate();
  bool isCollidingWithLeftWall(int8_t leftWallx);
  bool isCollidingWithRightWall(int8_t rightWallx);
  uint16_t getShape();
  void replaceGrid(uint16_t (*newGrid)[4]);
  void copyShape(Shape& shape);
  void addToFrame(GGrid frame) override;

private:
  uint16_t (*grid)[4];
  int8_t rotationIndex;
};

class TetrisBlob : public Blob {
public:
  void addToFrame(GGrid frame) override;
  bool isCollidingWithShape(Shape& shape);
  void addShape(Shape& shape);
  int8_t squash();
  int8_t getValue(int8_t x, int8_t y);

private:
  void pushLineDown(int8_t idx);
};

#endif
