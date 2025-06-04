# CYBERsnake

Fully working example of running simple game on single cycle processor, developed on [APS labs](https://github.com/MPSU/APS), though you can try it on different platforms (see `platform` directory).

You can configure game through [config.h](game/inc/config.h).

For adding your platform, check [snake.h](game/inc/snake.h) to see list of functions that must be defined in platform directory.

To build game, one can run `make` from one of the platform subdirectory.
