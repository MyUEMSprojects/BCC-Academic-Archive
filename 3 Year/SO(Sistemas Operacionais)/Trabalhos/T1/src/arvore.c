#include <arvore.h>

struct Node *createNode(int pid, char *name)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    strcpy(new_node->name, name);
    new_node->pid = pid;
    new_node->num_child = 0;
    new_node->children = NULL;
    return new_node;
}

struct Node *findNode(struct Node *node, int pid)
{

    if (!node) return NULL;
    if (node->pid == pid) return node;

    for (int i = 0; i < node->num_child; i++)
    {
        struct Node *found = findNode(node->children[i], pid);
        if (found != NULL)
            return found;
    }

    return NULL;
}

void freeTree(struct Node *node)
{
    if (!node) return;

    if (node->children != NULL)
    {
        for (int i = 0; i < node->num_child; i++)
        {
            if (node->children[i] != NULL)
            {
                freeTree(node->children[i]);
            }
        }

        free(node->children);
        node->children = NULL;
    }

    free(node);
}

void printTree(struct Node *node, int level)
{
    if (!node) return;
    for (int i = 0; i < level - 1; i++) printf("              ");
    printf("└──%s(%d)\n", node->name, node->pid);
    for (int i = 0; i < node->num_child; i++) printTree(node->children[i], level + 1);
}

int addChild(struct Node *parent, struct Node *child)
{
    if (parent == NULL || child == NULL)
    {
        fprintf(stderr, "Erro: No nulo passado para a funcao addChild()\n");
        return -1;
    }

    // calcula o novo tamanho para realocar
    size_t new_size = parent->num_child + 1;
    struct Node **new_children = realloc(parent->children, sizeof(struct Node *) * new_size);

    if (new_children == NULL)
    {
        fprintf(stderr, "Erro: alocacao de memoria falhou em addChild()\n");
        return -1;
    }

    // atualiza os filhos do pai e conta
    parent->children = new_children;
    parent->children[parent->num_child] = child;
    parent->num_child = new_size;

    return 0;
}

void buildTree(int pid, struct Node **root)
{
    DIR *dir;
    struct dirent *entry;
    char path[MAX_PATH_SIZE];
    char proc_name[256];
    int proc_pid, proc_ppid;
    char proc_status;
    FILE *file;

	// valida a entrada
    if (root == NULL)
    {
        fprintf(stderr, "Error: Invalid root pointer\n");
        return;
    }

	// Abre /proc directory
    if ((dir = opendir("/proc")) == NULL)
    {
        perror("Error opening /proc directory");
        return;
    }

	// passo1: encontra a raiz do processo e cria os nós
    while ((entry = readdir(dir)) != NULL)
    {
		// pular diretorios que nao sao de processos
        if (!isNumeric(entry->d_name))
        {
            continue;
        }

		// cria o caminho do arquivo stat
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

		// abre e le as informacoes do professo 
        if ((file = fopen(path, "r")) == NULL)
        {
            continue; // processo "talvez" tenha terminado
        }

		// le a informacao do processo
        if (fscanf(file, "%d %255s %c %d", &proc_pid, proc_name, &proc_status, &proc_ppid) != 4)
        {
            fclose(file);
            continue;
        }
        fclose(file);

		// cria o no raiz se for encontrado
        if (proc_pid == pid && *root == NULL)
        {
            *root = createNode(proc_pid, proc_name);
            if (*root == NULL)
            {
                closedir(dir);
                return;
            }
        }
    }

	// passo2: cria a arvore hieraquica
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL)
    {
		// pula diretorios que nao sao de processos
        if (!isNumeric(entry->d_name))
        {
            continue;
        }

		// cria o caminho do arquivo stat
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

		// abre e le a informacao do processo
        if ((file = fopen(path, "r")) == NULL)
        {
            continue;
        }

		// le a informacao do processo
        if (fscanf(file, "%d %255s %c %d", &proc_pid, proc_name, &proc_status, &proc_ppid) != 4)
        {
            fclose(file);
            continue;
        }
        fclose(file);

		// pula o processo raiz e processos com PIDs pequenos(menores)
        if (proc_pid == pid || proc_pid < pid)
        {
            continue;
        }

		// encontra o pai e adiciona o filho
        struct Node *parent = findNode(*root, proc_ppid);
        if (parent != NULL)
        {
            struct Node *child = createNode(proc_pid, proc_name);
            if (child == NULL || addChild(parent, child) != 0)
            {
				// libera a memoria do filho se tiver erro
                if (child)
                    free(child);
                closedir(dir);
                return;
            }
        }
    }

    closedir(dir);
    return;
}

static int isNumeric(const char *str)
{
    if (str == NULL || *str == '\0')
    {
        return 0; // string vazia não é um numero
    }

    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)str[i]))
        {
            return 0;
        }
    }
    return 1;
}
