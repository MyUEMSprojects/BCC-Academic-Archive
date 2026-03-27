#ifndef LIBS_H
#define LIBS_H 
/* header dedicado a importação de todas as libs necessarias para o programa,
 * tal como constantes e cores do sistema
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <ctype.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

#define MAX_ARGS 20
#define MAX_PATH_SIZE 1045
#define MAX_ARG_SIZE 200

#endif 
