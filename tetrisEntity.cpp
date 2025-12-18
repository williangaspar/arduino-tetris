#include "tetrisEntity.h"

#include <Arduino.h>

void Shape::rotate() {
  this->setRotation(this->rotationIndex + 1);
}

void Shape::setRotation(int8_t index) {
  this->rotationIndex = index % 4;
}

int8_t Shape::getRotation() {
  return this->rotationIndex;
}

uint16_t Shape::getShape() {
  // (*this->grid) gives the pointer to the array, [this->rotationIndex] get an index in that array.
  return (*this->grid)[this->rotationIndex];
}

bool Shape::isCollidingWithLeftWall(int8_t leftWallx) {
  int8_t minx = this->x;
  uint16_t shape = this->getShape();

  if ((shape & L_MSK_C1) == 0) minx++;
  return minx < leftWallx;
}

bool Shape::isCollidingWithRightWall(int8_t rightWallx) {
  int8_t maxx = this->x + 2;
  uint16_t shape = this->getShape();

  if ((shape & R_MSK_C1) == 0) maxx--;
  if ((shape & R_MSK_C2) == 0) maxx--;
  return maxx >= (rightWallx - 1);
}

void Shape::addToFrame(GGrid frame) {
  int8_t posx = this->x;
  int8_t posy = this->y;
  uint16_t shape = this->getShape();

  for (int8_t i = 0; i < ROW_SQR_SIZE; i++) {
    posx = this->x + (i % ROW_SIZE);
    if (posx == this->x) posy++;

    if (bitRead(shape, ROW_SQR_SIZE - i - 1)) {
      if ((posx >= 0 && posx < BLOB_W) && (posy >= 0 && posy < BLOB_H)) {
        frame[posx][posy] = this->color;
      }
    }
  }
}

void Shape::replaceGrid(uint16_t (*newGrid)[4]) {
  this->grid = newGrid;
}

void Shape::copyShape(Shape& shape) {
  memcpy(this, &shape, sizeof(Shape));
}

void TetrisBlob::addToFrame(GGrid frame) {
  for (int x = 0; x < BLOB_W; x++) {
    for (int y = 0; y < BLOB_H; y++) {
      frame[x][y] = this->getValue(x, y);
    }
  }
}

bool TetrisBlob::isCollidingWithShape(Shape& shape) {
  int8_t posx = shape.x;
  int8_t posy = shape.y;
  uint16_t cshape = shape.getShape();

  for (int8_t i = 0; i < ROW_SQR_SIZE; i++) {
    posx = shape.x + (i % ROW_SIZE);
    if (posx == shape.x) posy++;

    if (bitRead(cshape, ROW_SQR_SIZE - i - 1)) {
      // This should catch any overlap: top, bottom, left or right.
      // If the shape is touching the blob, this should trigger.
      // When that happens, the caller should undo the last move.
      if (posy >= 0 && Blob::grid[posx][posy] > 0 || posy >= BLOB_H) {
        return true;
      }
    }
  }

  return false;
}

void TetrisBlob::addShape(Shape& shape) {
  shape.addToFrame(Blob::grid);
}

int8_t TetrisBlob::getValue(int8_t x, int8_t y) {
  return Blob::grid[x][y];
}

int8_t TetrisBlob::squash() {
  int8_t totalLinesRemoved = 0;
  for (int8_t i = 0; i < BLOB_H; i++) {
    bool shouldRemove = true;
    for (int8_t j = 0; j < BLOB_W; j++) {
      if (Blob::grid[j][i] == 0) {
        shouldRemove = false;
        break;
      };
    }
    if (shouldRemove) {
      memset(Blob::grid[i], 0, (size_t)BLOB_W * sizeof(Blob::grid[0][0]));
      this->pushLineDown(i);
      totalLinesRemoved++;
    };
  };

  return totalLinesRemoved;
}

void TetrisBlob::pushLineDown(int8_t idx) {
  for (int8_t i = idx; i > 0; i--) {
    for (int8_t j = 0; j < BLOB_W; j++) {
      Blob::grid[j][i] = Blob::grid[j][i - 1];
    }
  }
}
