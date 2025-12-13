#ifndef SNAKES_H
#define SNAKES_H

#include "game.h"
#include "snakesEntity.h"

class Snakes : public Game {
public:
  Snakes(int8_t id, const char* name)
    : Game(id, name) {}
  void start() override;
  void addEntitiesToFrame(int8_t frame[BLOB_W][BLOB_H]) override;
  Events& tick(Input userInput) override;

private:
  void setRandomFoodPosition();

};

#endif  // SNAKES_H