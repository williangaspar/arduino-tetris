#include "entity.h"

void Blob::reset() {
  memset(this->grid, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(this->grid[0][0]));
}

int8_t Blob::grid[BLOB_W][BLOB_H] = { { 0 } };