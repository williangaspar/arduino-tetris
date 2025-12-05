#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

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

class Game {
public:
  static void start();
  static int getScore();

protected:
  static Events events;
  // Carefull! we're sharing the same score between all the games. 
  static int score; // Why? 2KB of RAM, that's why!
  static int8_t moveCounter;
};

#endif