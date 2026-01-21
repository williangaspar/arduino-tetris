#include "sound.h"

void Sound::gameOver() {
  play(554, 200);
  wait(200);
  play(277, 400);
  wait(1000);
}

void Sound::pause() {
  play(131, 200);
}

void Sound::unpause() {
  play(262, 200);
}

void Sound::menuNav() {
  play(880, 50);
  wait(50);
  play(1318, 50);
}

void Sound::menuSelect() {
  play(880, 100);
}

void Sound::points() {
  play(440, 100);
  wait(100);
  play(880, 200);
}

void Sound::start() {
  play(392, 100);
  wait(100);
  play(523, 100);
  wait(100);
  play(784, 200);
}

void Sound::nextShape() {
  play(440, 100);
}

void Sound::play(int toneFreq, int ms) {
  if (Sound::isEnabled) {
    tone(BUZZER, toneFreq, ms);
  }
};

void Sound::wait(int ms) {
  if (Sound::isEnabled) {
    delay(ms);
  }
}

bool Sound::isEnabled = true;