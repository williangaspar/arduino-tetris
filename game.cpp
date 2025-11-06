#include "game.h"

#include "shape.h"

extern uint16_t z[4];
extern uint16_t s[4];
extern uint16_t l[4];
extern uint16_t j[4];
extern uint16_t t[4];
extern uint16_t line[4];
extern uint16_t sqr[4];

namespace {
Blob blob;
Shape* shape = nullptr;
int moveDownCounter = 0;
int32_t score = 0;

Shape shapes[SHAPE_COUNT] = {
  Shape(l),
  Shape(j),
  Shape(t),
  Shape(z),
  Shape(s),
  Shape(line),
  Shape(sqr)
};

}

Shape* getRandomShape() {
  Shape* shape = &shapes[random(0, SHAPE_COUNT)];
  shape->setRotation(random(0, 4));
  shape->color = random(1, SHAPE_COUNT);
  shape->x = (BLOB_W - ROW_SIZE / 2) / 2;
  shape->y = -ROW_SIZE;
  return shape;
}

void Game::UserInput::reset() {
  this->left = false;
  this->right = false;
  this->up = false;
  this->down = false;
}

Blob& Game::getBlob() {
  return blob;
}

Shape* Game::getShape() {
  return shape;
}

void Game::start() {
  score = 0;
  blob.reset();
  blob.x = 1;
  shape = getRandomShape();
}

void Game::tick(Game::UserInput& userInput, void (*pointsCallback)(int32_t newScore)) {
  if (userInput.left) {
    shape->x--;
    if (blob.isCollidingWithShape(shape) || shape->isCollidingWithLeftWall(0)) {
      shape->x++;
    }
  } else if (userInput.right) {
    shape->x++;
    if (blob.isCollidingWithShape(shape) || shape->isCollidingWithRightWall(BLOB_H)) {
      shape->x--;
    }
  } else if (userInput.up) {
    int oldRotation = shape->getRotation();
    shape->rotate();
    if (blob.isCollidingWithShape(shape) || shape->isCollidingWithLeftWall(0) || shape->isCollidingWithRightWall(BLOB_H)) {
      shape->setRotation(oldRotation);
    }
  } else if (userInput.down) {
    // join with count down logic
  }

  userInput.reset();

  if (moveDownCounter == 10) {
    shape->y++;
    if (blob.isCollidingWithShape(shape)) {
      shape->y--;
      blob.addShape(shape);
      shape = getRandomShape();
      int newPoints = blob.eraseFilledLines();

      if (newPoints) {
        score += newPoints * newPoints;
        pointsCallback(score);
        blob.squashBlob();
      }
    }

    moveDownCounter = 0;
  }

  moveDownCounter++;
}