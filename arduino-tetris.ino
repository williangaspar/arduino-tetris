#include <EEPROM.h>

#include "game.h"
#include "shape.h"
#include "screen.h"
#include "config.h"

#define BTN_UP A2
#define BTN_DOWN A5
#define BTN_LEFT A1
#define BTN_RIGHT A3
#define BTN_PAUSE A4
#define BUZZER 5

bool isButtonPressed = false;
int currentHighScore = 0;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_PAUSE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  Screen::start();
  EEPROM.get(0, currentHighScore);
  if (currentHighScore < 0) currentHighScore = 0;
  Game::start();
  Screen::updateHighScore(currentHighScore);
  Screen::updateNextShape(Game::getNextShape());
}

Game::Input readButtons() {
  Game::Input input = Game::Input::None;

  // Debouce the buttons
  if (isButtonPressed) {
    isButtonPressed = false;
    return input;
  };

  if (digitalRead(BTN_UP) == LOW) input = Game::Input::Up;
  if (digitalRead(BTN_DOWN) == LOW) input = Game::Input::Down;
  if (digitalRead(BTN_LEFT) == LOW) input = Game::Input::Left;
  if (digitalRead(BTN_RIGHT) == LOW) input = Game::Input::Right;
  if (digitalRead(BTN_PAUSE) == LOW) input = Game::Input::Pause;

  isButtonPressed = input != Game::Input::None;

  return input;
}

void loop() {
  Game::Input userInput = readButtons();

  if (userInput == Game::Input::Pause) {
    userInput = Game::Input::None;
    Screen::printPause();
    tone(BUZZER, 131, 200);
    delay(TICK_SPEED);
    while (userInput != Game::Input::Pause) {
      userInput = readButtons();
      delay(TICK_SPEED);
    };
    tone(BUZZER, 262, 200);
    Screen::reset();
    Screen::addBlobToFrame(Game::getBlob());
    Screen::addShapeToFrame(Game::getShape());
    Screen::updateScore(Game::getScore());
    Screen::updateHighScore(currentHighScore);
    Screen::updateNextShape(Game::getNextShape());
    Screen::drawFrame();
    delay(1500);
    return;
  }

  Game::Events& events = Game::tick(userInput);

  if (events.isNewPoints) {
    tone(BUZZER, 440, 100);
    delay(100);
    tone(BUZZER, 880, 200);
    delay(200);
    Screen::updateScore(Game::getScore());
  };

  if (events.isNewShape) {
    Screen::updateNextShape(Game::getNextShape());
    tone(BUZZER, 440, 100);
  };

  if (events.isGameOver) {
    Screen::printGameOver();
    tone(BUZZER, 554, 200);
    delay(200);
    tone(BUZZER, 277, 400);
    delay(1000);
    userInput = Game::Input::None;

    while (userInput == Game::Input::None) {
      userInput = readButtons();
      delay(TICK_SPEED);
    };

    int newScore = Game::getScore();

    if (newScore > currentHighScore) {
      // congratulations, you have the new high score
      currentHighScore = newScore;
      EEPROM.put(0, newScore);
    }

    Screen::reset();
    Game::start();
    Screen::updateNextShape(Game::getNextShape());
    Screen::updateScore(Game::getScore());
    Screen::updateHighScore(currentHighScore);
  };

  Screen::addBlobToFrame(Game::getBlob());
  Screen::addShapeToFrame(Game::getShape());
  Screen::drawFrame();

  delay(TICK_SPEED);
}
