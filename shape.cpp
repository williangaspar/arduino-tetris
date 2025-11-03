#include "shape.h"

#include <Arduino.h>

void Shape::rotate() {
  this->setRotationIndex(this->rotationIndex + 1);
}

void Shape::setRotationIndex(int8_t index) {
  this->rotationIndex = index % 4;
}

void Shape::moveLeft() {
  int minx = 0;
  for (int i = 0; i < this->rowSize; i++) {
    for (int j = 0; j < this->squareSize; j += this->rowSize) {
      if (bitRead(this->shape[this->rotationIndex], i + j)) {
        minx =  i;
        break;
      }
    }
  }

  if (minx > 0) { this->x--; }
}

void Shape::moveRight() {
  this->x++;
}

void Shape::moveDown() {
  this->y++;
}

uint16_t Shape::getShape() {
  return this->shape[this->rotationIndex];
}

void Blob::reset() {
  for (int x = 0; x < BLOB_W; x++) {
    for (int y = 0; y < BLOB_H; y++) {
      this->grid[x][y] = 0;
    }
  }
}


bool Blob::collisionCheck(Shape *shape) {
  return false;
}

void Blob::addShape(Shape *shape) {
  //
}

int8_t Blob::getValue(int x, int y) {
  return this->grid[x][y];
}
