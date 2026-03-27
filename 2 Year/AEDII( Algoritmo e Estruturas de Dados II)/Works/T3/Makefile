# Diretórios
SRC_DIR = $(PWD)/src
INCLUDE_DIR = $(PWD)/include
GFX_DIR = $(PWD)/gfx
BUILD_DIR = $(PWD)/build
OBJ_DIR = $(BUILD_DIR)/objects

# Arquivos
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I$(INCLUDE_DIR) -I$(GFX_DIR)
LDFLAGS = -L$(GFX_DIR) -lgfx -lSDL -lSDL_gfx -lSDL_ttf

# Alvo principal
all: $(BUILD_DIR)/main.exe run

# Criação dos diretórios de build
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilação dos objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação do executável
$(BUILD_DIR)/main.exe: $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Executar o arquivo compilado
run: $(BUILD_DIR)/main.exe
	$<

# Limpeza dos arquivos gerados
clean:
	rm -rf $(BUILD_DIR) *~

# Dependências
$(OBJECTS): $(HEADERS)