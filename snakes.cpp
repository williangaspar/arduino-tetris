#include "snakes.h"

namespace {
LiveEntity food;
Snake snake;
}  // namespace

void Snakes::start() {
  Game::start();
  food.color = 1;
  snake.color = 2;
  snake.x = 3;
  snake.y = 3;
  snake.direction = Direction::RIGHT;
  snake.finishDigestion();
  this->getNewFood();
};

Events& Snakes::tick(Input userInput) {
  Game::events.reset();
  /*There are only 4 valid positions for the body start to be: directly above, directly below, to the left or to the right.
   \/
  |=|  |=|   >|=|  |=|<
        /\
  If the body starts anywhere else something went wrong.
  */

  if (userInput == Input::Up && snake.direction != Direction::DOWN) {
    snake.direction = Direction::UP;
  } else if (userInput == Input::Down && snake.direction != Direction::UP) {
    snake.direction = Direction::DOWN;
  } else if (userInput == Input::Left && snake.direction != Direction::RIGHT) {
    snake.direction = Direction::LEFT;
  } else if (userInput == Input::Right && snake.direction != Direction::LEFT) {
    snake.direction = Direction::RIGHT;
  };

  if (Game::moveCounter >= 10) {
    if (snake.direction == Direction::UP) {
      if (snake.y <= 0)
        Game::events.isGameOver = true;
      else
        snake.y--;
    } else if (snake.direction == Direction::DOWN) {
      if (snake.y >= BLOB_H)
        Game::events.isGameOver = true;
      else
        snake.y++;
    } else if (snake.direction == Direction::LEFT) {
      if (snake.x <= 0)
        Game::events.isGameOver = true;
      else
        snake.x--;
    } else if (snake.direction == Direction::RIGHT) {
      if (snake.x >= BLOB_W)
        Game::events.isGameOver = true;  // There is not scape from the blob perimeter!
      else
        snake.x++;
    };

    if (snake.getIsDigesting()) {
      snake.finishDigestion();
      this->getNewFood();
    } else {
      if (snake.x == food.x && snake.y == food.y) {
        snake.eat(food);
        Game::score++;
        Game::events.isNewPoints = true;
      };
    }
    Game::moveCounter = 0;
  };

  Game::moveCounter = ++Game::moveCounter % (10 + 1);
  return Game::events;
};

void Snakes::addEntitiesToFrame(GGrid frame) {
  memset(frame, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(frame[0][0]));
  frame[food.x][food.y] = food.color;
  snake.addToFrame(frame);
};

void Snakes::getNewFood() {
  food.x = random(0, BLOB_W);
  food.y = random(0, BLOB_H);
}
