#include "Memory.h"

Memory::Memory()
{
	this->memory = new int[100];
	this->size = 100;
	for (int i = 0; i < 100; i++)
	{
		this->memory[i] = 0;
	}
}

Memory::Memory(int size)
{
	this->size = size;
	this->memory = new int[size];
	for (int i = 0; i < size; i++)
	{
		this->memory[i] = 0;
	}
}

Memory::~Memory()
{
	delete[] this->memory;
}

int &Memory::operator[](int i)
{
	return memory[i];
}

int &Memory::Size()
{
	return this->size;
}

// Le na memória em um indice i
int Memory::Read(int i)
{
	return memory[i];
}

// Escreve na memória em um indice i
void Memory::Write(int i, int value)
{
	memory[i] = value;
}

void Memory::clean()
{
	this->memory = new int[size];
	for (int i = 0; i < size; i++)
	{
		this->memory[i] = 0;
	}
}