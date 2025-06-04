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

#if !defined(_WIN32)
bool _kbhit()
{
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

int _getch()
{
  unsigned char ch;
  if (read(STDIN_FILENO, &ch, 1) == 1)
    return ch;
  return -1;
}

static struct termios orig_termios;

void reset_terminal_mode()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
  std::cout << "\033[?25h"; // show cursor
  std::cout.flush();
}

void set_terminal_mode()
{
  struct termios new_termios;
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(reset_terminal_mode);

  new_termios = orig_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);
  new_termios.c_cc[VMIN] = 0;
  new_termios.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
  std::cout << "\033[2J\033[H\033[?25l";
  std::cout.flush();
}
#endif

std::atomic<bool> active;
std::atomic<uint8_t> cur_key = 0;
extern volatile uint8_t (*video_memory_2d)[WIDTH];
extern volatile uint8_t *video_memory_1d;
volatile uint8_t char_map[SCREEN_AREA];

std::default_random_engine rng;
std::uniform_int_distribution<size_t> uniform_dist(WIDTH, WIDTH *(HEIGHT - 1));

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
      int ch = _getch();
      cur_key = static_cast<uint8_t>(ch);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void config_periph()
{
  video_memory_1d = char_map;
  video_memory_2d = reinterpret_cast<volatile uint8_t(*)[WIDTH]>(char_map);

#if !defined(_WIN32)
  set_terminal_mode();
#endif

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
      frame += video_memory_2d[i][j];
    }
    frame += '\n';
  }

  std::cout << frame;
  std::cout.flush();
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

void seed_rng(size_t seed)
{
  if(seed == 0)
  {
    std::random_device r;
    seed = r();
  }
  rng = std::default_random_engine(seed);
}
