CC      =   g++

OUTPUT      = snake.exe

INC_DIRS    = ./inc/ ../../game/inc/
CC_FLAGS    = -std=c++17 $(addprefix -I, $(INC_DIRS))
.PHONY: clean

$(OUTPUT): ./inc/platform.h ./src/platform.cpp ../../game/inc/config.h ../../game/inc/snake.h ../../game/src/snake.cpp
	${CC} $(CC_FLAGS) ./src/platform.cpp ../../game/src/snake.cpp -o $@


clean:
	rm -f $(OUTPUT)