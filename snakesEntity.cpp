#include "snakesEntity.h"

void Body::addToFrame(GGrid frame) {
  for (int i = 0; i <= this->tailIdx; i++) {
    uint8_t x = this->array[i] & LOW_NIB_MSK;
    uint8_t y = this->array[i] & HIGH_NIB_MSK >> 4;
  };
}

void Snake::eat(LiveEntity& food) {
  this->isDigesting = true;  // Yummy
};

bool Snake::getIsDigesting() {
  return this->isDigesting;
}

void Snake::finishDigestion() {
  this->isDigesting = false;
  // TODO: add extra node to snake;
}

void Snake::addToFrame(GGrid frame) {
  this->body.addToFrame(frame);
  frame[this->x][this->y] = this->color;
}