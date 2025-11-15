#include "screen.h"

namespace {
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int8_t currentFrame[BLOB_W][BLOB_H];
int8_t nextFrame[BLOB_W][BLOB_H];
int32_t score = 0;
int32_t highScore = 0;
}

void Screen::resetFrame(int8_t frame[BLOB_W][BLOB_H]) {
  memset(frame, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(frame[0][0]));
}

void Screen::start() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);
  delay(200);
  tft.fillScreen(BG_COLOR);
  tft.drawFastVLine(BLOB_W * SQR_TSIZE + SM_PAD, SM_PAD, SCR_HEIGHT, FG_COLOR);
  Screen::updateScore(score);
  Screen::resetFrame(nextFrame);
}

void Screen::reset() {
  tft.fillScreen(BG_COLOR);
  tft.drawFastVLine(BLOB_W * SQR_TSIZE + SM_PAD, SM_PAD, SCR_HEIGHT, FG_COLOR);
}

void Screen::addShapeToFrame(Shape *shape) {
  shape->addShapeToGrid(nextFrame);
}

void Screen::addBlobToFrame(Blob &blob) {
  for (int x = 0; x < BLOB_W; x++) {
    for (int y = 0; y < BLOB_H; y++) {
      nextFrame[x][y] = blob.getValue(x, y);
    }
  }
}

void Screen::drawFrame() {
  for (int8_t i = 0; i < BLOB_W; i++) {
    for (int8_t j = 0; j < BLOB_H; j++) {
      if (currentFrame[i][j] != nextFrame[i][j]) {
        tft.fillRect(i * SQR_TSIZE + SM_PAD, j * SQR_TSIZE + MD_PAD, SQR_SIZE, SQR_SIZE, colors[nextFrame[i][j]]);
        currentFrame[i][j] = nextFrame[i][j];
      }
    }
  };
}

void Screen::updateNextShape(Shape *nextShape) {

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
  uint16_t cshape = nextShape->getShape();

  for (int8_t i = 0; i < ROW_SQR_SIZE; i++) {
    posx = 0 + (i % ROW_SIZE);
    if (posx == 0) posy++;
    int color = bitRead(cshape, ROW_SQR_SIZE - i - 1) ? nextShape->color : 0;
    tft.fillRect(posx * NXT_SQR_TSIZE + x,
                 posy * NXT_SQR_TSIZE + TXT_HEIGHT,
                 NXT_SQR_SIZE, NXT_SQR_SIZE,
                 colors[color]);
  }
}

void Screen::updateScore(int32_t newScore) {
  Screen::printext("SCOR", newScore, score, 0);
  score = newScore;  // Update local score
}

void Screen::updateHighScore(int32_t newHighScore) {
  Screen::printext("HIGH", newHighScore, highScore, 1);
  highScore = newHighScore;  // Update local high score
}

void Screen::printGameOver() {
  Screen::printBigText("GAME", "OVER");
}

void Screen::printPause() {
  tft.fillScreen(BG_COLOR);
  Screen::printBigText("PAUSED", "");
  Screen::resetFrame(currentFrame);
}

void Screen::printext(char *text, int32_t newNumber, int32_t oldNumber, int offset) {
  int x = BLOB_W * SQR_TSIZE + MD_PAD + 1;
  int shapeTextPad =  MD_PAD + NXT_SQR_TSIZE * ROW_SIZE;
  int y = shapeTextPad + (offset * TXT_HEIGHT) + (TXT_HEIGHT + SM_PAD) * ++offset;
  char buffer[5];

  tft.setCursor(x, y);
  tft.setTextColor(FG_COLOR);
  tft.setTextSize(1);
  tft.println(text);

  // Clear old text
  tft.setCursor(x, y + TXT_HEIGHT);
  tft.setTextColor(BG_COLOR);
  sprintf(buffer, "%04d", oldNumber);
  tft.println(buffer);

  // Draw new text
  tft.setCursor(x, y + TXT_HEIGHT);
  tft.setTextColor(FG_COLOR);
  sprintf(buffer, "%04d", newNumber);

  tft.println(buffer);
}

void Screen::printBigText(char *msg1, char *msg2) {
  int x = (BLOB_W * SQR_TSIZE) / 2 - 36;
  int y = SCR_HEIGHT / 2 - (TXT_HEIGHT * 6 / 2);
  tft.setTextSize(3);

  tft.setCursor(x, y);
  tft.setTextColor(BG_COLOR);
  tft.println(msg1);
  tft.setCursor(x, tft.getCursorY());
  tft.println(msg2);
  x += 2;
  y += 2;
  tft.setCursor(x, y);
  tft.setTextColor(FG_COLOR);
  tft.println(msg1);
  tft.setCursor(x, tft.getCursorY());
  tft.println(msg2);
}
