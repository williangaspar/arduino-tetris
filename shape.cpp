#include "shape.h"

#include <Arduino.h>

#include "screen.h"

void Shape::rotate() {
  this->setRotationIndex(this->rotationIndex + 1);
}

void Shape::setRotationIndex(int8_t index) {
  this->rotationIndex = index % 4;
}


// TODO: check bounds
void Shape::moveLeft() {
  this->x -= SQUARE_LENGHT;
}

void Shape::moveRight() {
  this->x += SQUARE_LENGHT;
}

void Shape::moveDown() {
  this->y -= 2 * SQUARE_LENGHT;
}

void Shape::draw() {
  auto currentShape = this->shape[this->rotationIndex];

  for (int i = 15; i >= 0; i--) {
    Screen::drawSquare(this->x, this->y, 255);
  }
}

// tft.fillScreen(ST77XX_BLACK);
