#include <EEPROM.h>

#include "config.h"
#include "game.h"
#include "screen.h"
#include "snakes.h"
#include "tetris.h"
#include "sound.h"

#define SAVE_GAME_ID 9
#define SOUND_ID 10

bool isButtonPressed = false;
int currentHighScore = 0;
GGrid frame;

Tetris tetris(0, "Tetris");
Snakes snakes(1, "Snakes");

Game* games[GAME_LIST_SIZE] = { &tetris, &snakes };
Game* game = &tetris;

// You can ditch this declaration and copy the string to GameMenuItem directly if you want to crash the game. :)
char soundMenuText[] = "Sound:on ";

// You can change the order, add or remove items. But keep the IDs the same, and avoid duplicated IDs!
GameMenuItem menuItems[GAME_MENU_ITEM_SIZE] = {
  { .id = tetris.id, .name = tetris.name },
  { .id = snakes.id, .name = snakes.name },
  { .id = SOUND_ID, .name = soundMenuText },
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

void updateSoundText(int soundIdx) {
  // This might me wastfull, but it is also harder to break, since it can account for a dinamic menu.
  if (soundIdx < 0) {
    for (int i = 0; i < GAME_MENU_ITEM_SIZE; i++) {
      if (menuItems[i].id == SOUND_ID) {
        soundIdx = i;
        break;
      }
    }
  }

  if (soundIdx < 0) return;

  int len = strlen(menuItems[soundIdx].name);

  if (Sound::isEnabled) {
    // Initial Text: Sound:off ; Final text: Sound:on
    menuItems[soundIdx].name[len - 1] = ' ';
    menuItems[soundIdx].name[len - 2] = 'n';
  } else {
    menuItems[soundIdx].name[len - 1] = 'f';
    menuItems[soundIdx].name[len - 2] = 'f';
  }
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

  Sound::isEnabled = !!loadStoredData(SOUND_ID);
  updateSoundText(-1);

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

void onPause(Input& userInput) {
  userInput = Input::None;
  menu.pointingToIndex = 0;
  Screen::printPause(menuItems);
  Screen::printMenu(menu, menuItems);
  Sound::pause();
  delay(TICK_SPEED);
  bool didSaveTheGame = false;
  bool didSaveSound = false;
  int8_t currentActiveGameIndex = menu.activeGameIndex;

  while (userInput != Input::Pause) {
    userInput = readButtons();
    if (userInput == Input::Up) {
      if (menu.pointingToIndex > 0) {
        menu.pointingToIndex--;
      }
      Screen::printMenu(menu, menuItems);
      Sound::menuNav();
    } else if (userInput == Input::Down) {
      if (menu.pointingToIndex < GAME_MENU_ITEM_SIZE - 1) {
        menu.pointingToIndex++;
      }
      Screen::printMenu(menu, menuItems);
      Sound::menuNav();
    } else if (userInput == Input::Right) {
      if (menuItems[menu.pointingToIndex].id == SAVE_GAME_ID) {
        didSaveTheGame = true;
        userInput = Input::Pause;
      } else if (menuItems[menu.pointingToIndex].id == SOUND_ID) {
        Sound::isEnabled = !Sound::isEnabled;
        updateSoundText(menu.pointingToIndex);
        didSaveSound = true;
        Screen::printMenu(menu, menuItems);
      } else {
        menu.activeGameIndex = menu.pointingToIndex;
        Screen::printMenu(menu, menuItems);
      }
      Sound::menuSelect();
    }
    delay(TICK_SPEED);
  };

  if (didSaveSound) {
    storeData(SOUND_ID, Sound::isEnabled);
  }

  if (!didSaveTheGame) {
    menu.activeGameIndex = currentActiveGameIndex;
  } else {
    setCurrentGame(menuItems[menu.activeGameIndex].id);
    storeData(SAVE_GAME_ID, game->id);
    currentHighScore = loadStoredData(menuItems[menu.activeGameIndex].id);
    game->start();
    Screen::resetFrame(frame);
  }
  Sound::unpause();
  Screen::reset();
  game->addEntitiesToFrame(frame);
  Screen::updateScore(game->getScore());
  Screen::updateHighScore(currentHighScore);
  if (game->id == tetris.id) {
    Screen::updateNextShape(tetris.getNextShape(), tetris.getNextColor());
  }
  Screen::drawFrame(frame);
  delay(1500);
}

void loop() {
  unsigned long startTime = millis();
  Input userInput = readButtons();

  if (userInput == Input::Pause) {
    onPause(userInput);
    return;
  }

  Events& events = game->tick(userInput);

  if (events.isNewPoints) {
    Sound::points();
    if (game->id == tetris.id) delay(200);
    Screen::updateScore(game->getScore());
  };

  if (events.isNewShape && game->id == tetris.id) {
    Screen::updateNextShape(tetris.getNextShape(), tetris.getNextColor());
    Sound::nextShape();
  };

  if (events.isGameOver) {
    Screen::printGameOver();
    Sound::gameOver();
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

    if (userInput == Input::Pause) {
      onPause(userInput);
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
