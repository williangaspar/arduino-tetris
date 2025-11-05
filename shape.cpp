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

bool Shape::isCollidingWithLeftWall(int leftWallx) {
  int minx = this->x;
  uint16_t shape = this->getShape();

  if ((shape & L_MSK_C1) == 0) { minx++; };
  return minx < leftWallx;
}

bool Shape::isCollidingWithRightWall(int rightWallx) {
  int maxx = this->x + ROW_SIZE + 3;
  uint16_t shape = this->getShape();

  if ((shape & R_MSK_C1) == 0) { maxx--; };
  if ((shape & R_MSK_C2) == 0) { maxx--; };
  return maxx >= (rightWallx - 1);
}

uint16_t Shape::getShape() {
  return this->grid[this->rotationIndex];
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
  for (int i = 0; i < BLOB_W; i++) {
    for (int j = 0; j < BLOB_H; j++) {
      this->grid[i][j] = 0;
    }
  }
}


bool Blob::isCollidingWithShape(Shape *shape) {
  int posx = shape->x;
  int posy = shape->y + ROW_SIZE;
  uint16_t cshape = shape->getShape();

  if (shape->y < 0) {
    return false;
  }

  for (int i = 0; i < ROW_SQR_SIZE; i++) {
    posx = shape->x + (ROW_SIZE - 1) - (i % ROW_SIZE);
    if (posx == shape->x) posy--;

    if (bitRead(cshape, i)) {
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
      totalRemoved++;
    };
  };

  return totalRemoved;
}

void Blob::squashBlob() {
  bool isEmptyLine = true;
  for (int i = BLOB_H - 1; i > 0; i--) {
    isEmptyLine = true;
    for (int j = 0; j < BLOB_W; j++) {
      if (this->grid[j][i] != 0) {
        isEmptyLine = false;
        break;
      }
    };

    if (isEmptyLine) {
      this->pushLineDown(i);
      isEmptyLine = false;
    }
  }
}

void Blob::pushLineDown(int idx) {
  for (int i = idx; i > 0; i--) {
    for (int j = 0; j < BLOB_W; j++) {
      this->grid[j][i] = this->grid[j][i - 1];
    }
  }
}
