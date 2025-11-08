#include "shape.h"

#include <Arduino.h>

void Shape::rotate() {
  this->setRotation(this->rotationIndex + 1);
}

void Shape::setRotation(int8_t index) {
  this->rotationIndex = index % 4;
}

int Shape::getRotation() {
  return this->rotationIndex;
}

uint16_t Shape::getShape() {
  return this->grid[this->rotationIndex];
}

bool Shape::isCollidingWithLeftWall(int leftWallx) {
  int minx = this->x;
  uint16_t shape = this->getShape();

  if ((shape & L_MSK_C1) == 0) { minx++; };
  return minx < leftWallx;
}

bool Shape::isCollidingWithRightWall(int rightWallx) {
  int maxx = this->x + 2;
  uint16_t shape = this->getShape();

  if ((shape & R_MSK_C1) == 0) { maxx--; };
  if ((shape & R_MSK_C2) == 0) { maxx--; };
  return maxx >= (rightWallx - 1);
}

void Shape::addShapeToGrid(int8_t grid[BLOB_W][BLOB_H]) {
  int posx = this->x;
  int posy = this->y;
  uint16_t shape = this->getShape();

  for (int i = 0; i < ROW_SQR_SIZE; i++) {
    posx = this->x + (i % ROW_SIZE);
    if (posx == this->x) posy++;

    if (bitRead(shape, ROW_SQR_SIZE - i - 1)) {
      if ((posx >= 0 && posx < BLOB_W) && (posy >= 0 && posy < BLOB_H)) {
        grid[posx][posy] = this->color;
      }
    }
  }
}

void Blob::reset() {
  memset(this->grid, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(this->grid[0][0]));
}

bool Blob::isCollidingWithShape(Shape *shape) {
  int posx = shape->x;
  int posy = shape->y;
  uint16_t cshape = shape->getShape();

  if (shape->y < 0) {
    return false;
  }

  for (int i = 0; i < ROW_SQR_SIZE; i++) {
    posx = shape->x + (i % ROW_SIZE);
    if (posx == shape->x) posy++;

    if (bitRead(cshape, ROW_SQR_SIZE - i - 1)) {
      // This should catch any overlap: top, bottom, left or right.
      // If the shape is touching the blob, this should trigger.
      // When that happens, the caller should undo the last move.
      if (this->grid[posx][posy] > 0 || posy >= BLOB_H) {
        return true;
      }
    }
  }

  return false;
}

void Blob::addShape(Shape *shape) {
  shape->addShapeToGrid(this->grid);
}

int8_t Blob::getValue(int x, int y) {
  return this->grid[x][y];
}

int Blob::eraseFilledLines() {
  int totalRemoved = 0;
  for (int i = 0; i < BLOB_H; i++) {
    bool shouldRemove = true;
    for (int j = 0; j < BLOB_W; j++) {
      if (this->grid[j][i] == 0) {
        shouldRemove = false;
        break;
      };
    }
    if (shouldRemove) {
      for (int j = 0; j < BLOB_W; j++) {
        this->grid[j][i] = 0;
      }
      this->emptyLines[this->emptyLineIdx++] = i;
      totalRemoved++;
    };
  };

  return totalRemoved;
}

void Blob::squashBlob() {
  for (int i = 0; i < ROW_SIZE; i++) {
    if (this->emptyLines[i]) {
      this->pushLineDown(this->emptyLines[i]);
      this->emptyLines[i] = 0;
    }
  }
  this->emptyLineIdx = 0;
}

void Blob::pushLineDown(int idx) {
  for (int i = idx; i > 0; i--) {
    for (int j = 0; j < BLOB_W; j++) {
      this->grid[j][i] = this->grid[j][i - 1];
    }
  }
}
