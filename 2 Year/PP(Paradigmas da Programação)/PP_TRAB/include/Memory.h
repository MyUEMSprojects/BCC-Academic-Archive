#pragma once
#include "defaultLibrary.h"

class Memory
{
private:
	int* memory;
	int size;
public:

    // Construtor padrão
    Memory();

    // Construtor de memória n
    Memory(int size);

    ~Memory();

    int& operator[](int i);

    int& Size();

    int Read(int i);

    void Write(int i, int value);

    void clean();
};
