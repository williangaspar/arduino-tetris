#include "game.h"
#include "shape.h"
#include "screen.h"
#include "config.h"

#define BTN_UP A1
#define BTN_DOWN A3
#define BTN_LEFT A4
#define BTN_RIGHT A2

bool isButtonPressed = false;

Shape* shape = nullptr;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  Screen::start();
  Game::start();
  shape = Game::getRandomShape();
}

void readButtons() {
  // Debouce the buttons
  if (isButtonPressed) {
    isButtonPressed = false;
    return;
  }

  if (digitalRead(BTN_UP) == LOW) {
    shape->rotate();
    isButtonPressed = true;
  };

  if (digitalRead(BTN_DOWN) == LOW) {
    shape->moveDown();
    isButtonPressed = true;
  };

  if (digitalRead(BTN_LEFT) == LOW) {
    shape->moveLeft();
    isButtonPressed = true;
  }

  if (digitalRead(BTN_RIGHT) == LOW) {
    shape->moveRight();
    isButtonPressed = true;
  }
}

void loop() {
  readButtons();
  Game::tick(shape);
  Screen::addBlobToFrame(Game::getBlob());
  Screen::addShapeToFrame(shape);
  Screen::drawFrame();

  delay(50);
  
}
