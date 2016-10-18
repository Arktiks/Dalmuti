CC     = g++
CFLAGS = -std=c++11
DEBUG  ?= 0

SOURCE       = Source/Main.cpp Source/Player.cpp Source/GameManager.cpp Source/Parser.cpp
AI_SOURCE    = $(wildcard Source/AI/*.cpp)
DEBUG_SOURCE = Source/Logging/Log.cpp Source/Logging/Memory.cpp

ifeq ($(DEBUG), 1)
	CFLAGS += -g -D_DEBUG
	SOURCE += $(DEBUG_SOURCE)
else
	CFLAGS += -DNDEBUG
endif

Dalmuti: $(SOURCE)
	$(CC) $(SOURCE) $(AI_SOURCE) $(CFLAGS) -o $@.out
