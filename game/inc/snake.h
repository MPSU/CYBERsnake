#pragma once

#include "config.h"
#include <cstdint>
#include <cstddef>
#include <array>


/*
================================================================================
  List of functions that must be defined in platform directory.
================================================================================
*/

/*
  Function that will be called first in main.
  It used to configure all platform peripherals like input and output devices,
  timers, etc.
*/
extern void config_periph();

/*
  Function that print passed symbol at specified coord of output device.
  Base function of the entire game graphic.
*/
extern void print_symbol(size_t coord, uint8_t symbol);

/*
  Function that return current symbol that printed on output device at specified
  coord. Used to backup output device regions while printing game overlays like
  game pause messages.
*/
extern uint8_t get_symbol(size_t coord);

/*
  Function that get current input from input devices and if that's input is
  new — returns true.
*/
extern bool get_key(uint8_t &key);

/*
  Function that returns random or pseudorandom value used in snack coordinate
  generation.
*/
extern size_t get_random_value();

/*
  Function thats seeds random (pseudorandom) generator with new seed.
  If seed is 0, it has to be generated by platform itself.
  Function returns seed that has been in use for the debug sake.
*/
extern size_t seed_rng(size_t seed = 0);
//==============================================================================




/*
  Base logic of the single game cycle:
  - get input
  - check game mode. If it's play mode, then:
    - move snake head
    - check whether it's eats snack
    - move snake tail
    - check whether it's collided with obstacle
*/
void game_cycle();




/*
================================================================================
  List of game tasks that called when game mode changes somehow: new game has
  been started, game has been paused/unpaused/winned/overed.
================================================================================
*/

/*
  Function that prepares another game round: seeds rng and redraws map.
  Called after player pressed any key on the new game screen.
*/
void prepare_game();

/*
  Function that print "new game" message and resets game mode flags like:
  game_started, game_finished, game_paused.
  Called at the every round of the game super cycle.
*/
void do_new_game();

/*
  Function that prints "game paused" message (while keeping underlayer part in
  the buffer) and raises game_paused flag.
  Called upon PAUSE_KEY press.
*/
void do_game_pause();

/*
  Function that restores region that has been overwritten by "game paused"
  message from the buffer and resets game_paused flag.
  Called upon UNPAUSE_KEY press.
*/
void do_game_unpause();

/*
  function that prints "game over" message with the player score and moves
  game at the another round by raising game_finished flag.
  Called when snake has collided with some obstacle.
*/
void do_game_over();

/*
  function that prints "win" message and moves game at the another round by
  raising game_finished flag.
  Called when snake length reaches WIN_LENGTH value.
*/
void do_game_finished();

//==============================================================================





/*
================================================================================
  List of graphic tasks that has been built upon print_symbol function.
================================================================================
*/

/*
  Function that fills screen with spaces.
*/
void clear_screen();

/*
  Function that prints 32-bit value in hex format with the '0x' prefix.
  Used for the debug purpose and for the printing player score.
*/
void print_uint32(size_t start_coord, uint32_t number);

/*
  Function that prints first <size> symbols of the <str> from the <start_coord>.
*/
void print_string(const uint8_t *str, const size_t size, size_t start_coord);

/*
  Function that draws WALL_CHAR at the map bounds.
*/
void place_walls();

/*
  Function that gives snack_coord random value and prints SNACK_CHAR at that
  coord.
*/
void place_snack();
//==============================================================================


/*
  Snake direction enum. Represents snake's head direction
*/
enum direction
{
  // We use values that can be used directly in computing new position
  UP    = -WIDTH,
  DOWN  = WIDTH,
  RIGHT = 1,
  LEFT  = -1
};

/*
  There was problem in placing snake coordinates array in the body of the class
  so it has been moved into global space.
  head and tail indexes are the indexes of the ring buffer.
*/
class Snake
{
  public:
  // If snake reach WIN_LENGTH, you win
  size_t length;
  size_t head_index;
  size_t tail_index;
  direction dir;
  Snake();

/*
  Method that process player input.
*/
  void get_input();

  void move_head();
  void move_tail();

/*
  Method that checks whether a something (not necessarily snake) at the given
  coord collides with the walls or the snake itself.
  Used in checking snake collide and snack coordinate generation
*/
  bool has_collided(const size_t coord);

/*
  Method that tells whether some part of the snake placed at specified coord.
*/
  bool is_in_snake(const size_t coord);
};
