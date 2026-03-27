#include <stdio.h>
#include <string.h>

int palindromo(char *s, int inicio, int fim) {
    if (inicio >= fim) {
        return 1;
    }
    
    if (s[inicio] == s[fim]) {
        return palindromo(s, inicio + 1, fim - 1);
    }

    return 0;
}

int main() {
    char string[] = "";
    int tamanho = strlen(string);
    
    printf("%d", palindromo(string, 0, tamanho - 1));
    
    return 0;
}
