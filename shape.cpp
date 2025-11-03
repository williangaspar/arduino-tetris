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
      if (bitRead(this->grid[this->rotationIndex], i + j)) {
        minx = i;
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
  return shape->y >= 9;
}

void Blob::addShape(Shape *shape) {
  shape->addShapeToGrid(this->grid);
}

int8_t Blob::getValue(int x, int y) {
  return this->grid[x][y];
}
