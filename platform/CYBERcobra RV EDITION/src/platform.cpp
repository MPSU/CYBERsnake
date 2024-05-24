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

void config_periph()
{
  config_timer();
}

void config_timer()
{
  timer_ptr->delay = 2000000; // 1/5s
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
  Actually, there is simple PRNG, known as minstd_rand0 defined in <random>
  but it seems to be too hard to work with this MCU.
  So we need to get entropy from user input i.e. time when player reach snake
  and dir of the snake at that moment. Since every game frame is happen at fixed
  discretes of time, there is no much entropy in low bits of system timer.
  We mix to entropy sources (system_counter and snake dir) and result value mask
  with low 11 bits, because there is 2184 free coordinates at map 80x30
  (80*30-80*2-30*2+4) and we need to get value in that range.
*/
size_t get_random_value()
{
  return ((timer_ptr->system_counter >> 20) ^ snake.dir) & 0x7FF;
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
  return seed;
}

extern "C" void int_handler()
{
  game_cycle();
}