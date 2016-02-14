#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

const int cols  = 128 / 3;  // 42
const int rows  = 64 / 3;   // 21

const int bytes = (cols >> 3) + 1;

const int top   = 0;
const int left  = 0;

byte  grid[rows][bytes];
byte  next[rows][bytes];
int   generation = 0;

struct _live {
  int y;
  int x;
};

struct _live alive_cells[] = {
  {1, 4}, {1, 5}, {1, 6},
  {2, 4}, {2, 7}, {2, 8}, {2, 9},
  {3, 5}, {3, 7},
  {4, 1}, {4, 2}, {4, 10},
  {5, 1}, {5, 3}, {5, 8}, {5, 11},
  {6, 1}, {6, 10}, {6, 11},
  {7, 2}, {7, 3},
  {8, 2}, {8, 5}, {8, 11}, {8, 13}, {8, 14},
  {9, 2}, {9, 12}, {9, 13}, {9, 15},
  {10, 4}, {10, 6}, {10, 13}, {10, 14}, {10, 17},
  {11, 5}, {11, 6}, {11, 8}, {11, 13}, {11, 14}, {11, 18},
  {12, 9}, {12, 17},
  {13, 8}, {13, 9}, {13, 10}, {13, 11}, {13, 15}, {13, 17},
  {14, 8}, {14, 10}, {14, 11}, {14, 15}, {14, 16}, {14, 17}, {14, 18},
  {15, 9}, {15, 13}, {15, 14}, {15, 16},
  {16, 14}, {16, 15},
  {17, 10}, {17, 12}, {17, 13}, {17, 14},
  {18, 11}, {18, 14},
};

const int num_alive_cells = sizeof(alive_cells) / sizeof(struct _live);

void setup()
{
  int           i, j;
  struct _live* cur = alive_cells;
  
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < bytes; ++j) {
      set_state(i, j, 0);
    }
  }

  for(i = 0; i < num_alive_cells; ++i) {
    set_state(cur->y, cur->x, 1);
    ++cur;
  }  

  u8g.setFont(u8g_font_u8glib_4);
}

void draw()
{
  for(int row = 0; row < rows; ++row) {
    for(int col = 0; col < cols; ++col) {
      if(state(row, col)) {
        u8g.drawBox(left + col * 3, top + row * 3, 2, 2);
      }
    }
  }

  u8g.setPrintPos(116, 63);
  u8g.print(generation);
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
  ++generation;
  
  delay(330);
}

void update_next(void) {
  int i, j, n;
  
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      n = neighbours(i, j);

      if(state(i, j))
        set_new_state(i, j, n == 2 || n == 3);  // Two or three neighbours sustains
      else
        set_new_state(i, j, n == 3);  // Three neighbours is generative
    }
  }  

  for(i = 0; i < rows; ++i) {
    for(j = 0; j < bytes; ++j) {
      grid[i][j] = next[i][j];
    }
  }
}

int neighbours(int row, int col) {
  int n = 0;
  
  for(int r = row - 1; r <= row + 1; ++r) {
    for(int c = col - 1; c <= col + 1; ++c) {
      if((c != col || r != row) && state(r, c))
        ++n;
    }
  }

  return n;
}

boolean state(int row, int col) {
  if(row < 0 || col < 0 || row >= rows || col >= cols)
    return 0;

  int cbyte = col >> 3;
  int cmask = col & 0x7;

  return grid[row][cbyte] & (1 << cmask);
}

void set_state(int row, int col, boolean state) {
  int cbyte = col >> 3;
  int cmask = col & 0x7;

  if(state)
    grid[row][cbyte] |= (1 << cmask);
   else
    grid[row][cbyte] &= ~(1 << cmask);   
}

void set_new_state(int row, int col, boolean state) {
  int cbyte = col >> 3;
  int cmask = col & 0x7;

  if(state)
    next[row][cbyte] |= (1 << cmask);
   else
    next[row][cbyte] &= ~(1 << cmask);   
}

