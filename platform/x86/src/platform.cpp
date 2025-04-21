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

std::atomic<bool> active;
std::atomic<uint8_t> cur_key = 0;
uint8_t char_map[HEIGHT][WIDTH];

std::default_random_engine rng;
std::uniform_int_distribution<int> uniform_dist(WIDTH, WIDTH *(HEIGHT - 1));

void timer_interrupt()
{
  active = true;
  while (active)
  {
    game_cycle();
    redraw_frame();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

void blocking_input()
{
  while (active)
  {
    if (_kbhit())
    {
      cur_key = _getch();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void config_periph()
{
  for (size_t i = 0; i < HEIGHT; ++i)
    for (size_t j = 0; j < WIDTH; ++j)
      char_map[i][j] = SPACE_CHAR;

  std::thread(timer_interrupt).detach();
  std::thread(blocking_input).detach();
}

void redraw_frame()
{
  // Clear terminal with special sequence
  // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
  std::string frame;
  frame.reserve(WIDTH * HEIGHT + HEIGHT + 10);

  frame += "\x1B[2J\x1B[H"; // clear + home
  for (size_t i = 0; i < HEIGHT; i++)
  {
    for (size_t j = 0; j < WIDTH; j++)
    {
      frame += char_map[i][j];
    }
    frame += '\n';
  }

  std::cout << frame;
  std::cout.flush();
}

void print_symbol(size_t coord, uint8_t symbol)
{
  size_t row = coord / WIDTH;
  size_t col = coord % WIDTH;
  if (row < HEIGHT && col < WIDTH)
  {
    char_map[row][col] = symbol;
  }
}

uint8_t get_symbol(size_t coord)
{
  size_t row = coord / WIDTH;
  size_t col = coord % WIDTH;
  if (row < HEIGHT && col < WIDTH)
  {
    return char_map[row][col];
  }
  return SPACE_CHAR;
}

bool get_key(uint8_t &key)
{
  if(cur_key)
  {
    key = cur_key;
    cur_key = 0;
    return true;
  }
  return false;
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
