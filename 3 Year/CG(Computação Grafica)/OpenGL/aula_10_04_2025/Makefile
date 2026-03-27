CC = g++
CPPFLAGS = -Wall -Wextra -pedantic
LINKEROPENGL = -lglut -lGL -lGLU
LINKERMATH = -lm
SRC = square.cpp # Alterar aqui o nome do aquivo.cpp
TARGET = square # Alterar aqui o nome do executável

all: $(TARGET)

run: 
	./$(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(LINKEROPENGL) $(LINKERMATH) $(CPPFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all run clean