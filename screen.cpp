#include "screen.h"

namespace {
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
int8_t currentFrame[BLOB_W][BLOB_H];
int score = 0;
int highScore = 0;
}

void Screen::resetFrame(int8_t frame[BLOB_W][BLOB_H]) {
  memset(frame, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(frame[0][0]));
}

void Screen::start(int8_t nextFrame[BLOB_W][BLOB_H]) {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);
  tft.fillScreen(BG_COLOR);
  tft.drawFastVLine(BLOB_W * SQR_TSIZE + SM_PAD, SM_PAD, SCR_HEIGHT, FG_COLOR);
  Screen::updateScore(score);
  Screen::resetFrame(nextFrame);
}

void Screen::reset() {
  tft.fillScreen(BG_COLOR);
  tft.drawFastVLine(BLOB_W * SQR_TSIZE + SM_PAD, SM_PAD, SCR_HEIGHT, FG_COLOR);
}

void Screen::drawFrame(int8_t nextFrame[BLOB_W][BLOB_H]) {
  for (int8_t i = 0; i < BLOB_W; i++) {
    for (int8_t j = 0; j < BLOB_H; j++) {
      if (currentFrame[i][j] != nextFrame[i][j]) {
        tft.fillRect(i * SQR_TSIZE + SM_PAD, j * SQR_TSIZE + MD_PAD, SQR_SIZE, SQR_SIZE, colors[nextFrame[i][j]]);
        currentFrame[i][j] = nextFrame[i][j];
      }
    }
  };
}

void Screen::updateNextShape(uint16_t nextShape, int8_t color) {
  int x = BLOB_W * SQR_TSIZE + MD_PAD + 1;
  int y = MD_PAD;
  char buffer[5];

  tft.setCursor(x, y);
  tft.setTextColor(FG_COLOR);
  tft.setTextSize(1);
  tft.println("NEXT");

  y += TXT_HEIGHT;

  int posx = 0;
  int posy = 0;

  for (int8_t i = 0; i < ROW_SQR_SIZE; i++) {
    posx = 0 + (i % ROW_SIZE);
    if (posx == 0) posy++;
    tft.fillRect(posx * NXT_SQR_TSIZE + x,
                 posy * NXT_SQR_TSIZE + TXT_HEIGHT,
                 NXT_SQR_SIZE, NXT_SQR_SIZE,
                 colors[bitRead(nextShape, ROW_SQR_SIZE - i - 1) ? color : 0]);
  }
}

void Screen::updateScore(int newScore) {
  Screen::printLabelNumber("SCOR", newScore, score, 0);
  score = newScore;  // Update local score
}

void Screen::updateHighScore(int newHighScore) {
  Screen::printLabelNumber("HIGH", newHighScore, highScore, 1);
  highScore = newHighScore;  // Update local high score
}

void Screen::printGameOver() {
  int x = (BLOB_W * SQR_TSIZE) / 2 - 36;
  int y = SCR_HEIGHT / 2 - (TXT_HEIGHT * 6 / 2);
  tft.setTextSize(3);

  char *game = "Game";
  char *over = "Over";

  Screen::printText(game, x, y, BG_COLOR);
  Screen::printText(over, x, tft.getCursorY(), BG_COLOR);
  x += 2;
  y += 2;
  Screen::printText(game, x, y, FG_COLOR);
  Screen::printText(over, x, tft.getCursorY(), FG_COLOR);
}

void Screen::printMenuText(GameMenuItem items[GAME_LIST_SIZE + 1]) {
  tft.fillScreen(BG_COLOR);
  int x = (BLOB_W * SQR_TSIZE) / 2 - 36;
  int y = TXT_HEIGHT * 2;
  tft.setTextSize(3);
  Screen::printText("Paused", x, y, FG_COLOR);

  y += TXT_HEIGHT + SM_PAD;
  tft.setTextSize(1);
  for (int i = 0; i <= GAME_LIST_SIZE; i++) {
    y += TXT_HEIGHT + SM_PAD;
    Screen::printText(items[i].name, x + 10, y, FG_COLOR);
  };
  Screen::resetFrame(currentFrame);
}

void Screen::updateMenu(GameMenu menu, GameMenuItem items[GAME_LIST_SIZE + 1]) {
  int x = (BLOB_W * SQR_TSIZE) / 2 - 36;
  int y = TXT_HEIGHT * 3 + SM_PAD;

  for (int i = 0; i <= GAME_LIST_SIZE; i++) {
    y += TXT_HEIGHT + SM_PAD;

    uint16_t arrowColor = menu.pointingToIndex == i ? ST7735_BLUE : BG_COLOR;
    tft.fillTriangle(x, y, x + 6, y + 3, x, y + 6, arrowColor);
  };
}

void Screen::printLabelNumber(char *label, int newNumber, int oldNumber, int offset) {
  int x = BLOB_W * SQR_TSIZE + MD_PAD + 1;
  int shapeTextPad = MD_PAD + NXT_SQR_TSIZE * ROW_SIZE;
  int y = shapeTextPad + (offset * TXT_HEIGHT) + (TXT_HEIGHT + SM_PAD) * ++offset;

  tft.setTextSize(1);
  Screen::printText(label, x, y, FG_COLOR);

  // Clear old number
  Screen::printNumber(oldNumber, x, y + TXT_HEIGHT, BG_COLOR);

  // Draw new  number
  Screen::printNumber(oldNumber, x, y + TXT_HEIGHT, FG_COLOR);
}

void Screen::printText(char *text, int x, int y, uint16_t color) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.println(text);
}

void Screen::printNumber(int number, int x, int y, uint16_t color) {
  char buffer[5];
  tft.setCursor(x, y);
  tft.setTextColor(color);
  sprintf(buffer, "%04d", number);
  tft.println(buffer);
}
