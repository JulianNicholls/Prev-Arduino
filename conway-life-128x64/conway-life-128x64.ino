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
  {1, 25},
  {2, 23}, {2, 25},
  {3, 13}, {3, 14}, {3, 21}, {3, 22}, {3, 35}, {3, 36},
  {4, 12}, {4, 16}, {4, 21}, {4, 22}, {4, 35}, {4, 36},
  {5, 1}, {5, 2}, {5, 11}, {5, 17}, {5, 21}, {5, 22},
  {6, 1}, {6, 2}, {6, 11}, {6, 15}, {6, 17}, {6, 18}, {6, 23}, {6, 25},
  {7, 11}, {7, 17}, {7, 25},
  {8, 12}, {8, 16},
  {9, 13}, {9, 14},
};

const int num_alive_cells = sizeof(alive_cells) / sizeof(struct _live);

void setup()
{
  int           i, j;
  struct _live* cur = alive_cells;
  
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < bytes; ++j) {
      grid[i][j] = 0;
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
  for(int r = 0; r < rows; ++r) {
    for(int c = 0; c < cols; ++c) {
      if(state(r, c)) {
        u8g.drawBox(left + c * 3, top + r * 3, 2, 2);
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
  
  delay(50);
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
  int cmask = 1 << (col & 7);

  return grid[row][cbyte] & cmask;
}

void set_state(int row, int col, boolean state) {
  int cbyte = col >> 3;
  int cmask = 1 << (col & 7);

  if(state)
    grid[row][cbyte] |= cmask;
  else
    grid[row][cbyte] &= cmask;
}

void set_new_state(int row, int col, boolean state) {
  int cbyte = col >> 3;
  int cmask = 1 << (col & 7);

  if(state)
    next[row][cbyte] |= cmask;
  else
    next[row][cbyte] &= ~cmask;   
}

