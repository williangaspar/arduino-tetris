#include "snakes.h"

namespace {
LiveEntity food;
}  // namespace

void Snakes::start() {
  Game::start();
  food.color = 1;
  this->setRandomFoodPosition();
};

Events& Snakes::tick(Input userInput) {
  return Game::events;
};

void Snakes::addEntitiesToFrame(int8_t frame[BLOB_W][BLOB_H]) {
  frame[food.x][food.y] = food.color;
};

void Snakes::setRandomFoodPosition() {
  food.x = random(0, BLOB_W);
  food.y = random(0, BLOB_H);
}
