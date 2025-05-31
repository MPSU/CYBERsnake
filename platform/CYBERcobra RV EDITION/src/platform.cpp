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
#include "platform.h"
#include "snake.h"

volatile VGA_HANDLE vga = {
  CAST(uint8_t *, 0x07000000),
  CAST(uint8_t *, 0x07001000),
  CAST(uint8_t *, 0x07002000)
};
volatile TIMER_HANDLE *timer_ptr = CAST(struct TIMER_HANDLE *, 0x08000000);
volatile PS2_HANDLE *ps2_ptr = CAST(struct PS2_HANDLE *, 0x03000000);
extern Snake snake;
std::minstd_rand0 rng;

void config_periph()
{
  config_timer();
}

void config_timer()
{
  timer_ptr->delay = 1000000; // 1/5s
  timer_ptr->mode = 2;        // forever
}

void print_symbol(const size_t coord, const uint8_t symbol)
{
  vga.char_map[coord] = symbol;
}

uint8_t get_symbol(const size_t coord)
{
  return vga.char_map[coord];
}

/*
  Function implementation does not handle key release scan codes i.e. it will
  not ignore F0 and following scan-codes. Thus it's better to not use same key
  for several purposes like use ESC key for pause and unpause.
*/
bool get_key(uint8_t &key)
{
  bool res = ps2_ptr->unread_data;
  key = ps2_ptr->scan_code;
  return res;
}

/*
  We need to get a random number started from the top-left non-wall coordinate
  (which is WIDTH+1) to the bottom-right one (which is (WIDTH*(HEIGHT - 1) -1).
*/
size_t get_random_value()
{
  size_t value = WIDTH + 1 + rng() % (WIDTH * (HEIGHT - 2) - 2);
  return value;
}

/*
  Returns current system counter time. Used in seeding RNG (if there is one).
*/
size_t get_tick_number()
{
  return timer_ptr->system_counter;
}

size_t seed_rng(size_t seed)
{
  if(seed == 0)
  {
    seed = get_tick_number();
  }
  rng.seed(seed);
  return seed;
}

extern "C" void int_handler()
{
  game_cycle();
}