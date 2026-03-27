#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//#include <linux/sched.h>
// O bloco de controle de processo é representado no sistema operacional Linux pela estrutura em C task_struct, campos:
//long state; /* estado do processo */
//struct sched_entity se; /* informações de scheduling */
//struct task_struct *parent; /* pai desse processo */
//struct list_head children; /* filhos desse processo */
//struct files_struct *files; /* lista de arquivos abertos */
//struct mm_struct *mm; /* espaço de endereçamento desse processo */


/*
pid_t pid;
int status;
pid = wait (&status);
*/

int main()
{
   pid_t pid;
   /* cria um processo-filho */
   pid = fork();
   if (pid < 0) { /* um erro ocorreu */
      fprintf(stderr, "Fork Failed");
      return 1;
    }
    else if (pid == 0) { /* processo-filho */
      execlp("/bin/ls","ls",NULL);
    }
    else { /* processo-pai */
      /* o pai esperará que o filho seja concluído */
      wait(NULL);
      printf("Child Complete");
    }

    return 0;
}