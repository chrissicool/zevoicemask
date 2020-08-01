#include <effects/distort.h>

void Distort::apply(unsigned percent)
{
  unsigned rnd = random(100);
  if (rnd > percent)
    return;

  for (unsigned col = 0; col < COLUMNS; col++ ) {
    for (unsigned row = 0; row < ROWS; row++) {
      auto c = GetPixel(col, row);
      if (c == 0)
        continue;

      rnd = random(100);
      if ((rnd * rnd) > (percent * percent))
        continue;

      ShowPixel(col, row, 0);

      unsigned colrow = random(4);
      if (colrow & 1)
        row = constrain(row + random(-1, 2), 0, ROWS);
      if (colrow & 2)
        col = constrain(col + random(-1, 2), 0, COLUMNS);

      ShowPixel(col, row, c);
    }
  }
}

Distort distort;
