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

enum game_state
{
  // We use values that can be used directly in computing new position
  STARTED,
  PAUSED,
  FINISHED
};

game_state state = FINISHED;
volatile uint8_t (*video_memory_2d)[WIDTH];
volatile uint8_t *video_memory_1d;

Snake snake;
size_t snack_coord  = 0;

uint8_t start_str[]   = "Press any key to start new game";
uint8_t pause_str1[]  = "Game paused";
uint8_t pause_str2[]  = "Press Enter to continue";
uint8_t over_str[]    = "Game over. Your score is:         ";
uint8_t win_str[]     = "You won. Your score is           ";

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

uint8_t backup_array[PAUSE_STR1_LEN + PAUSE_STR2_LEN];

int main()
{
  config_periph();
  clear_screen();

  while(1)
  {
    do_new_game();
    while (state != STARTED);
    prepare_game();
    while (state != FINISHED);
  }
  return 0;
}

void clear_screen()
{
  for (size_t i = 0; i < WIDTH * HEIGHT; i++)
  {
    video_memory_1d[i] = ' ';
  }
}

void print_string(const uint8_t *str, const size_t size, size_t start_coord)
{
  for (size_t i = 0; i < size; i++)
  {
    video_memory_1d[start_coord + i] = str[i];
  }
}

void print_uint32(size_t start_coord, uint32_t number)
{
  video_memory_1d[start_coord   ] = '0';
  video_memory_1d[start_coord + 1]= 'x';
  uint8_t cur_digit;
  uint8_t cur_nibble;

  for (size_t i = 0; i < 8; i++)
  {
    cur_nibble = (number >> (i * 4)) & 0xF;
    cur_digit = cur_nibble < 10 ? cur_nibble + '0' : cur_nibble + 'a' - 10;
    // start_coord + 2 (because of 0x) + 7 - i (to place MSB first)
    video_memory_1d[start_coord + 9 - i] = cur_digit;
  }
}

void do_new_game()
{
  print_string(start_str, START_STR_LEN, START_STR_COORD);
  state = FINISHED;
  snake.reset();
}

void prepare_game()
{
  uint32_t seed = seed_rng();
  clear_screen();
  place_walls();
  place_snack();
  snake.reset();
}

void do_game_over()
{
  state = FINISHED;
#if DEBUG == false
  clear_screen();
#endif
  print_string(over_str, OVER_STR_LEN, OVER_STR_COORD);
  print_uint32(OVER_STR_COORD + OVER_STR_LEN - 9, snake.length - START_TAIL_WIDTH);
  return;
}

void do_game_finish()
{
  state = FINISHED;
  clear_screen();
  print_string(win_str, WIN_STR_LEN, WIN_STR_COORD);
}

void do_game_pause()
{
  for (size_t i = 0; i < PAUSE_STR1_LEN; i++)
  {
    backup_array[i] = video_memory_1d[PAUSE_STR1_COORD + i];
  }
  for (size_t i = 0; i < PAUSE_STR2_LEN; i++)
  {
    backup_array[PAUSE_STR1_LEN + i] = video_memory_1d[PAUSE_STR2_COORD + i];
  }
  print_string(pause_str1, PAUSE_STR1_LEN, PAUSE_STR1_COORD);
  print_string(pause_str2, PAUSE_STR2_LEN, PAUSE_STR2_COORD);
  state = PAUSED;
}

void do_game_unpause()
{
  for (size_t i = 0; i < PAUSE_STR1_LEN; i++)
  {
    video_memory_1d[PAUSE_STR1_COORD + i] = backup_array[i];
  }
  for (size_t i = 0; i < PAUSE_STR2_LEN; i++)
  {
    video_memory_1d[PAUSE_STR2_COORD + i] = backup_array[PAUSE_STR1_LEN + i];
  }
  state = STARTED;
}

void game_cycle()
{
  snake.get_input();
  if(state == STARTED)
  {
    snake.move_head();
    if (snake.is_eating(snack_coord))
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
    if (snake.has_head_accident())
    {
      do_game_over();
    }
  }
}

void place_walls()
{

  for(size_t i = 0; i < WIDTH; i++)
  {
    video_memory_2d[0][i] = WALL_CHAR; // place top wall
  }
  for (size_t i = 1; i < HEIGHT - 1; i++)
  {
    video_memory_2d[i][        0] = WALL_CHAR; // place brick of left  wall
    video_memory_2d[i][WIDTH - 1] = WALL_CHAR; // place brick of right wall
  }
  for (size_t i = 0; i < WIDTH; i++)
  {
    video_memory_2d[HEIGHT - 1][i] = WALL_CHAR; // place bottom wall
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
    snack_coord = get_random_value();
    try_count++;
  }
  while(snake.has_collided(snack_coord));
  video_memory_1d[snack_coord] = SNACK_CHAR;
}

Snake::Snake(){}

void Snake::reset()
{
  length = START_TAIL_WIDTH;
  head_index = 0;
  tail_index = 0;
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
    if(state == PAUSED)
    {
      if(key == UNPAUSE_KEY)
      {
        do_game_unpause();
      }
      return;
    }
    state = STARTED;
    direction new_dir;
    new_dir = dir;
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
      do_game_pause();
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
    video_memory_1d[snake_coords[head_index]] = VER_TAIL_CHAR;
  }
  else
  {
    video_memory_1d[snake_coords[head_index]] = HOR_TAIL_CHAR;
  }
  head_index = head_index + 1 < WIN_LENGTH ? head_index + 1 : 0;
  snake_coords[head_index] = cur_pos + dir;
  video_memory_1d[snake_coords[head_index]] = HEAD_CHAR;
}

void Snake::move_tail()
{
  size_t cur_pos = snake_coords[tail_index];
  video_memory_1d[snake_coords[tail_index]] = SPACE_CHAR;
  tail_index = tail_index + 1 < WIN_LENGTH ? tail_index + 1 : 0;
}

bool Snake::is_in_snake(const size_t coord)
{
  bool res = 0;
  for (size_t i = tail_index; i != head_index; i = (i + 1)% WIN_LENGTH)
  {
    res |= coord == snake_coords[i];
  }
  return res;
}

bool Snake::is_eating(const size_t coord)
{
  return snake_coords[head_index] == coord;
}

bool Snake::has_collided(const size_t coord)
{
  size_t top                = WIDTH;
  size_t bottom             = WIDTH * (HEIGHT-1);
  bool top_collide          = coord < top;
  bool bottom_collide       = coord >= bottom;
  bool left_collide         = false;
  bool right_collide        = false;
  size_t wrapped_coord = coord % WIDTH;
  left_collide  = wrapped_coord == 0;
  right_collide = wrapped_coord == WIDTH - 1;
  bool self_collide = is_in_snake(coord);
#if DEBUG == true
  if(left_collide)    video_memory_2d[HEIGHT - 2][WIDTH - 4] = 'l';
  if(right_collide)   video_memory_2d[HEIGHT - 2][WIDTH - 4] = 'r';
  if(top_collide)     video_memory_2d[HEIGHT - 2][WIDTH - 4] = 't';
  if(bottom_collide)  video_memory_2d[HEIGHT - 2][WIDTH - 4] = 'b';
  if(self_collide)    video_memory_2d[HEIGHT - 2][WIDTH - 4] = 's';
#endif
  return top_collide || bottom_collide || left_collide || right_collide || self_collide;
}

bool Snake:: has_head_accident()
{
  return has_collided(snake_coords[head_index]);
}
