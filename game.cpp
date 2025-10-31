#include "game.h"

#include "screen.h"
#include "shape.h"

extern uint16_t z[4];
extern uint16_t l[4];
extern uint16_t j[4];
extern uint16_t t[4];
extern uint16_t line[4];
extern uint16_t sqr[4];

int32_t Game::score = 0;
const uint16_t Screen::colors[6];

Shape Game::shapes[SHAPE_LIST_SIZE] = {
    Shape(l, 3),
    Shape(j, 3),
    Shape(t, 3),
    Shape(z, 3),
    Shape(line, 4),
    Shape(sqr, 4)};

Shape* Game::getRandomShape() {
  Shape* shape = &shapes[random(0, SHAPE_LIST_SIZE)];
  shape->setRotationIndex(random(0, 4));
  shape->color = Screen::colors[random(0, SHAPE_LIST_SIZE)];
  shape->x = 49;
  shape->y = 0;
  return shape;
}

void Game::start() {
  Game::score = 0;
  Screen::start();
  Screen::setCursor(20, 20);
}
