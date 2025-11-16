#include "game.h"

#include "shape.h"

extern uint16_t shapeList[][4];

namespace {
Blob blob;
Shape currshape(nullptr);
Shape nextShape(nullptr);
int8_t moveDownCounter = 0;
int score = 0;
Game::Events events;

}  // namespace

void getRandomShape(Shape& shape) {
  uint16_t(*newGrid)[4] = &shapeList[random(0, SHAPE_COUNT)];

  shape.replaceGrid(newGrid);
  shape.setRotation(random(0, 4));
  shape.color = random(1, SHAPE_COUNT);
  shape.x = (BLOB_W - ROW_SIZE / 2) / 2;
  shape.y = -ROW_SIZE;
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

Shape& Game::getShape() {
  return currshape;
}

Shape& Game::getNextShape() {
  return nextShape;
}

void Game::start() {
  score = 0;
  moveDownCounter = 0;
  events.reset();
  blob.reset();
  getRandomShape(currshape);
  getRandomShape(nextShape);
}

/* Here is where the heartbeat of the game happens. After the game start, any changes to the objects on the screen will be
  dicted by this function. It processes the user input and returns an event object with averything that happened.  */
Game::Events& Game::tick(Game::Input userInput) {
  events.reset();

  /* Here is how the user input processing works: We try to execute the move. If it hits anything, we role it back */
  if (userInput == Game::Input::Left) {
    currshape.x--;
    if (blob.isCollidingWithShape(currshape) || currshape.isCollidingWithLeftWall(0)) {
      currshape.x++;
    }

  } else if (userInput == Game::Input::Right) {
    currshape.x++;
    if (blob.isCollidingWithShape(currshape) || currshape.isCollidingWithRightWall(BLOB_W)) {
      currshape.x--;
    }
  }
  if (userInput == Game::Input::Up) {
    int oldRotation = currshape.getRotation();
    currshape.rotate();
    if (blob.isCollidingWithShape(currshape) || currshape.isCollidingWithLeftWall(0) || currshape.isCollidingWithRightWall(BLOB_W)) {
      currshape.setRotation(oldRotation);
    }
  } else if (userInput == Game::Input::Down || moveDownCounter == MOVE_COUNT_DOWN) {
    /*Two things can trigger the down moviment:
    1. Ther user pressing the down button; 2. The move down counter getting to its limit;
    This "else" condition prevents double triggering of the events. 
    If double triggering is allowed, the shape will go down faster, but a visual bug 
    will happen when the shape is about to hit something after 2 ticks. The shape will not visually 
    hit the blob before an event like game over or points is triggered.
    */
    currshape.y++;
    if (blob.isCollidingWithShape(currshape)) {
      currshape.y--;
      blob.addShape(currshape);

      if (currshape.y <= 0) {  // We're to close to the sun
        events.isGameOver = true;
      }

      // Every time the down move hits the blob, we get a new shape
      currshape.copyShape(nextShape);
      getRandomShape(nextShape);
      events.isNewShape = true;

      int8_t erasedLines = blob.squash();
      if (erasedLines) {
        score += erasedLines * erasedLines;
        events.isNewPoints = true;
      }
    }
  };

  moveDownCounter = ++moveDownCounter % (MOVE_COUNT_DOWN + 1);

  return events;
}