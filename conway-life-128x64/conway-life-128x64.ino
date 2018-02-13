#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

// U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

const int cols  = 128 / 2;  // 2 -> 64, 3 -> 42
const int rows  = 64 / 2;   // 2 -> 32, 3 -> 21

const int bytes = (cols >> 3); // + 1 only needed for /3

const int top   = 0;
const int left  = 0;

byte  grid[rows][bytes];
byte  next[rows][bytes];
int   generation = 0;

// New Spaceship
struct _live {
  int y;
  int x;
} alive_cells[] = {
  {1, 7}, {1, 9}, {1, 10},
  {2, 6}, {2, 13},
  {3, 5}, {3, 6}, {3, 10}, {3, 13},
  {4, 2}, {4, 3}, {4, 5}, {4, 11}, {4, 12},
  {5, 2}, {5, 3}, {5, 5}, {5, 11}, {5, 12},
  {6, 5}, {6, 6}, {6, 10}, {6, 13},
  {7, 6}, {7, 13},
  {8, 7}, {8, 9}, {8, 10}
};
 
// glider-gun.life
//struct _live {
//  int y;
//  int x;
//} alive_cells[] = {
//  {1, 25},
//  {2, 23}, {2, 25},
//  {3, 13}, {3, 14}, {3, 21}, {3, 22}, {3, 35}, {3, 36},
//  {4, 12}, {4, 16}, {4, 21}, {4, 22}, {4, 35}, {4, 36},
//  {5, 1}, {5, 2}, {5, 11}, {5, 17}, {5, 21}, {5, 22},
//  {6, 1}, {6, 2}, {6, 11}, {6, 15}, {6, 17}, {6, 18}, {6, 23}, {6, 25},
//  {7, 11}, {7, 17}, {7, 25},
//  {8, 12}, {8, 16},
//  {9, 13}, {9, 14},
//};

//// blinker-puffer.life
//struct _live {
//  int y;
//  int x;
//} alive_cells[] = {
//  {1, 15}, {1, 16}, {1, 17}, {1, 18},
//  {2, 15}, {2, 19},
//  {3, 15},
//  {4, 6}, {4, 7}, {4, 16}, {4, 19},
//  {5, 5}, {5, 6}, {5, 8}, {5, 9},
//  {6, 6}, {6, 7}, {6, 8}, {6, 9}, {6, 13},
//  {7, 7}, {7, 8}, {7, 12}, {7, 14}, {7, 15}, {7, 24}, {7, 29}, {7, 30}, {7, 31},
//  {8, 11}, {8, 15}, {8, 24}, {8, 29}, {8, 31},
//  {9, 7}, {9, 8}, {9, 12}, {9, 14}, {9, 15}, {9, 24}, {9, 29}, {9, 30}, {9, 31},
//  {10, 6}, {10, 7}, {10, 8}, {10, 9}, {10, 13},
//  {11, 5}, {11, 6}, {11, 8}, {11, 9},
//  {12, 6}, {12, 7}, {12, 16}, {12, 19},
//  {13, 15},
//  {14, 15}, {14, 19},
//  {15, 15}, {15, 16}, {15, 17}, {15, 18},
//};


const int num_alive_cells = sizeof(alive_cells) / sizeof(alive_cells[0]);

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
    set_state(cur->y, cur->x);
    ++cur;
  }  

  u8g.setFont(u8g_font_u8glib_4);
}

void draw()
{
  for(int r = 0; r < rows; ++r) {
    for(int c = 0; c < cols; ++c) {
      if(state(r, c)) {
        u8g.drawBox(left + c * 2, top + r * 2, 2, 2);
      }
    }
  }

  u8g.setPrintPos(112, 63);
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

  delay(100);
  
//  delay(330);
}

void update_next(void) {
  int i, j, n;
  
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      n = neighbours(i, j);

      // Two or three neighbours sustains, Three neighbours is generative
      set_new_state(i, j, (state(i, j) && n == 2) || n == 3);  
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

// Set a cell as on
void set_state(int row, int col) {
  int cbyte = col >> 3;
  int cmask = 1 << (col & 7);

  grid[row][cbyte] |= cmask;
}

void set_new_state(int row, int col, boolean state) {
  int cbyte = col >> 3;
  int cmask = 1 << (col & 7);

  if(state)
    next[row][cbyte] |= cmask;
  else
    next[row][cbyte] &= ~cmask;   
}

