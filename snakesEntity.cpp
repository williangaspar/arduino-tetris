#include "snakesEntity.h"

void Snake::eat(LiveEntity &food) {
  this->isDigesting = true; // Yummy
};

bool Snake::getIsDigesting() {
  return this->isDigesting;
}

void Snake::finishDigestion() {
  this->isDigesting = false;
  // TODO: add extra node to snake;
}