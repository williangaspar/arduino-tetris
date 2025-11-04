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
  Shape(l),
  Shape(j),
  Shape(t),
  Shape(z),
  Shape(line),
  Shape(sqr)
};

Shape* Game::getRandomShape() {
  Shape* shape = &shapes[random(0, SHAPE_COUNT)];
  shape->setRotationIndex(random(0, 4));
  shape->color = random(1, SHAPE_COUNT + 1);
  shape->x = (BLOB_W - ROW_SIZE / 2) / 2;
  shape->y = -ROW_SIZE;
  return shape;
}

Blob& Game::getBlob() {
  return blob;
}

void Game::start() {
  Game::score = 0;
  blob.reset();
}

void Game::tick(Shape*& shape) {
  if (blob.collisionCheck(shape)) {
    blob.addShape(shape);
    shape = Game::getRandomShape();
  }

  // Game::score += blob.pointsCheck();

  moveDownCounter++;
  if (moveDownCounter + 1 == 10) {
    shape->moveDown();
    moveDownCounter = 0;
  }
}