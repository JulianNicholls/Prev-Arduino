#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

const int rows  = 64 / 3;
const int cols  = 128 / 4;
const int top   = 0;
const int left  = 0;

const int DEAD  = false;
const int ALIVE = true;
 
boolean grid[rows][cols];

struct _live {
  int y;
  int x;
};

struct _live alive_cells[] = {
  {1, 6}, {1, 7}, {1, 8}, {1, 20}, {1, 21}, {1, 22},
  {2, 5}, {2, 9}, {2, 19}, {2, 23},
  {3, 4}, {3, 5}, {3, 10}, {3, 18}, {3, 23}, {3, 24},
  {4, 3}, {4, 5}, {4, 7}, {4, 8}, {4, 10}, {4, 11}, {4, 17}, {4, 18}, {4, 20}, {4, 21}, {4, 23}, {4, 25},
  {5, 2}, {5, 3}, {5, 5}, {5, 10}, {5, 12}, {5, 13}, {5, 15}, {5, 16}, {5, 18}, {5, 23}, {5, 25}, {5, 26},
  {6, 1}, {6, 6}, {6, 10}, {6, 13}, {6, 15}, {6, 18}, {6, 22}, {6, 27},
  {7, 13}, {7, 15},
  {8, 1}, {8, 2}, {8, 10}, {8, 11}, {8, 13}, {8, 15}, {8, 17}, {8, 18}, {8, 26}, {8, 27},
  {9, 13}, {9, 15},
  {10, 7}, {10, 8}, {10, 9}, {10, 19}, {10, 20}, {10, 21},
  {11, 7}, {11, 11}, {11, 21},
  {12, 7}, {12, 9}, {12, 14}, {12, 15}, {12, 16},
  {13, 13}, {13, 16}, {13, 21}, {13, 22},
  {14, 16},
  {15, 12}, {15, 16},
  {16, 12}, {16, 16},
  {17, 16},
  {18, 13}, {18, 15},
};

const int num_alive_cells = sizeof(alive_cells) / sizeof(struct _live);

void draw()
{
  for(int row = 0; row < rows; ++row) {
    for(int col = 0; col < cols; ++col) {
      if(grid[row][col] == ALIVE) {
        u8g.drawBox(left + col * 4, top + row * 3, 3, 2);
      }
    }
  }
  
}

void setup()
{
  int           i, j;
  struct _live* cur = alive_cells;
  
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      grid[i][j] = DEAD;
    }
  }

  for(i = 0; i < num_alive_cells; ++i) {
    grid[cur->y][cur->x] = ALIVE;
    ++cur;
  }  
}

void loop()
{
// Picture loop, required by u8g
  u8g.firstPage();

  do {
    draw();
  }
  while(u8g.nextPage());

  update_next();
  
  delay(500);
}

void update_next(void) {
  boolean next[rows][cols];
  int     i, j, n;
  
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      n = neighbours(i, j);

      if(grid[i][j] == ALIVE)
        next[i][j] = (n == 2 || n == 3);  // Two or three neighbours sustains
      else
        next[i][j] = (n == 3);            // Three neighbours is generative
    }
  }  

  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      grid[i][j] = next[i][j];
    }
  }
}

int neighbours(int row, int col) {
  int n = 0;
  
  for(int i = row - 1; i <= row + 1; ++i) {
    for(int j = col - 1; j <= col + 1; ++j) {
      if(state(i, j) == ALIVE)
        ++n;
    }
  }

  return n;
}

boolean state(int row, int col) {
  if(row < 0 || col < 0 || row >= rows || col >= cols)
    return DEAD;

  return grid[row][col];
}


