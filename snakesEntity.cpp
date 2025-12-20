#include "snakesEntity.h"

bool Snake::eat(LiveEntity& food) {  // Yummy
  if (food.x > 15 || food.y > 15) {
    return false;
  };
  this->isDigesting = body.addNewSegment(this->x, this->y);
  return this->isDigesting;
};

bool Snake::move() {
  this->body.move(this->x, this->y);

  if (this->direction == Direction::UP) {
    if (this->y <= 0) return false;
    this->y--;
  } else if (this->direction == Direction::DOWN) {
    if (this->y + 1 >= BLOB_H) return false;
    this->y++;
  } else if (this->direction == Direction::LEFT) {
    if (this->x <= 0) return false;
    this->x--;
  } else if (this->direction == Direction::RIGHT) {
    if (this->x + 1 >= BLOB_W) return false;
    this->x++;
  };

  return true;
}

bool Snake::getIsDigesting() {
  return this->isDigesting;
}

void Snake::reset() {
  this->color = BLUE_IDX;
  this->x = 3;
  this->y = 3;
  /* IMPORTANT: If you change the default direction it will break the game!
    You will need to alter Body::addNewSegment to fix it.
   */
  this->direction = Direction::RIGHT;
  this->isDigesting = false;
  this->body.reset();
  this->body.addNewSegment(this->x, this->y);
}

void Snake::addToFrame(GGrid frame) {
  this->body.addToFrame(frame);
}
uint8_t Snake::getSize() {
  return this->body.getSize() | 0;
}

bool Body::addNewSegment(int8_t x, int8_t y) {
  if (this->size >= sizeof(GGrid)) { return false; }  // We ate too much already

  if (this->size == 0) {
    /* IMPORTANT: Here I'm assumming the initial direction is alway to the right, and that we are not
      touching the left wall. I might code the direction check later, but only if I need to.
    */
    uint8_t xy = (x - 1) | (y << 4);
    this->array[0] = xy;
  } else {
    // duplicate last element and add it to the end of the array
    this->array[this->size] = this->array[this->size - 1];
  }

  this->size++;
  return true;
}

void Body::move(int8_t x, int8_t y) {
  if (this->size == 0) return;

  int8_t oldxy = this->array[0];
  this->array[0] = x | (y << 4);

  for (int i = 1; i < this->size; i++) {
    int8_t auxOldxy = this->array[i];
    this->array[i] = oldxy;
    oldxy = auxOldxy;
  };
}

void Body::reset() {
  Blob::reset();
  this->size = 0;
}

void Body::addToFrame(GGrid frame) {
  for (int i = 0; i < this->size; i++) {
    uint8_t x = this->array[i] & LOW_NIB_MSK;
    uint8_t y = (this->array[i] & HIGH_NIB_MSK) >> 4;
    frame[x][y] = BLUE_IDX;
  };
}

uint8_t Body::getSize() {
  return this->size;
}
