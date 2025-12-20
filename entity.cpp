#include "entity.h"

void Blob::reset() {
  memset(this->grid, 0, (size_t)BLOB_W * (size_t)BLOB_H * sizeof(this->grid[0][0]));
}

GGrid Blob::grid = { { 0 } };