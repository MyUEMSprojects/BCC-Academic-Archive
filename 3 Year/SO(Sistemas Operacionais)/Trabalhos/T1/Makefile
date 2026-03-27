TARGET = programa
CC = gcc
CFLAGS = -Iheader # -Wall -Wextra -pedantic  # Flags

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
HEADER_DIR = header

# Arquivos fonte e objeto
SRCS = main.c $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Regra principal
all: $(OBJ_DIR) $(TARGET)

# Cria diretório obj/
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compila .c → .o (na raiz)
$(OBJ_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compila .c → .o (em src/)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar o programa
run: $(TARGET)
	      ./$(TARGET)

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
