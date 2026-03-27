#include <terminal.h>

void print_welcome()
{
    printf("\033[1;35m");
    printf("  ★・。・゜・。・。・゜・☆・゜・。・゜・。・★\n");
    printf("  ☆                                          ☆\n");
    printf("  ★            \033[1;33mB E M - V I N D O\033[1;35m             ★\n");
    printf("  ☆                                          ☆\n");
    printf("  ★・。・゜・。・。・゜・☆・゜・。・゜・。・★\033[0m\n");
    printf("\n");
    usleep(300000);
}

void print_interpreter()
{
    printf("\033[1;33m");
    printf("✦•━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━•✦\n");
    printf("   \033[1;34m▐▓▒░  I N T E R P R E T A D O R  ░▒▓▌\033[1;33m   \n");
    printf("✦•━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━•✦\033[0m\n");
    printf("\n");
    usleep(300000); // Pausa de 0.3 segundos
}

void displayPrompt()
{
   printf("%s❂%s %s~%s %s>%s ", CYAN, RESET, GREEN, RESET, BLUE, RESET);
   fflush(stdout);
}

void executeCommand(char *command)
{
    char *args[MAX_ARG_SIZE] = {NULL};
    int nArgs = 0;
    int waitProcess = 0;

    splitArgs(command, args, &nArgs, &waitProcess);

    if (nArgs == 0)
        return;

    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }

    builtInShell(args, nArgs);

    if (strcmp(args[0], "cd") != 0 && strcmp(args[0], "tree") != 0)
    {
        executeExternalCommand(args, nArgs, waitProcess);
    }

    // Libera memória alocada para os argumentos
    for (int i = 0; i < nArgs; i++)
    {
        free(args[i]);
    }
}

void splitArgs(char *command, char **args, int *nArgs, int *waitProcess)
{
    // Verifica se deve esperar pelo processo
    if (command[strlen(command) - 1] == '&')
    {
        *waitProcess = 1;
        command[strlen(command) - 1] = '\0';
    }

    // Divide o command em tokens
    char *token = strtok(command, " ");
    while (token)
    {
        args[*nArgs] = malloc(MAX_ARG_SIZE);
        strcpy(args[*nArgs], token);
        (*nArgs)++;
        token = strtok(NULL, " ");
    }
}

void builtInShell(char **args, int nArgs)
{
    if (strcmp(args[0], "cd") == 0)
    {
        if (nArgs > 1)
        {
            chdir(args[1]);
        }
        else
        {
            chdir(getenv("HOME"));
        }
    }
    else if (strcmp(args[0], "tree") == 0)
    {
        if (nArgs > 1)
        {
            struct Node *root = NULL;
            printf("pid: %s\n", args[1]);
            buildTree(atoi(args[1]), &root);
            if (!root)
                printf("Processo nao encontrado!\n");
            else
                printTree(root, 0);

            freeTree(root);
        }
        else
        {
            printf("Uso: tree <PID>\n");
        }
    }
}

void executeExternalCommand(char **args, int nArgs, int waitProcess)
{
    char cmd[MAX_ARG_SIZE];
    pid_t pid;
    int status;

    // Tenta encontrar o comando em /bin/
    strcpy(cmd, "/bin/");
    strcat(cmd, args[0]);

    // cria outro processo para lidar com os comandos shell linux
    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed.\n");
    }
    else if (pid == 0) // filho
    {
        // Tenta executar o comando em /bin/
        if (execve(cmd, args, __environ) == -1)
        {
            // Se não encontrar, tenta em /snap/bin/
            strcpy(cmd, "/snap/bin/");
            strcat(cmd, args[0]);

            if (execve(cmd, args, __environ) == -1)
            {
                printf("comando não encontrado!\n");
                exit(1);
            }
        }
    }
    else
    { // Processo pai
        if (!waitProcess)
        {
            waitpid(pid, &status, 0);
        }
    }
}

void terminal(char *command, size_t buffersize)
{
    displayPrompt();

    // Lê o comando do usuário
    if (!fgets(command, buffersize, stdin))
    {
        // Sai do programa em caso de EOF(end  of file)
        exit(0);
    }

    // Remove o \n do final
    command[strlen(command) - 1] = '\0';

    // Ignora linhas vazias
    if (strcmp(command, "\0") == 0)
    {
        return;
    }

    // Executa o comando
    executeCommand(command);
}
