#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include "config.h"

class Sound {
public:
  static bool isEnabled;
  static void gameOver();
  static void pause();
  static void unpause();
  static void points();
  static void menuNav();
  static void menuSelect();
  static void nextShape();
  static void start();

private:
  static void play(int toneFreq, int ms);  // This will break if the time greater than 32s.
  static void wait(int ms);                // But if that happens, wtf are you doing?
};

#endif  // SOUND_H