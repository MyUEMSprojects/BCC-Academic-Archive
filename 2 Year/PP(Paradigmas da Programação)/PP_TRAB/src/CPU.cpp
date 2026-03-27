#include "CPU.h"

CPU::CPU(Memory &mem) : memory(mem), Accumulator(0), InstructionPointer(nullptr), _index(0), id(-1), operand(-1), running(false), _ler(false), _imp(false), command("") {}

// Getters and Setters
int &CPU::getAccumulator()
{
    return this->Accumulator;
}

void CPU::setAccumulator(int Accumulator)
{
    this->Accumulator = Accumulator;
}

int *CPU::getInstructionPointer()
{
    return this->InstructionPointer;
}

void CPU::setInstructionPointer(int *pointer)
{
    this->InstructionPointer = pointer;
}

int CPU::getIndex()
{
    return this->_index;
}

void CPU::setIndex(int index)
{
    this->_index = index;
}

int CPU::getID()
{
    return this->id;
}

int CPU::getOperand()
{
    return this->operand;
}

bool CPU::isRunning()
{
    return this->running;
}

void CPU::setRunning(bool isRunning)
{
    this->running = isRunning;
}

bool CPU::getRunning()
{
    return this->running;
}

void CPU::set_ler(bool _ler)
{
    this->_ler = _ler;
}

bool CPU::get_ler()
{
    return this->_ler;
}

void CPU::set_imp(bool _imp)
{
    this->_imp = _imp;
}

bool CPU::get_imp()
{
    return this->_imp;
}

Memory &CPU::getMemory()
{
    return this->memory;
}

std::string CPU::getCommand()
{
   return this->command;
}

void CPU::setCommand(std::string &newCommand)
{
    this->command = newCommand;
}

//

// Esta função é responsável por ler a próxima instrução da memória, interpretá-la e executá-la.
void CPU::ReadInstructionPointer()
{
    ReadInstruction();
    this->_index++;
    this->InstructionPointer = &this->memory[this->_index];
}

// Esta função é responsável por interpretar a instrução apontada pelo InstructionPointer atual e executar a operação apropriada com base no código de operação (id)
void CPU::ReadInstruction()
{
    this->id = static_cast<int>(*this->InstructionPointer / 100);
    this->operand = *this->InstructionPointer - id * 100;

    if (id == 0)
    {
        command += "PAR";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        par();
    }
    if (id == 11)
    {
        command += "CEA";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        cea(operand);
    }
    if (id == 12)
    {
        command += "CAE";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        cae(operand);
    }
    if (id == 21)
    {
        command += "SOM";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        som(operand);
    }
    if (id == 22)
    {
        command += "SUB";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        sub(operand);
    }
    if (id == 23)
    {
        command += "MUL";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        mul(operand);
    }
    if (id == 24)
    {
        command += "DIV";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        _div(operand);
    }
    if (id == 25)
    {
        command += "MOD";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        mod(operand);
    }
    if (id == 31)
    {
        command += "LER";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        ler(operand);
    }
    if (id == 41)
    {
        command += "IMP";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        imp(operand);
    }
    if (id == 50)
    {
        command += "NOP";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        nop();
    }
    if (id == 51)
    {
        command += "DES";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        des(operand);
    }
    if (id == 52)
    {
        command += "DPO";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        dpo(operand);
    }
    if (id == 53)
    {
        command += "DPZ";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        dpz(operand);
    }
    if (id == 54)
    {
        command += "DNE";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        dne(operand);
    }
    if (id == 55)
    {
        command += "DNZ";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        dnz(operand);
    }
    if (id == 56)
    {
        command += "DDZ";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        ddz(operand);
    }
    if (id == 57)
    {
        command += "DZZ";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        dzz(operand);
    }
    if (id == 61)
    {
        command += "ADE";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";

        ade();
    }
    if (id == 62)
    {
        command += "ADD";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        add();
    }
    if (id == 70)
    {
        command += "PAR";
        command += " ";
        command += std::to_string(this->operand);
        command += "\n";
        par();
    }
}

/*Esta função é responsável por codificar uma instrução em formato legível por máquina em um formato de número inteiro. Ela recebe dois argumentos:
o nome do comando (cmd) e o operando (op) e retorna um número inteiro que representa a instrução codificada
*/
int CPU::Encode(const char cmd[10], const char op[10]) const
{
    int _op = std::atoi(op);
    if (std::strcmp(cmd, "CEA") == 0)
        return 1100 + _op;
    if (std::strcmp(cmd, "CAE") == 0)
        return 1200 + _op;
    if (std::strcmp(cmd, "SOM") == 0)
        return 2100 + _op;
    if (std::strcmp(cmd, "SUB") == 0)
        return 2200 + _op;
    if (std::strcmp(cmd, "MUL") == 0)
        return 2300 + _op;
    if (std::strcmp(cmd, "DIV") == 0)
        return 2400 + _op;
    if (std::strcmp(cmd, "MOD") == 0)
        return 2500 + _op;
    if (std::strcmp(cmd, "LER") == 0)
        return 3100 + _op;
    if (std::strcmp(cmd, "IMP") == 0)
        return 4100 + _op;
    if (std::strcmp(cmd, "NOP") == 0)
        return 5000 + _op;
    if (std::strcmp(cmd, "DES") == 0)
        return 5100 + _op;
    if (std::strcmp(cmd, "DPO") == 0)
        return 5200 + _op;
    if (std::strcmp(cmd, "DPZ") == 0)
        return 5300 + _op;
    if (std::strcmp(cmd, "DNE") == 0)
        return 5400 + _op;
    if (std::strcmp(cmd, "DNZ") == 0)
        return 5500 + _op;
    if (std::strcmp(cmd, "DDZ") == 0)
        return 5600 + _op;
    if (std::strcmp(cmd, "DZZ") == 0)
        return 5700 + _op;
    if (std::strcmp(cmd, "ADE") == 0)
        return 6100 + _op;
    if (std::strcmp(cmd, "ADD") == 0)
        return 6200 + _op;
    if (std::strcmp(cmd, "PAR") == 0)
        return 7000 + _op;
    return 0;
}

// 11: (CEA) Copie o conte´udo do endere¸co EE no acumulador. (AC recebe [EE]).
void CPU::cea(int addr)
{
    this->Accumulator = this->memory.Read(addr);
    this->_ler = false;
    this->_imp = false;
}

// 12: (CAE) Copie o conte´udo do acumulador no endere¸co EE. (EE recebe [AC])
void CPU::cae(int addr)
{
    this->memory.Write(addr, this->Accumulator);
    this->_ler = false;
    this->_imp = false;
}

// 21: (SOM) Some o conte´udo do endere¸co EE com o conte´udo do acumulador e guarde o resultado no acumulador. (AC recebe [AC] + [EE])
void CPU::som(int addr)
{
    this->Accumulator += this->memory.Read(addr);
    this->_ler = false;
    this->_imp = false;
}

// 22: (SUB) Subtraia o conte´udo do endere¸co EE do conte´udo do acumulador e guarde o resultado no acumulador. (AC recebe [AC] - [EE])
void CPU::sub(int addr)
{
    this->Accumulator -= this->memory.Read(addr);
    this->_ler = false;
    this->_imp = false;
}

// 23: (MUL) Multiplique o conte´udo do endere¸co EE com o conte´udo do acumulador e guarde o resultado no acumulador. (AC recebe [AC] * [EE])
void CPU::mul(int addr)
{
    this->Accumulator = this->Accumulator * this->memory.Read(addr);
    this->_ler = false;
    this->_imp = false;
}

// 24: (DIV) Divide o conte´udo do acumulador pelo conte´udo do endere¸co EE e guarde o resultado no acumulador. (AC recebe [AC] / [EE])
void CPU::_div(int addr)
{
    this->Accumulator = static_cast<int>(this->Accumulator / this->memory.Read(addr));
    this->_ler = false;
    this->_imp = false;
}

// 25: (MOD) [AC] recebe o resto da divis˜ao [AC] / [EE].
void CPU::mod(int addr)
{
    this->Accumulator = this->Accumulator % this->memory.Read(addr);
    this->_ler = false;
    this->_imp = false;
}

// 31: (LER) Leia um n´umero e guarde-o no endere¸co EE. (EE recebe o valor lido)
void CPU::ler(int addr)
{
    this->_ler = true;
    this->_imp = false;
}

// 41: (IMP) Imprima o conte´udo do endere¸co EE.
void CPU::imp(int addr)
{
    this->_imp = true;
    this->_ler = false;
}

// 50: (NOP) Nenhuma opera¸c˜ao ´e efetuada.
void CPU::nop()
{
    this->_ler = false;
    this->_imp = false;
}

// 51: (DES) Desvie a execu¸c˜ao para o endere¸co EE, i.e. AI recebe EE.
void CPU::des(int addr)
{
    this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 52: (DPO) Se o conte´udo do acumulador for maior do que zero, desvie a execu¸c˜ao para o endere¸co EE. (Se [AC] > 0, AI recebe EE)
void CPU::dpo(int addr)
{
    if (this->Accumulator > 0)
        this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 53: (DPZ) Se o conte´udo do acumulador for maior ou igual a zero, desvie a execu¸c˜ao para o endere¸co EE. (Se [AC] ≥ 0, AI recebe EE).
void CPU::dpz(int addr)
{
    if (this->Accumulator >= 0)
        this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 54: (DNE) Se o conte´udo do acumulador for menor do que zero, desvie a execu¸c˜ao para o endere¸co EE. (Se [AC] < 0, AI recebe EE.)
void CPU::dne(int addr)
{
    if (this->Accumulator < 0)
        this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 55: (DNZ) Se o conte´udo do acumulador for menor ou igual a zero, desvie a execu¸c˜ao para o endere¸co EE. (Se [AC] ≤ 0, AI recebe EE).
void CPU::dnz(int addr)
{
    if (this->Accumulator <= 0)
        this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 56: (DDZ) Se o conte´udo do acumulador for diferente de zero, desvie a execu¸c˜ao para o endere¸co EE. (Se [AC] 6= 0, AI recebe EE).
void CPU::ddz(int addr)
{
    if (this->Accumulator != 0)
        this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 57: (DZZ) Se o conte´udo do acumulador for igual a zero, desvie a execu¸c˜ao para o endere¸co EE. (Se [AC] = 0, AI recebe EE).
void CPU::dzz(int addr)
{
    if (this->Accumulator == 0)
        this->_index = addr - 1;
    this->_ler = false;
    this->_imp = false;
}

// 61: (ADE) Desloque os digitos do acumulador uma posi¸c˜ao `a esquerda, desprezando o digito mais significativo.
void CPU::ade()
{
    this->Accumulator = this->Accumulator * 10;
    this->_ler = false;
    this->_imp = false;
}

// 62: (ADD) Desloque os digitos do acumulador uma posi¸c˜ao `a direita, desprezando o digito menos significativo
void CPU::add()
{
    this->Accumulator = static_cast<int>(this->Accumulator / 10);
    this->_ler = false;
    this->_imp = false;
}

// 70: (PAR) Pare a execu¸c˜ao do programa. OBS.: Esta instru¸c˜ao deve ser executada para encerrar a execu¸c˜ao do programa
void CPU::par()
{
    //std::cout << "FIM!!!" << std::endl;
    this->_index = -1;
    this->running = false;
    this->_ler = false;
    this->_imp = false;
}