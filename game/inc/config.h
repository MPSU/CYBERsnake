#define DEBUG false

#define WIDTH 80
#define HEIGHT 30

#define SNACK_ATTEMPTS_LIMIT 100

/*
  Set false, if your platform can't do hardware mul/div/rem operations.
  If set to false, check values of "left_wall_coords" and "right_wall_coords"
  arrays in snake.cpp.
*/
#define HAS_HARD_MUL false

#define WALL_CHAR '|'
#define HEAD_CHAR 'o'
#define HOR_TAIL_CHAR '-'
#define VER_TAIL_CHAR '|'
#define SNACK_CHAR '@'
#define SPACE_CHAR ' '
#define WIN_LENGTH 256

#define UP_KEY      0x1D // W
#define LEFT_KEY    0x1C // A
#define DOWN_KEY    0x1B // S
#define RIGHT_KEY   0x23 // D
#define PAUSE_KEY   0x76 // ESC
#define UNPAUSE_KEY 0x5a // Enter

#define START_TAIL_WIDTH 3
#define START_TAIL_COORD 15 * WIDTH + 10;