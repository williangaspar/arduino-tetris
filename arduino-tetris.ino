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
#define SAVE_DATA_ID 2

bool isButtonPressed = false;
int currentHighScore = 0;
int8_t frame[BLOB_W][BLOB_H];

// You can change the order, add or remove items. But keep the IDs the same, and avoid duplicated IDs!
GameMenuItem menuItems[GAME_MENU_ITEM_SIZE] = {
  { .id = 0, .name = "Tetris" },
  { .id = 1, .name = "Snakes" },
  { .id = SAVE_DATA_ID, .name = "Save  " },
};

GameMenu menu = {
  .pointingToIndex = 0,
  .activeGameIndex = 0,
};

/*
  Using an id instead of the array index here allows the menu items to be rearrange
  without breaking the location of stored data.
*/
int loadStoredData(int8_t dataId) {
  int offset = sizeof(int) * dataId;
  int score = 0;
  EEPROM.get(offset, score);
  if (score < 0) score = 0;
  return score;
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_PAUSE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  int savedGameId = loadStoredData(SAVE_DATA_ID);

  // Start menu
  for (int i = 0; i < GAME_LIST_SIZE; i++) {
    if (menuItems[i].id == savedGameId) {
      menu.activeGameIndex = i;
      break;
    };
    savedGameId = menuItems[0].id;  // ID not found, default to first item on the list
  }

  currentHighScore = loadStoredData(savedGameId);
  Screen::start(frame);
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
    Screen::printMenuText(menuItems);
    Screen::updateMenu(menu, menuItems);
    tone(BUZZER, 131, 200);
    delay(TICK_SPEED);
    while (userInput != Input::Pause) {
      userInput = readButtons();
      if (userInput == Input::Up) {
        if (menu.pointingToIndex > 0) {
          menu.pointingToIndex--;
        }
        Screen::updateMenu(menu, menuItems);
        tone(BUZZER, 880, 50);
        delay(50);
        tone(BUZZER, 1318, 50);
      } else if (userInput == Input::Down) {
        if (menu.pointingToIndex < GAME_MENU_ITEM_SIZE - 1) {
          menu.pointingToIndex++;
        }
        Screen::updateMenu(menu, menuItems);
        tone(BUZZER, 880, 50);
        delay(50);
        tone(BUZZER, 1318, 50);
      } else if (userInput == Input::Right) {
        menu.activeGameIndex = menu.pointingToIndex;
      }
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

  onTetrisTick(userInput);

  // This is an attempt to make the frame rate stable
  unsigned long diffTime = millis() - startTime;
  if (diffTime < TICK_SPEED) {
    delay(TICK_SPEED - diffTime);
  }
}
