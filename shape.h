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
  int8_t const rowSize;
  int8_t const squareSize;

  Shape(uint16_t (&grid)[4], int8_t rowSize)
    : grid(grid), rowSize(rowSize), squareSize(rowSize * rowSize){};

  void setRotationIndex(int8_t index);
  void rotate();
  void moveLeft();
  void moveRight();
  void moveDown();

  uint16_t getShape();
  void addShapeToGrid(int8_t grid[BLOB_W][BLOB_H]);

private:
  uint16_t (&grid)[4];
  int8_t rotationIndex;
};

class Blob : public Entity {
public:
  void reset();
  bool collisionCheck(Shape *shape);
  void addShape(Shape *shape);
  int pointsCheck();
  int8_t getValue(int x, int y);

private:
  int8_t grid[BLOB_W][BLOB_H];
};

#endif
