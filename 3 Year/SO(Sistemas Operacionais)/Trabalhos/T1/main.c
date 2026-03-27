/*
* Trabalho do Terminal de processos(T1)
* Aluno: Felipe Echeverria Vilhalva RGM: 45611
* Professor: Fabricio Sergio De Paula - Sistemas Operacionais
*/
#include <terminal.h>

int main()
{
    char command[MAX_ARG_SIZE];
    system("clear");
    print_welcome();
    print_interpreter();
    
    while (1)
    {		
        terminal(command, sizeof(command));
    }
    
    return 0;
}
