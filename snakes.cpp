#include "snakes.h"

namespace {
LiveEntity food;
Snake snake;
bool directionJustChanged = false;
}  // namespace

void Snakes::start() {
  Game::start();
  food.color = RED_IDX;
  snake.reset();
  this->getNewFood();
};

Events& Snakes::tick(Input userInput) {
  Game::events.reset();

  // This check prevents the user killing from the snake by changing direction faster then it can move.
  if (!directionJustChanged) {
    if (userInput == Input::Up && snake.direction != Direction::DOWN) {
      snake.direction = Direction::UP;
      directionJustChanged = true;
    } else if (userInput == Input::Down && snake.direction != Direction::UP) {
      snake.direction = Direction::DOWN;
      directionJustChanged = true;
    } else if (userInput == Input::Left && snake.direction != Direction::RIGHT) {
      snake.direction = Direction::LEFT;
      directionJustChanged = true;
    } else if (userInput == Input::Right && snake.direction != Direction::LEFT) {
      snake.direction = Direction::RIGHT;
      directionJustChanged = true;
    };
  }

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
    directionJustChanged = false;
    Game::moveCounter = 0;
  };

  Game::moveCounter = ++Game::moveCounter % (MOVE_COUNT + 1);
  return Game::events;
};

void Snakes::addEntitiesToFrame(GGrid frame) {
  memset(frame, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(frame[0][0]));
  frame[food.x][food.y] = food.color;
  snake.addToFrame(frame);
  int8_t snakeColor = Game::moveCounter % 5 <= 1 ? BLACK_IDX : snake.color;
  frame[snake.x][snake.y] = snakeColor;
};

void Snakes::getNewFood() {
  food.x = random(0, BLOB_W);
  food.y = random(0, BLOB_H);

  /* As my grandma used to say: if first you dont't succeed, try again.
  But if that fails too, give up already.
  */
  for (int i = 0; i < 3; i++) {
    if (snake.isColidingWith(food.x, food.y)) {
      if (food.x < (BLOB_W - 1) && !snake.isColidingWith(food.x + 1, food.y)) {
        food.x++;
        return;
      } else if (food.x > 0 && !snake.isColidingWith(food.x - 1, food.y)) {
        food.x--;
        return;
      } else if (food.y < (BLOB_H - 1) && !snake.isColidingWith(food.x, food.y + 1)) {
        food.y++;
        return;
      } else if (food.y > 0 && !snake.isColidingWith(food.x, food.y - 1)) {
        food.y--;
        return;
      }

      food.x = random(0, BLOB_W);
      food.y = random(0, BLOB_H);
    } else {
      return;
    }
  }
}
