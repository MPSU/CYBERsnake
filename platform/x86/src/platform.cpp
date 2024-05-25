#include "platform.h"

std::atomic<bool> active;

// std::vector<std::vector<uint8_t>> char_map(HEIGHT, std::vector<uint8_t>(WIDTH));
uint8_t char_map[HEIGHT][WIDTH];
std::default_random_engine rng;
std::uniform_int_distribution<int> uniform_dist(WIDTH, WIDTH *(HEIGHT - 1));
uint8_t cur_key;

void timer_interrupt()
{
  active = true;
  while (active)
  {
    game_cycle();
    redraw_frame();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

void blocking_input()
{
  while(true)
  {
    cur_key = getch();
  }
}

void redraw_frame()
{
  // Clear terminal with special sequence
  // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
  std::cout << "\x1B[2J\x1B[H";
  for (size_t i = 0; i < HEIGHT; i++)
  {
    for (size_t j = 0; j < WIDTH;  j++)
    {
      std::cout << char_map[i][j];
    }
    std::cout << std::endl;
  }
}

void config_periph()
{
  std::thread t1;
  std::thread t2;
  t1 = std::thread(timer_interrupt);
  t2 = std::thread(blocking_input);
  t1.join();
  t2.join();
}

void print_symbol(size_t coord, uint8_t symbol)
{
  char_map[coord / WIDTH][coord % WIDTH] = symbol;
}

uint8_t get_symbol(size_t coord)
{
  return char_map[coord / WIDTH][coord % WIDTH];
}

bool get_key(uint8_t &key)
{
  if(cur_key)
  {
    key = cur_key;
    cur_key = 0;
    return true;
  }
  else
  {
    return false;
  }
}

size_t get_random_value()
{
  return uniform_dist(rng);
}

size_t seed_rng(size_t seed)
{
  if(seed == 0)
  {
    std::random_device r;
    seed = r();
  }
  rng = std::default_random_engine(seed);
  return seed;
}