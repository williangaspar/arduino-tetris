#ifndef SNAKES_H
#define SNAKES_H

#include "game.h"
#include "snakesEntity.h"

class Snakes : public Game {
public:
  Snakes(int8_t id, const char* name)
    : Game(id, name) {}
  void start() override;
  void addEntitiesToFrame(GGrid frame) override;
  Events& tick(Input userInput) override;

private:
  void getNewFood();
};

#endif  // SNAKES_H