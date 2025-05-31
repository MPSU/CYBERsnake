/* -----------------------------------------------------------------------------
* Project Name   : Architectures of Processor Systems (APS) lab work
* Organization   : National Research University of Electronic Technology (MIET)
* Department     : Institute of Microdevices and Control Systems
* Author(s)      : Andrei Solodovnikov
* Email(s)       : hepoh@org.miet.ru

See https://github.com/MPSU/CYBERsnake/blob/master/LICENSE file for licensing
details.
* ------------------------------------------------------------------------------
*/
#define DEBUG true

#define WIDTH 80
#define HEIGHT 30

#define SNACK_ATTEMPTS_LIMIT 100

#define PS2 0
#define ASCII 1
#ifndef INPUT_TYPE
#define INPUT_TYPE PS2
#endif

#define WALL_CHAR '|'
#define HEAD_CHAR 'o'
#define HOR_TAIL_CHAR '-'
#define VER_TAIL_CHAR '|'
#define SNACK_CHAR '@'
#define SPACE_CHAR ' '
#define WIN_LENGTH 256

#if INPUT_TYPE == PS2
#define UP_KEY      0x1D // W
#define LEFT_KEY    0x1C // A
#define DOWN_KEY    0x1B // S
#define RIGHT_KEY   0x23 // D
#define PAUSE_KEY   0x76 // ESC
#define UNPAUSE_KEY 0x5a // Enter
#elif INPUT_TYPE == ASCII
#define UP_KEY      'w'   // W
#define LEFT_KEY    'a'   // A
#define DOWN_KEY    's'   // S
#define RIGHT_KEY   'd'   // D
#define PAUSE_KEY   0x1B  // ESC
#if defined(_WIN32)
#define UNPAUSE_KEY '\r'  // Enter
#else
#define UNPAUSE_KEY '\n'  // Enter
#endif
#else
#error "You must define INPUT_TYPE as either PS2 or ASCII"
#endif

#define START_TAIL_WIDTH 3
#define START_TAIL_COORD 15 * WIDTH + 10;