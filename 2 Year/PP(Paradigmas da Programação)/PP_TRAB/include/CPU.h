#pragma once

#include "Memory.h"

class CPU {
private:
    Memory memory; // Memoria
    int Accumulator; // Acumulador
    int* InstructionPointer; // Ponteiro para a instrução
    int _index; // index(linha) da instrução

    int id;
    int operand;

    bool running;
    bool _ler;
    bool _imp;

    std::string command;

public:
    CPU(Memory& mem);
    int& getAccumulator();
    void setAccumulator(int Accumulator);
    int* getInstructionPointer();
    void setInstructionPointer(int* pointer);
    int getIndex();
    void setIndex(int index);
    int getID();
    int getOperand();
    bool isRunning();
    void setRunning(bool isRunning);
    bool getRunning();
    void set_ler(bool _ler);
    bool get_ler();
    void set_imp(bool _imp);
    bool get_imp();
    Memory& getMemory();
    std::string getCommand();
    void setCommand(std::string &newCommand);
    void ReadInstructionPointer();
    void ReadInstruction();
    int Encode(const char cmd[10], const char op[10]) const;
    void cea(int addr);
    void cae(int addr);
    void som(int addr);
    void sub(int addr);
    void mul(int addr);
    void _div(int addr);
    void mod(int addr);
    void ler(int addr);
    void imp(int addr);
    void nop();
    void des(int addr);
    void dpo(int addr);
    void dpz(int addr);
    void dne(int addr);
    void dnz(int addr);
    void ddz(int addr);
    void dzz(int addr);
    void ade();
    void add();
    void par();
};
