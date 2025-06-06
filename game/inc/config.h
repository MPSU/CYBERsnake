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
#include <cstddef>
#include <cstdint>

#define DEBUG true

constexpr size_t WIDTH = 80;
constexpr size_t HEIGHT = 30;
constexpr size_t SCREEN_AREA = WIDTH * HEIGHT;
constexpr size_t WIN_LENGTH = 256;

constexpr size_t START_TAIL_WIDTH = 3;
constexpr size_t START_TAIL_COORD = 15 * WIDTH + 10;
constexpr size_t SNACK_ATTEMPTS_LIMIT = 100;

constexpr uint8_t WALL_CHAR = '|';
constexpr uint8_t HEAD_CHAR = 'o';
constexpr uint8_t HOR_TAIL_CHAR = '-';
constexpr uint8_t VER_TAIL_CHAR = '|';
constexpr uint8_t SNACK_CHAR = '@';
constexpr uint8_t SPACE_CHAR = ' ';

#define PS2 0
#define ASCII 1
#ifndef INPUT_TYPE
#define INPUT_TYPE PS2
#endif

#if INPUT_TYPE == PS2
constexpr uint8_t UP_KEY      = 0x1D; // W
constexpr uint8_t LEFT_KEY    = 0x1C; // A
constexpr uint8_t DOWN_KEY    = 0x1B; // S
constexpr uint8_t RIGHT_KEY   = 0x23; // D
constexpr uint8_t PAUSE_KEY   = 0x76; // ESC
constexpr uint8_t UNPAUSE_KEY = 0x5a; // Enter
#elif INPUT_TYPE == ASCII
constexpr uint8_t UP_KEY      = 'w' ;  // W
constexpr uint8_t LEFT_KEY    = 'a' ;  // A
constexpr uint8_t DOWN_KEY    = 's' ;  // S
constexpr uint8_t RIGHT_KEY   = 'd' ;  // D
constexpr uint8_t PAUSE_KEY   = 0x1B;  // ESC
#if defined(_WIN32)
constexpr uint8_t UNPAUSE_KEY = '\r'; // Enter
#else
constexpr uint8_t UNPAUSE_KEY = '\n'; // Enter
#endif
#else
#error "You must define INPUT_TYPE as either PS2 or ASCII"
#endif
