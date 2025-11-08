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
Shape* nextShape = nullptr;
int moveDownCounter = 0;
int32_t score = 0;
Game::Events events;

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

void Game::Events::reset() {
  this->isNewShape = false;
  this->isNewPoints = false;
  this->isGameOver = false;
}

int32_t Game::getScore() {
  return score;
}

Blob& Game::getBlob() {
  return blob;
}

Shape* Game::getShape() {
  return shape;
}

Shape* Game::getNextShape() {
  return nextShape;
}

void Game::start() {
  score = 0;
  moveDownCounter = 0;
  events.reset();
  blob.reset();
  blob.x = 1;
  shape = getRandomShape();
  nextShape = getRandomShape();
}

void onMoveDown() {
  shape->y++;
  if (blob.isCollidingWithShape(shape)) {
    shape->y--;
    blob.addShape(shape);

    if (shape->y <= 0) {
      events.isGameOver = true;
    }

    shape = nextShape;
    nextShape = getRandomShape();

    events.isNewShape = true;

    int newPoints = blob.eraseFilledLines();
    if (newPoints) {
      score += newPoints * newPoints;
      events.isNewPoints = true;
      blob.squashBlob();
    }
  }
}


Game::Events& Game::tick(Game::Input& userInput) {
  events.reset();

  switch (userInput) {
    case Game::Input::Up:
      {
        int oldRotation = shape->getRotation();
        shape->rotate();
        if (blob.isCollidingWithShape(shape) || shape->isCollidingWithLeftWall(0) || shape->isCollidingWithRightWall(BLOB_W)) {
          shape->setRotation(oldRotation);
        }
        break;
      }

    case Game::Input::Down:
      onMoveDown();
      break;
    case Game::Input::Left:
      shape->x--;
      if (blob.isCollidingWithShape(shape) || shape->isCollidingWithLeftWall(0)) {
        shape->x++;
      }
      break;
    case Game::Input::Right:
      shape->x++;
      if (blob.isCollidingWithShape(shape) || shape->isCollidingWithRightWall(BLOB_W)) {
        shape->x--;
      }
      break;
  };


  if (moveDownCounter == 10) {
    onMoveDown();
    moveDownCounter = 0;
  }
  moveDownCounter++;

  return events;
}