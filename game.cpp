#include "game.h"

#include "shape.h"

extern uint16_t z[4];
extern uint16_t l[4];
extern uint16_t j[4];
extern uint16_t t[4];
extern uint16_t line[4];
extern uint16_t sqr[4];

int32_t Game::score = 0;


namespace {
Blob blob;
int moveDownCounter = 0;
}

Shape Game::shapes[SHAPE_COUNT] = {
  Shape(l, 3),
  Shape(j, 3),
  Shape(t, 3),
  Shape(z, 3),
  Shape(line, 4),
  Shape(sqr, 4)
};

Shape* Game::getRandomShape() {
  Shape* shape = &shapes[random(0, SHAPE_COUNT)];
  shape->setRotationIndex(random(0, 4));
  shape->color = random(1, SHAPE_COUNT + 1);
  shape->x = 3;
  shape->y = -shape->rowSize;
  return shape;
}

Blob& Game::getBlob() {
  return blob;
}

void Game::start() {
  Game::score = 0;
  blob.reset();
}

void Game::tick(Shape* shape) {
  // if (blob.collisionCheck(shape)) {
  //   blob.addShape(shape);
  // }

  // Game::score += blob.pointsCheck();

  moveDownCounter++;
  if (moveDownCounter + 1 == 10) {
    shape->moveDown();
    moveDownCounter = 0;
  }
}