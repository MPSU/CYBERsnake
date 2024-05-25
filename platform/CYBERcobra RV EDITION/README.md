# CYBERcobra RV EDITION CYBERsnake implementation

Fully working example of running simple game on single cycle processor,
developed on [APS labs](https://github.com/MPSU/APS).

It's uses three peripheral units: PS/2 keyboard, VGA controller and system timer.
Altho PS/2 keyboard can generate interrupt requests, game logic solely bases on
timer interrupts.

You can configure game through [config.h](../../game/inc/config.h).

To build game, one can run `make` from this directory, which produce
`snake_instr.mem` and `snake_data.mem` files. To flash SoCs memories, there is
`flash.py` script, that has two mandatory arguments and four optional ones.

Usage:

```bash
python flash.py [-h] [-d DATA] [-c COLOR] [-s SYMBOLS] [-t TIFF] instr comport
```

Where instr is snake_instr.mem, DATA is snake_data.mem, COLOR is
"vga mem files/color_map.mem", SYMBOLS is "vga mem files/char_map.mem" and TIFF
is "vga mem files/char_tiff.mem". Comport is serial port processor connected to
your PC through.

Game controlled by 6 keys: `WASD` for snake direction control, `ESC` for pause and
`Enter` for unpause.
