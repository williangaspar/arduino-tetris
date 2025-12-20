#include "snakes.h"

namespace {
LiveEntity food;
Snake snake;
}  // namespace

void Snakes::start() {
  Game::start();
  food.color = RED_IDX;
  snake.reset();
  this->getNewFood();
};

Events& Snakes::tick(Input userInput) {
  Game::events.reset();

  if (userInput == Input::Up && snake.direction != Direction::DOWN) {
    snake.direction = Direction::UP;
  } else if (userInput == Input::Down && snake.direction != Direction::UP) {
    snake.direction = Direction::DOWN;
  } else if (userInput == Input::Left && snake.direction != Direction::RIGHT) {
    snake.direction = Direction::LEFT;
  } else if (userInput == Input::Right && snake.direction != Direction::LEFT) {
    snake.direction = Direction::RIGHT;
  };

  if (Game::moveCounter >= MOVE_COUNT) {

    if (!snake.move()) {
      Game::events.isGameOver = true;
    } else {
      if (snake.x == food.x && snake.y == food.y) {
        snake.eat(food);
        this->getNewFood();
        Game::score += snake.getSize();
        Game::events.isNewPoints = true;
      };
    }

    Game::moveCounter = 0;
  };

  Game::moveCounter = ++Game::moveCounter % (MOVE_COUNT + 1);
  return Game::events;
};

void Snakes::addEntitiesToFrame(GGrid frame) {
  memset(frame, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(frame[0][0]));
  frame[food.x][food.y] = food.color;
  snake.addToFrame(frame);
  int8_t snakeColor = Game::moveCounter % 5 == 0 ? BLACK_IDX : snake.color;
  frame[snake.x][snake.y] = snakeColor;
};

void Snakes::getNewFood() {
  food.x = random(0, BLOB_W - 1);
  food.y = random(0, BLOB_H - 1);

  /* As my grandma used to say: if first you dont't succeed, try again.
  But if that fails too, give up already.
  */
  for (int i = 0; i < 3; i++) {
    if (snake.isColidingWith(food.x, food.y)) {
      food.x = random(0, BLOB_W - 1);
      food.y = random(0, BLOB_H - 1);
    } else {
      return;
    }
  }
}
