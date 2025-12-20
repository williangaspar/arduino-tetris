#include "game.h";

void Events::reset() {
  this->isNewShape = false;
  this->isNewPoints = false;
  this->isGameOver = false;
}

void Game::start() {
  Game::score = 0;
  Game::moveCounter = 0;
  Game::events.reset();
};

int Game::getScore() {
  return Game::score;
};

int Game::score = 0;
int8_t Game::moveCounter = 0;
Events Game::events;
