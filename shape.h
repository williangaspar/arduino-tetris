#ifndef SHAPE_H
#define SHAPE_H

#include <Arduino.h>
#include "config.h"

class Entity {
public:
  int x;
  int y;
  Entity()
    : x(0), y(0){};
};

class Shape : public Entity {
public:
  int8_t color;

  Shape(uint16_t (&grid)[4])
    : grid(grid){};

  void setRotation(int8_t index);
  int getRotation();
  void rotate();
  bool isCollidingWithLeftWall(int leftWallx);
  bool isCollidingWithRightWall(int rightWallx);
  uint16_t getShape();
  void addShapeToGrid(int8_t grid[BLOB_W][BLOB_H]);

private:
  uint16_t (&grid)[4];
  int8_t rotationIndex;
};

class Blob : public Entity {
public:
  void reset();
  bool isCollidingWithShape(Shape *shape);
  void addShape(Shape *shape);
  int eraseFilledLines();
  void squashBlob();
  int8_t getValue(int x, int y);

private:
  void pushLineDown(int idx);
  int8_t grid[BLOB_W][BLOB_H];
};

#endif
