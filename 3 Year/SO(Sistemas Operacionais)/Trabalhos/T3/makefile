# Nome do compilador
CXX = gcc

# Flags do compilador
CXXFLAGS = -Wall #-Wextra -pedantic

# Regra padrão (compilar um arquivo .c para o executável com nome especificado)
%: %.c
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

# Limpar executáveis gerados
clean:
	rm -f $(basename $(wildcard *.c))
