#ifndef TERMINAL_H
#define TERMINAL_H

/*
 * header dedicado a organização do terminal em funções/módulos
 * */

#include <arvore.h>

// printa o "bem vindo" estilizado
void print_welcome();

// printa o "interpretador" estilizado
void print_interpreter();

// mostrar prompt
void displayPrompt();

// executar comando
void executeCommand(char *command);

// separar argumentos
void splitArgs(char *command, char **args, int *nArgs, int *waitProcess);

// manipulars comandos internos da shell
void builtInShell(char **args, int nArgs);

// executa comandos externos da shell
void executeExternalCommand(char **args, int nArgs, int waitProcess);

// terminal principal
void terminal(char *command, size_t buffersize);

#endif
