CC = g++

CFLAGS = -Wall -Wextra -Werror -std=c++17 -O3 -g -Iincludes
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -li2c

SRC = $(wildcard sources/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = jetracer

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

re: clean all

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
