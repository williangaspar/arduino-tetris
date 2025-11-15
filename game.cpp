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
Shape* currshape = nullptr;
Shape* nextShape = nullptr;
int8_t moveDownCounter = 0;
int score = 0;
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

Shape shapesDuplicate[SHAPE_COUNT] = {
  Shape(l),
  Shape(j),
  Shape(t),
  Shape(z),
  Shape(s),
  Shape(line),
  Shape(sqr)
};

}  // namespace

Shape* getRandomShape() {
  int idx = random(0, SHAPE_COUNT);
  Shape* shape = &shapes[idx];

  /* If the current shape is the same as the next one, a conflict will happen since we are changing all the properties
    of the shape when getting a new one.
    To avoid this, and to allow the same shape to appear twice at the same time, we mirror the shape list. */
  if (shape == currshape) {
    shape = &shapesDuplicate[idx];
  }

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

int Game::getScore() {
  return score;
}

Blob& Game::getBlob() {
  return blob;
}

Shape* Game::getShape() {
  return currshape;
}

Shape* Game::getNextShape() {
  return nextShape;
}

void Game::start() {
  score = 0;
  moveDownCounter = 0;
  events.reset();
  blob.reset();
  currshape = getRandomShape();
  nextShape = getRandomShape();
}

void onMoveDown() {
  /*Two things can trigger the down moviment:
    1. Ther user pressing the down button; 2. The move down counter getting to its limit;
    That is why this function was extracted from Game::tick.
    */
  currshape->y++;
  if (blob.isCollidingWithShape(currshape)) {
    currshape->y--;
    blob.addShape(currshape);

    if (currshape->y <= 0) {  // We're to close to the sun
      events.isGameOver = true;
    }

    // Every time the down move hits the blob, we get a new shape
    currshape = nextShape;
    nextShape = getRandomShape();
    events.isNewShape = true;

    int8_t erasedLines = blob.squash();
    if (erasedLines) {
      score += erasedLines * erasedLines;
      events.isNewPoints = true;
    }
  }
}

/* Here is where the heartbeat of the game happens. After the game start, any changes to the objects on the screen will be
  dicted by this function. It processes the user input and returns an event object with averything that happened.  */
Game::Events& Game::tick(Game::Input userInput) {
  events.reset();

  /* Here is how the user input processing works: We try to execute the move. If it hits anything, we role it back */
  if (userInput == Game::Input::Left) {
    currshape->x--;
    if (blob.isCollidingWithShape(currshape) || currshape->isCollidingWithLeftWall(0)) {
      currshape->x++;
    }

  } else if (userInput == Game::Input::Right) {
    currshape->x++;
    if (blob.isCollidingWithShape(currshape) || currshape->isCollidingWithRightWall(BLOB_W)) {
      currshape->x--;
    }
  }
  if (userInput == Game::Input::Up) {
    int oldRotation = currshape->getRotation();
    currshape->rotate();
    if (blob.isCollidingWithShape(currshape) || currshape->isCollidingWithLeftWall(0) || currshape->isCollidingWithRightWall(BLOB_W)) {
      currshape->setRotation(oldRotation);
    }
  } else if (userInput == Game::Input::Down) {
    onMoveDown();
  };

  if (moveDownCounter == MOVE_COUNT_DOWN) {
    onMoveDown();
  }

  moveDownCounter = ++moveDownCounter % (MOVE_COUNT_DOWN + 1);

  return events;
}