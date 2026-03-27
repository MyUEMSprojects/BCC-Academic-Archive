# Configurações básicas
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = out
SRC = main.cpp

# Configurações de biblioteca por sistema operacional
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
    LIBS = -lglut -lGLU -lGL
endif
ifeq ($(UNAME), Darwin)  # macOS
    LIBS = -framework GLUT -framework OpenGL
endif

# Regra padrão
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Limpeza
clean:
	rm -f $(TARGET)

# Executar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run