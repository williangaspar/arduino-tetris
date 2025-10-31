#ifndef SHAPE_H
#define SHAPE_H

#include <Arduino.h>

#define SQR_SIZE 12
#define SQR_GAP 2

class Entity {
public:
  int x;
  int y;
  void draw() {};
};

class Shape : public Entity {
public:
  uint16_t color;

  Shape(uint16_t (&shape)[4], int8_t rowSize)
    : shape(shape), rowSize(rowSize){};
  void draw();
  void rotate();
  void setRotationIndex(int8_t index);
  void moveLeft();
  void moveRight();
  void moveDown();

private:
  uint16_t (&shape)[4];
  int8_t rotationIndex;
  int8_t rowSize;
};

class Blob : public Entity {
public:
  void reset();
  bool collisionCheck(Shape &shape);
  void addShape(Shape &shape);
  int pointsCheck();

private:
  // int8_t& grid[H][W];
};

#endif
