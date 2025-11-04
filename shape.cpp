#include "shape.h"

#include <Arduino.h>

bool Shape::rotate() {
  if (this->x >= 0 && (this->x + this->rowSize) <= BLOB_W) {
    this->setRotationIndex(this->rotationIndex + 1);
    return true;
  }

  return false;
}

void Shape::setRotationIndex(int8_t index) {
  this->rotationIndex = index % 4;
}

bool Shape::moveLeft() {
  int minx = this->x;
  uint16_t shape = this->getShape();
  uint16_t mask = this->rowSize == 3 ? L_MSK_3X3 : L_MSK_4X4;

  if ((shape & mask) == 0) { minx++; };
  if (minx > 0) {
    this->x--;
    return true;
  } 
  
  return false;
}

bool Shape::moveRight() {
  int maxx = this->x + this->rowSize;
  uint16_t shape = this->getShape();
  uint16_t mask = this->rowSize == 3 ? R_MSK_3X3 : R_MSK_4X4;

  if ((shape & mask) == 0) { maxx--; };
  if (maxx < BLOB_W) {
    this->x++;
    return true;
  } 
  
  return false;
}

bool Shape::moveDown() {
  this->y++;
  return true;
}

uint16_t Shape::getShape(int8_t grid[BLOB_W][BLOB_H]) {
  return this->grid[this->rotationIndex];
}

void Shape::addShapeToGrid(int8_t grid[BLOB_W][BLOB_H]) {
  int posx = this->x;
  int posy = this->y;
  uint16_t shape = this->getShape();

  for (int i = 0; i < this->squareSize; i++) {
    posx = this->x + (i % this->rowSize);
    if (posx == this->x) posy++;

    if (bitRead(shape, this->squareSize - i - 1)) {
      if ((posx >= 0 && posx < BLOB_W) && (posy >= 0 && posy < BLOB_H)) {
        grid[posx][posy] = this->color;
      }
    }
  }
}

void Blob::reset() {
  for (int x = 0; x < BLOB_W; x++) {
    for (int y = 0; y < BLOB_H; y++) {
      this->grid[x][y] = 0;
    }
  }
}


bool Blob::collisionCheck(Shape *shape) {
  /*
    from the shape bottom to the top,
    check if there is some right underneath 
    in the blob
  */
  return shape->y >= 9;
}

void Blob::addShape(Shape *shape) {
  shape->addShapeToGrid(this->grid);
}

int8_t Blob::getValue(int x, int y) {
  return this->grid[x][y];
}
