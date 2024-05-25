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
#pragma once
#include "periph.h"
#include <cstdint>
#include <cstddef>

/*
================================================================================
  List of functions that snake.h needs to be defined.
================================================================================

/*
  Function that will be called first in main.
  It used to configure all platform peripherals like input and output devices,
  timers, etc.
*/
void config_periph();

/*
  Function that print passed symbol at specified coord of output device.
  Base function of the entire game graphic.
*/
void print_symbol(size_t coord, uint8_t symbol);

/*
  Function that return current symbol that printed on output device at specified
  coord. Used to backup output device regions while printing game overlays like
  game pause messages.
*/
uint8_t get_symbol(size_t coord);

/*
  Function that get current input from input devices and if that's input is
  new — returns true.
*/
bool get_key(uint8_t &key);

/*
  Function that returns random or pseudorandom value used in snack coordinate
  generation.
*/
size_t get_random_value();

/*
  Function thats seeds random (pseudorandom) generator with new seed.
  If seed is 0, it has to be generated by platform itself.
  Function returns seed that has been in use for the debug sake.
*/
size_t seed_rng(size_t seed);
//==============================================================================


/*
  Function that set's game speed.
*/
void config_timer();

/*
  Function that returns current value of system counter. Used in pseudo-random
  number generation.
*/
size_t get_tick_number();

/*
  Function, defined in shake.h. Base logic of the single game cycle. Called in
  int_handler every timer tick.
*/
extern void game_cycle();

/*
  High level interrupt handler.
*/
extern "C" void int_handler();