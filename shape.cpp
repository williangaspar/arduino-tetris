#include "shape.h"

#include <Arduino.h>

#include "screen.h"

#define SQR

void Shape::rotate() {
  this->setRotationIndex(this->rotationIndex + 1);
}

void Shape::setRotationIndex(int8_t index) {
  this->rotationIndex = index % 4;
}


// TODO: check bounds
void Shape::moveLeft() {
  this->x -= SQR_SIZE + SQR_GAP;
}

void Shape::moveRight() {
  this->x += SQR_SIZE + SQR_GAP;
}

void Shape::moveDown() {
  this->y -= 2 * SQR_SIZE + SQR_GAP;
}

void Shape::draw() {
  auto currentShape = this->shape[this->rotationIndex];

  int16_t x = this->x;
  int16_t y = this->y;

  int for_size = this->rowSize * this->rowSize;

  for (int i = 0; i < for_size; i++) {
    if (bitRead(this->shape[this->rotationIndex], i)) {
      Screen::drawSquare(x, y, SQR_SIZE, this->color);
    };

    x = this->x + (SQR_SIZE + SQR_GAP) * ((i + 1) % this->rowSize);
    if (x == this->x) y += (SQR_SIZE + SQR_GAP);
  }
}
