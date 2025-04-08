SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := build

EXE  := $(BIN_DIR)/seesnake
SRC  := $(wildcard $(SRC_DIR)/*.c)
OBJ  := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC       := gcc
CFLAGS 	 := -std=c11 -ggdb3 -O2 -Wall -Wextra -Wpedantic
LDFLAGS	 := -Llib/raylib 
LDLIBS	 := -l:libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
CPPFLAGS := -Iinclude -MMD -MP -DDEBUG

.PHONY: all clean build web

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(EXE) $(OBJ)

-include $(OBJ:.o=.d)
