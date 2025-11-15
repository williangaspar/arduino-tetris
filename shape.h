#ifndef SHAPE_H
#define SHAPE_H

#include <Arduino.h>
#include "config.h"

class Entity {
public:
  int8_t x;
  int8_t y;
  Entity()
    : x(0), y(0){};
};

class Shape : public Entity {
public:
  int8_t color;

  Shape(uint16_t (&grid)[4])
    : grid(grid){};

  void setRotation(int8_t index);
  int8_t getRotation();
  void rotate();
  bool isCollidingWithLeftWall(int8_t leftWallx);
  bool isCollidingWithRightWall(int8_t rightWallx);
  uint16_t getShape();
  void addShapeToGrid(int8_t grid[BLOB_W][BLOB_H]);

private:
  uint16_t (&grid)[4];
  int8_t rotationIndex;
};

class Blob {
public:
  void reset();
  bool isCollidingWithShape(Shape *shape);
  void addShape(Shape *shape);
  int8_t eraseFilledLines();
  void squashBlob();
  int8_t getValue(int8_t x, int8_t y);

private:
  void pushLineDown(int8_t idx);
  int8_t grid[BLOB_W][BLOB_H];
  int8_t emptyLines[4] = {0};
  int8_t emptyLineIdx = 0;
};

#endif
