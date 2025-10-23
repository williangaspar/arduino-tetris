#ifndef SHAPE_H
#define SHAPE_H

#include <Arduino.h>

#define SQUARE_LENGHT 50

class Entity {
public:
  int x;
  int y;
  virtual void draw() = 0;
};

template<int W, int H>
class Blob : public Entity {
public:
  constexpr int getWidth() const {
    return W;
  }
  constexpr int getHeight() const {
    return H;
  }

private:
  // int8_t& grid[H][W];
};

class Shape : public Entity {
public:
  uint16_t color;

  Shape(uint16_t (&shape)[4], int8_t rowSize)
    : shape(shape), rowSize(rowSize){};
  void draw() override;
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

#endif
