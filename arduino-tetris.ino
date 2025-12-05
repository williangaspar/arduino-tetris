#include <EEPROM.h>

#include "config.h"
#include "game.h"
#include "tetris.h"
#include "screen.h"

#define BTN_UP A2
#define BTN_DOWN A5
#define BTN_LEFT A1
#define BTN_RIGHT A3
#define BTN_PAUSE A4
#define BUZZER 5

enum class GameSelect {
  Tetris,
  Snakes
};

bool isButtonPressed = false;
int currentHighScore = 0;
int8_t frame[BLOB_W][BLOB_H];
GameSelect gameSelected = GameSelect::Tetris;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_PAUSE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  Screen::start(frame);
  EEPROM.get(0, currentHighScore);
  if (currentHighScore < 0) currentHighScore = 0;
  Tetris::start();
  Screen::updateHighScore(currentHighScore);
  Screen::updateNextShape(Tetris::getNextShape(), Tetris::getNextColor());
}

Input readButtons() {
  Input input = Input::None;

  // Debouce the buttons
  if (isButtonPressed) {
    isButtonPressed = false;
    return input;
  };

  if (digitalRead(BTN_UP) == LOW) input = Input::Up;
  if (digitalRead(BTN_DOWN) == LOW) input = Input::Down;
  if (digitalRead(BTN_LEFT) == LOW) input = Input::Left;
  if (digitalRead(BTN_RIGHT) == LOW) input = Input::Right;
  if (digitalRead(BTN_PAUSE) == LOW) input = Input::Pause;

  isButtonPressed = input != Input::None;

  return input;
}

void onTetrisTick(Input userInput) {
  Events& events = Tetris::tick(userInput);

  if (events.isNewPoints) {
    tone(BUZZER, 440, 100);
    delay(100);
    tone(BUZZER, 880, 200);
    delay(200);
    Screen::updateScore(Tetris::getScore());
  };

  if (events.isNewShape) {
    Screen::updateNextShape(Tetris::getNextShape(), Tetris::getNextColor());
    tone(BUZZER, 440, 100);
  };

  if (events.isGameOver) {
    Screen::printGameOver();
    tone(BUZZER, 554, 200);
    delay(200);
    tone(BUZZER, 277, 400);
    delay(1000);
    userInput = Input::None;

    while (userInput == Input::None) {
      userInput = readButtons();
      delay(TICK_SPEED);
    };

    int newScore = Tetris::getScore();

    if (newScore > currentHighScore) {
      // congratulations, you have the new high score
      currentHighScore = newScore;
      EEPROM.put(0, newScore);
    }

    Screen::reset();
    Tetris::start();
    Screen::updateNextShape(Tetris::getNextShape(), Tetris::getNextColor());
    Screen::updateScore(Tetris::getScore());
    Screen::updateHighScore(currentHighScore);
  };

  Tetris::addEntitiesToFrame(frame);
  Screen::drawFrame(frame);
}

void onSnakeTick(Input userInput) {
  // TODO: implement snakes
}

void loop() {
  unsigned long startTime = millis();
  Input userInput = readButtons();

  if (userInput == Input::Pause) {
    userInput = Input::None;
    Screen::printPause();
    tone(BUZZER, 131, 200);
    delay(TICK_SPEED);
    while (userInput != Input::Pause) {
      userInput = readButtons();
      delay(TICK_SPEED);
    };
    tone(BUZZER, 262, 200);
    Screen::reset();
    Tetris::addEntitiesToFrame(frame);
    Screen::updateScore(Tetris::getScore());
    Screen::updateHighScore(currentHighScore);
    Screen::updateNextShape(Tetris::getNextShape(), Tetris::getNextColor());
    Screen::drawFrame(frame);
    delay(1500);
    return;
  }

  gameSelected == GameSelect::Tetris ? onTetrisTick(userInput) : onSnakeTick(userInput);

  // This is an attempt to make the frame rate stable
  unsigned long diffTime = millis() - startTime;
  if (diffTime < TICK_SPEED) {
    delay(TICK_SPEED - diffTime);
  }
}
