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
#include "snake.h"

bool game_started   = false;
bool game_finished  = false;
bool game_paused    = false;

size_t snack_coord  = 0;

Snake snake;
// If snake reach WIN_LENGTH, you win
uint16_t snake_coords[WIN_LENGTH];


uint8_t start_str[] = "Press any key to start new game";
uint8_t pause_str1[] = "Game paused";
uint8_t pause_str2[] = "Press Enter to continue";
uint8_t over_str[] = "Game over. Your score is:         ";
uint8_t win_str[] = "You won. Your score is           ";

#if DEBUG == true
uint8_t stuck_str[] = "Stuck at generating snack. It's coord is:          ";
#endif

#define START_STR_LEN   (std::size(start_str) - 1)
#define PAUSE_STR1_LEN  (std::size(pause_str1) - 1)
#define PAUSE_STR2_LEN  (std::size(pause_str2) - 1)
#define OVER_STR_LEN    (std::size(over_str) - 1)
#define WIN_STR_LEN     (std::size(win_str) - 1)

#define START_STR_COORD   (WIDTH * HEIGHT / 2 + WIDTH / 2 - START_STR_LEN / 2)
#define PAUSE_STR1_COORD  (WIDTH * HEIGHT / 2 - WIDTH / 2 - PAUSE_STR1_LEN / 2)
#define PAUSE_STR2_COORD  (WIDTH * HEIGHT / 2 + WIDTH / 2 - PAUSE_STR2_LEN / 2)
#define OVER_STR_COORD    (WIDTH * HEIGHT / 2 - WIDTH / 2 - OVER_STR_LEN / 2)
#define WIN_STR_COORD     (WIDTH * HEIGHT / 2 - WIDTH / 2 - WIN_STR_LEN / 2)

uint8_t backup_array[PAUSE_STR1_LEN + PAUSE_STR1_LEN];

#if HAS_HARD_MUL == false
/*
  If device can't do rem operation, we need to precompute that values in order
  to check whether object collided with walls.
  left_wall_coords can be computed in python as follows:
    print(list(range(WIDTH,WIDTH*(HEIGHT-1),WIDTH)))
  right_wall_coords can be computed in python as follows:
    print(list(range(WIDTH*2-1,WIDTH*(HEIGHT-1),WIDTH)))
*/
uint16_t left_wall_coords[HEIGHT - 2]  = { 80, 160, 240, 320, 400, 480, 560, 640, 720, 800, 880, 960, 1040, 1120, 1200, 1280, 1360, 1440, 1520, 1600, 1680, 1760, 1840, 1920, 2000, 2080, 2160, 2240};
uint16_t right_wall_coords[HEIGHT - 2] = {159, 239, 319, 399, 479, 559, 639, 719, 799, 879, 959, 1039, 1119, 1199, 1279, 1359, 1439, 1519, 1599, 1679, 1759, 1839, 1919, 1999, 2079, 2159, 2239, 2319};
#endif

int main()
{
  config_periph();
  clear_screen();

  while(1)
  {
    do_new_game();
    while (!game_started);
    prepare_game();
    while (!game_finished);
  }
  return 0;
}

void clear_screen()
{
  for (size_t i = 0; i < WIDTH * HEIGHT; i++)
  {
    print_symbol(i, ' ');
  }
}

void print_string(const uint8_t *str, const size_t size, size_t start_coord)
{
  for (size_t i = 0; i < size; i++)
  {
    print_symbol(start_coord + i, str[i]);
  }
}

void print_uint32(size_t start_coord, uint32_t number)
{
  print_symbol(start_coord, '0');
  print_symbol(start_coord+1, 'x');
  uint8_t cur_digit;
  uint8_t cur_nibble;

  for (size_t i = 0; i < 8; i++)
  {
    cur_nibble = (number >> (i * 4)) & 0xF;
    cur_digit = cur_nibble < 10 ? cur_nibble + '0' : cur_nibble + 'a' - 10;
    // start_coord + 2 (because of 0x) + 7 - i (to place MSB first)
    print_symbol(start_coord + 9 - i, cur_digit);
  }
}

void do_new_game()
{
  print_string(start_str, START_STR_LEN, START_STR_COORD);
  game_started  = false;
  game_finished = false;
  game_paused   = false;
  snake = Snake();
}

void prepare_game()
{
  uint32_t seed = seed_rng();
  clear_screen();
  place_walls();
  place_snack();
  snake = Snake();
}

void do_game_over()
{
  game_finished = true;
#if DEBUG == false
  clear_screen();
#endif
  print_string(over_str, OVER_STR_LEN, OVER_STR_COORD);
  print_uint32(OVER_STR_COORD + OVER_STR_LEN - 9, snake.length - START_TAIL_WIDTH);
  return;
}

void do_game_finish()
{
  game_finished = true;
  clear_screen();
  print_string(win_str, WIN_STR_LEN, WIN_STR_COORD);
}

void do_game_pause()
{
  for (size_t i = PAUSE_STR1_COORD; i < PAUSE_STR1_COORD + PAUSE_STR1_LEN; i++)
  {
    backup_array[i] = get_symbol(i);
  }
  for (size_t i = PAUSE_STR2_COORD; i < PAUSE_STR2_COORD + PAUSE_STR2_LEN; i++)
  {
    backup_array[PAUSE_STR1_LEN+i] = get_symbol(i);
  }
  print_string(pause_str1, PAUSE_STR1_LEN, PAUSE_STR1_COORD);
  print_string(pause_str2, PAUSE_STR2_LEN, PAUSE_STR2_COORD);
  game_paused = true;
}

void do_game_unpause()
{
  for (size_t i = PAUSE_STR1_COORD; i < PAUSE_STR1_COORD + PAUSE_STR1_LEN; i++)
  {
    print_symbol(i, backup_array[i]);
  }
  for (size_t i = PAUSE_STR2_COORD; i < PAUSE_STR2_COORD + PAUSE_STR2_LEN; i++)
  {
    print_symbol(i, backup_array[PAUSE_STR1_LEN + i]);
  }
  game_paused = false;
}

void game_cycle()
{
  snake.get_input();
  if(game_started && !game_finished && !game_paused)
  {
    snake.move_head();
    if (snake_coords[snake.head_index] == snack_coord)
    {
      snake.length++;
      if(snake.length == WIN_LENGTH)
      {
        do_game_finish();
      }
      else
      {
        place_snack();
      }
    }
    else
    {
      snake.move_tail();
    }
    if (snake.has_collided(snake_coords[snake.head_index]))
    {
      do_game_over();
    }
  }
}

void place_walls()
{

  for(size_t i = 0; i < WIDTH; i++)
  {
    print_symbol(i, WALL_CHAR); // place top wall
  }
  for (size_t i = 1; i < HEIGHT - 1; i++)
  {
#if HAS_HARD_MUL == true
    size_t left_coord = i * WIDTH;
#else
    // ((i<<2)+i)<<4  = (i*4+i) * 16 = i*5 * 16 = i * 80 = i * WIDTH
    size_t left_coord = ((i << 2) + i) << 4;
#endif
    size_t right_coord = left_coord + (WIDTH - 1);
    print_symbol(left_coord, WALL_CHAR);          // place brick of left  wall
    print_symbol(right_coord, WALL_CHAR);         // place brick of right wall
  }
  for (size_t i = WIDTH * (HEIGHT - 1); i < WIDTH * HEIGHT; i++)
  {
    print_symbol(i, WALL_CHAR); // place bottom wall
  }
}

void place_snack()
{
  size_t try_count = 0;
  do
  {
#if DEBUG == true
    if (try_count > SNACK_ATTEMPTS_LIMIT)
    {
      print_string(stuck_str, std::size(stuck_str) - 1, WIDTH + 1);
      print_uint32(WIDTH + std::size(stuck_str) - 9, snack_coord);
    }
#endif
    snack_coord += get_random_value();
    // if snack coords went out of map, wrap it back
    if(snack_coord > (WIDTH * (HEIGHT-1)))
    {
      snack_coord -= WIDTH * (HEIGHT - 1);
    }
    try_count++;

  }
  while(snake.has_collided(snack_coord));
  print_symbol(snack_coord, SNACK_CHAR);
}

Snake::Snake(){
  length      = START_TAIL_WIDTH;
  head_index  = 0;
  tail_index  = 0;
  dir = RIGHT;
  snake_coords[tail_index] = START_TAIL_COORD;
  for (size_t i = 0; i < length; i++)
  {
    move_head();
  }
}

void Snake::get_input()
{
  uint8_t key;
  if (get_key(key))
  {
    direction new_dir;
    new_dir = dir;
    game_started = true;
    switch (key)
    {
    case UP_KEY:
      new_dir = UP;
      break;
    case LEFT_KEY:
      new_dir = LEFT;
      break;
    case DOWN_KEY:
      new_dir = DOWN;
      break;
    case RIGHT_KEY:
      new_dir = RIGHT;
      break;
    case PAUSE_KEY:
      if(!game_paused)
      {
        do_game_pause();
      }
      break;
    case UNPAUSE_KEY:
      if(game_paused)
      {
        do_game_unpause();
      }
      break;
    }
    if((new_dir != -dir) && (new_dir != dir))
    {
      dir = new_dir;
    }
  }
}

void Snake::move_head()
{
  size_t cur_pos = snake_coords[head_index];
  if((dir == UP) || (dir == DOWN))
  {
    print_symbol(snake_coords[head_index], VER_TAIL_CHAR);
  }
  else
  {
    print_symbol(snake_coords[head_index], HOR_TAIL_CHAR);
  }
  head_index = head_index + 1 < WIN_LENGTH ? head_index + 1 : 0;
  snake_coords[head_index] = cur_pos + dir;
  print_symbol(snake_coords[head_index], HEAD_CHAR);
}

void Snake::move_tail()
{
  size_t cur_pos = snake_coords[tail_index];
  print_symbol(snake_coords[tail_index], SPACE_CHAR);
  tail_index = tail_index + 1 < WIN_LENGTH ? tail_index + 1 : 0;
}

bool Snake::is_in_snake(const size_t coord)
{
  bool res = 0;
  for (size_t i = tail_index; i != head_index; i = i + 1 < WIN_LENGTH ? i + 1 : 0)
  {
    res |= coord == snake_coords[i];
  }
  return res;
}

bool Snake::has_collided(const size_t coord)
{
  size_t top                = WIDTH;
  size_t bottom             = WIDTH * (HEIGHT-1);
  bool top_collide          = coord < top;
  bool bottom_collide       = coord >= bottom;
  bool left_collide         = false;
  bool right_collide        = false;
#if HAS_HARD_MUL == true
  size_t wrapped_coord = coord % WIDTH;
  left_collide  = wrapped_coord == 0;
  right_collide = wrapped_coord == WIDTH - 1;
#else
  for (size_t i = 0; i < HEIGHT - 2; i++)
  {
    left_collide  |= coord == left_wall_coords[i];
    right_collide |= coord == right_wall_coords[i];
  }
#endif
  bool self_collide = is_in_snake(coord);
#if DEBUG == true
  if(left_collide)    print_symbol(WIDTH * (HEIGHT - 1) - 4, 'l');
  if(right_collide)   print_symbol(WIDTH * (HEIGHT - 1) - 4, 'r');
  if(top_collide)     print_symbol(WIDTH * (HEIGHT - 1) - 4, 't');
  if(bottom_collide)  print_symbol(WIDTH * (HEIGHT - 1) - 4, 'b');
  if(self_collide)    print_symbol(WIDTH * (HEIGHT - 1) - 4, 's');
#endif
  return top_collide || bottom_collide || left_collide || right_collide || self_collide;
}
