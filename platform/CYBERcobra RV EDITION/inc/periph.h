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
#include <stdint.h>
#ifdef __cplusplus
#define CAST(type, addr) reinterpret_cast<type>(addr)
#elif
#define CAST(addr) (type)(addr)
#endif

struct SW_HANDLE
{
  volatile const uint32_t value;
};

struct LED_HANDLE
{
  volatile uint32_t value;
  volatile uint32_t mode;
  volatile const uint32_t __unused__[7];
  volatile uint32_t rst;
};

struct PS2_HANDLE
{
  volatile const uint32_t scan_code;
  volatile const uint32_t unread_data;
  volatile const uint32_t __unused__[7];
  volatile uint32_t rst;
};

struct HEX_HANDLE
{
  uint32_t hex0;
  uint32_t hex1;
  uint32_t hex2;
  uint32_t hex3;
  uint32_t hex4;
  uint32_t hex5;
  uint32_t hex6;
  uint32_t hex7;
  uint32_t bitmask;
  uint32_t rst;
};

struct RX_HANDLE
{
  const uint32_t data;
  const uint32_t unread_data;
  const uint32_t busy;
  uint32_t baudrate;
  uint32_t parity_bit;
  uint32_t stop_bit;
  const uint32_t __unused__[3];
  uint32_t rst;
};

struct TX_HANDLE
{
  uint32_t data;
  const uint32_t __unused1__[1];
  const uint32_t busy;
  uint32_t baudrate;
  uint32_t parity_bit;
  uint32_t stop_bit;
  const uint32_t __unused2__[3];
  uint32_t rst;
};

struct VGA_HANDLE
{
  volatile uint8_t *char_map;
  volatile uint8_t *color_map;
  volatile uint8_t *tiff_map;
};

struct TIMER_HANDLE
{
  volatile const uint32_t system_counter;
  volatile uint64_t delay;
  volatile uint64_t mode;
  volatile uint32_t repeat_counter;
  volatile const uint32_t __unused2__[3];
  volatile uint32_t rst;
};
