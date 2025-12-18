#include <EEPROM.h>

#include "config.h"
#include "game.h"
#include "tetris.h"
#include "snakes.h"
#include "screen.h"

#define BTN_UP A2
#define BTN_DOWN A5
#define BTN_LEFT A1
#define BTN_RIGHT A3
#define BTN_PAUSE A4
#define BUZZER 5
#define SAVE_GAME_ID 9

bool isButtonPressed = false;
int currentHighScore = 0;
int8_t frame[BLOB_W][BLOB_H];

Tetris tetris(0, "Tetris");
Snakes snakes(1, "Snakes");

Game *games[GAME_LIST_SIZE] = { &tetris, &snakes };
Game *game = &tetris;

// You can change the order, add or remove items. But keep the IDs the same, and avoid duplicated IDs!
GameMenuItem menuItems[GAME_MENU_ITEM_SIZE] = {
  { .id = tetris.id, .name = tetris.name },
  { .id = snakes.id, .name = snakes.name },
  { .id = SAVE_GAME_ID, .name = "Save" },
};

GameMenu menu = {
  .pointingToIndex = 0,
  .activeGameIndex = 0,
};


int loadStoredData(int8_t dataId) {   // dataId can be a gameId, but they are not the same.
  int offset = sizeof(int) * dataId;  // A dataId can be anything, but gameId must be from a game.
  int data = 0;
  EEPROM.get(offset, data);
  return data;
}

void storeData(int8_t dataId, int data) {
  int offset = sizeof(int) * dataId;
  EEPROM.put(offset, data);
}

bool setMenuActiveID(int gameId) {
  for (int i = 0; i < GAME_LIST_SIZE; i++) {
    if (menuItems[i].id == gameId) {
      menu.activeGameIndex = i;
      return true;
    };
  };
  menu.activeGameIndex = 0;
  return false;  // ID not found
}

void setCurrentGame(int8_t gameId) {
  for (int i = 0; i < GAME_LIST_SIZE; i++) {
    if (games[i]->id == gameId) {
      game = games[i];
    };
  };
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_PAUSE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  int savedGameId = loadStoredData(SAVE_GAME_ID);

  // Start menu
  bool idFound = setMenuActiveID(savedGameId);
  if (!idFound) savedGameId = menuItems[0].id;

  currentHighScore = loadStoredData(savedGameId);
  if (currentHighScore < 0) currentHighScore = 0;
  Screen::start(frame);
  Screen::updateHighScore(currentHighScore);

  setCurrentGame(savedGameId);
  game->start();
  if (game->id == tetris.id) {  // I need a better solution for the next shape calls. But this will do for now
    Screen::updateNextShape(tetris.getNextShape(), tetris.getNextColor());
  }
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

void loop() {
  unsigned long startTime = millis();
  Input userInput = readButtons();

  if (userInput == Input::Pause) {
    userInput = Input::None;
    menu.pointingToIndex = 0;
    Screen::printPause(menuItems);
    Screen::printMenu(menu, menuItems);
    tone(BUZZER, 131, 200);
    delay(TICK_SPEED);
    bool didSaveTheGame = false;
    int8_t currentActiveGameIndex = menu.activeGameIndex;
    while (userInput != Input::Pause) {
      userInput = readButtons();
      if (userInput == Input::Up) {
        if (menu.pointingToIndex > 0) {
          menu.pointingToIndex--;
        }
        Screen::printMenu(menu, menuItems);
        tone(BUZZER, 880, 50);
        delay(50);
        tone(BUZZER, 1318, 50);
      } else if (userInput == Input::Down) {
        if (menu.pointingToIndex < GAME_MENU_ITEM_SIZE - 1) {
          menu.pointingToIndex++;
        }
        Screen::printMenu(menu, menuItems);
        tone(BUZZER, 880, 50);
        delay(50);
        tone(BUZZER, 1318, 50);
      } else if (userInput == Input::Right) {
        if (menuItems[menu.pointingToIndex].id == SAVE_GAME_ID) {
          didSaveTheGame = true;
          userInput = Input::Pause;
        } else {
          menu.activeGameIndex = menu.pointingToIndex;
          Screen::printMenu(menu, menuItems);
        }
        tone(BUZZER, 880, 100);
      }
      delay(TICK_SPEED);
    };
    if (!didSaveTheGame) {
      menu.activeGameIndex = currentActiveGameIndex;
    } else {
      setCurrentGame(menuItems[menu.activeGameIndex].id);
      storeData(SAVE_GAME_ID, game->id);
      currentHighScore = loadStoredData(menuItems[menu.activeGameIndex].id);
      game->start();
      Screen::resetFrame(frame);
    }
    tone(BUZZER, 262, 200);
    Screen::reset();
    game->addEntitiesToFrame(frame);
    Screen::updateScore(game->getScore());
    Screen::updateHighScore(currentHighScore);
    if (game->id == tetris.id) {
      Screen::updateNextShape(tetris.getNextShape(), tetris.getNextColor());
    }
    Screen::drawFrame(frame);
    delay(1500);
    return;
  }

  Events &events = game->tick(userInput);

  if (events.isNewPoints) {
    tone(BUZZER, 440, 100);
    delay(100);
    tone(BUZZER, 880, 200);
    delay(200);
    Screen::updateScore(game->getScore());
  };

  if (events.isNewShape && game->id == tetris.id) {
    Screen::updateNextShape(tetris.getNextShape(), tetris.getNextColor());
    tone(BUZZER, 440, 100);
  };

  if (events.isGameOver) {
    Screen::printGameOver();
    tone(BUZZER, 554, 200);
    delay(200);
    tone(BUZZER, 277, 400);
    delay(1000);
    userInput = Input::None;

    int newScore = game->getScore();
    if (newScore > currentHighScore) {
      // congratulations, you have the new high score
      currentHighScore = newScore;
      storeData(game->id, newScore);
    }

    while (userInput == Input::None) {  // Just chilling. Waiting for the next game.
      userInput = readButtons();
      delay(TICK_SPEED);
    };

    // And here we go again.
    Screen::reset();
    game->start();
    if (game->id == tetris.id) {
      Screen::updateNextShape(tetris.getNextShape(), tetris.getNextColor());
    }
    Screen::updateScore(game->getScore());
    Screen::updateHighScore(currentHighScore);
  };

  game->addEntitiesToFrame(frame);
  Screen::drawFrame(frame);

  // This is an attempt to make the frame rate stable
  unsigned long diffTime = millis() - startTime;
  if (diffTime < TICK_SPEED) {
    delay(TICK_SPEED - diffTime);
  }
}
