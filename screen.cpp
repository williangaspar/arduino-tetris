#include "screen.h"

namespace {
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int8_t currentFrame[BLOB_W][BLOB_H];
int8_t nextFrame[BLOB_W][BLOB_H];
}

void Screen::resetFrame(int8_t frame[BLOB_W][BLOB_H]) {
  for (int x = 0; x < BLOB_W; x++) {
    for (int y = 0; y < BLOB_H; y++) {
      frame[x][y] = 0;
    }
  }
}

void Screen::start() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);
  delay(200);
  tft.fillScreen(BG_COLOR);
  tft.drawRect(BLOB_W * SQR_TSIZE, 3, 28, BLOB_H * SQR_TSIZE, FG_COLOR);
  Screen::resetFrame(nextFrame);
}

void Screen::setCursor(int16_t x, int16_t y) {
  tft.setCursor(x, y);
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
  for (int i = 0; i < BLOB_W; i++) {
    for (int j = 0; j < BLOB_H; j++) {
      if (currentFrame[i][j] != nextFrame[i][j]) {
        tft.fillRect(i * SQR_TSIZE, j * SQR_TSIZE, SQR_SIZE, SQR_SIZE, colors[nextFrame[i][j]]);
        currentFrame[i][j] = nextFrame[i][j];
      }
    }
  }
}
