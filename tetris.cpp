#include "tetris.h"

#include "sprites.h"

namespace {
Shape currshape(nullptr);
Shape nextShape(nullptr);
TetrisBlob blob;
}  // namespace

void Tetris::start() {
  Game::start();
  blob.reset();
  this->getRandomShape(currshape);
  this->getRandomShape(nextShape);
}

/* Here is where the heartbeat of the game happens. After the game starts, any changes to the objects on the screen will be
  dictated by this function. It processes the user input and returns an event object with everything that happened. */
Events& Tetris::tick(Input userInput) {
  Game::events.reset();

  /* Here is how the user input processing works: We try to execute the move. If it hits anything, we role it back */
  if (userInput == Input::Left) {
    currshape.x--;
    if (blob.isCollidingWithShape(currshape) || currshape.isCollidingWithLeftWall(0)) {
      currshape.x++;
    }

  } else if (userInput == Input::Right) {
    currshape.x++;
    if (blob.isCollidingWithShape(currshape) || currshape.isCollidingWithRightWall(BLOB_W)) {
      currshape.x--;
    }
  }
  if (userInput == Input::Up) {
    int oldRotation = currshape.getRotation();
    currshape.rotate();
    if (blob.isCollidingWithShape(currshape) || currshape.isCollidingWithLeftWall(0) || currshape.isCollidingWithRightWall(BLOB_W)) {
      currshape.setRotation(oldRotation);
    }
  } else if (userInput == Input::Down || Game::moveCounter >= MOVE_COUNT_DOWN) {
    /* 
    Two things can trigger the down moviment:
    1. The user pressing the down button; 2. The "move down" counter is getting to its limit.
    This "else" condition prevents double triggering of the events.
    If double triggering is allowed, the shape will go down faster, but a visual bug
    will happen when the shape is about to hit something after 2 ticks. The shape will not visually
    hit the blob before an event like game over, or points are triggered.
    */
    currshape.y++;
    if (blob.isCollidingWithShape(currshape)) {
      currshape.y--;
      blob.addShape(currshape);

      int8_t shapey = currshape.y;
      uint16_t shape = currshape.getShape();
      if ((shape & TOP_MSK_ROW1) == 0) shapey++;
      if ((shape & TOP_MSK_ROW2) == 0) shapey++;

      if (shapey <= 0) {  // We're too close to the sun
        Game::events.isGameOver = true;
      }

      // Every time the down move hits the blob, we get a new shape
      currshape.copyShape(nextShape);
      this->getRandomShape(nextShape);
      Game::events.isNewShape = true;

      int8_t erasedLines = blob.squash();
      if (erasedLines) {
        Game::score += erasedLines * erasedLines;
        Game::events.isNewPoints = true;
      }
    }
  };

  Game::moveCounter = ++Game::moveCounter % (MOVE_COUNT_DOWN + 1);
  return Game::events;
}

void Tetris::getRandomShape(Shape& shape) {
  uint16_t(*newGrid)[4] = &shapeList[random(0, SHAPE_COUNT)];

  shape.replaceGrid(newGrid);
  shape.setRotation(random(0, 4));
  shape.color = random(1, SHAPE_COUNT);
  shape.x = (BLOB_W - ROW_SIZE / 2) / 2;
  shape.y = -ROW_SIZE;
  return shape;
}

uint16_t Tetris::getNextShape() {
  return nextShape.getShape();
}

int8_t Tetris::getNextColor() {
  return nextShape.color;
}

void Tetris::addEntitiesToFrame(GGrid frame) {
  blob.addToFrame(frame);
  currshape.addToFrame(frame);
}