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

Game::UserInput userInput;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  Screen::start();
  Game::start();
  Screen::updateNextShape(Game::getNextShape());
}

void readButtons() {
  // Debouce the buttons
  if (isButtonPressed) {
    isButtonPressed = false;
    return;
  }

  userInput.up = userInput.up || digitalRead(BTN_UP) == LOW;
  userInput.down = userInput.down || digitalRead(BTN_DOWN) == LOW;
  userInput.left = userInput.left || digitalRead(BTN_LEFT) == LOW;
  userInput.right = userInput.right || digitalRead(BTN_RIGHT) == LOW;

  isButtonPressed = userInput.up || userInput.down || userInput.left || userInput.right;
}

void loop() {
  readButtons();
  Game::tick(userInput, [](int32_t score, Shape* nextShape) {
    delay(400);
    Screen::updateScore(score);
    Screen::updateNextShape(nextShape);
  });
  Screen::addBlobToFrame(Game::getBlob());
  Screen::addShapeToFrame(Game::getShape());
  Screen::drawFrame();
  Screen::updateNextShape(Game::getNextShape());

  delay(50);
}
