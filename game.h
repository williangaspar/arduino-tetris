/*
  The game class determines what a game is and what happens in a game. It's like a stage where things happen.
*/

#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

#include "config.h"

enum class Input {
  Up,
  Down,
  Left,
  Right,
  Pause,
  None
};

struct Events {
  bool isNewShape;
  bool isNewPoints;
  bool isGameOver;

  void reset();
};

struct GameMenuItem {
  int8_t id;
  char* name;
};

struct GameMenu {
  int8_t pointingToIndex;
  int8_t activeGameIndex;
};

class Game {
public:
  const int8_t id;
  const char* name;
  virtual void start();
  virtual int getScore();
  virtual void addEntitiesToFrame(GGrid) = 0;
  virtual Events& tick(Input userInput) = 0;  // This is where all the big game logic lives.
  Game(int8_t id, const char* name)
    : id(id), name(name){};

protected:
  static Events events;       // Some events won't be used in every game; just ignore them
  static int score;           // Carefull! We're sharing the same score between all the games.
  static int8_t moveCounter;  // I'm assuming all games will use this.
};

#endif
