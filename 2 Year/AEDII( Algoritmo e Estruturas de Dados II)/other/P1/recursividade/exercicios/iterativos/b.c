#include <stdio.h>
#include <string.h>

int palindromo(char *s, int tamanho) {
    
    int i = 0;
    int j = tamanho - 1;

    while(i < tamanho){
        
        if(!(s[i] == s[j])){
            return 0;
        }

        i++;
        j--;
    }

    return 1;
}

int main()
{
    char string[] = "ana"; // "felipe"
    int tamanho = strlen(string);
    printf("%d", palindromo(string, tamanho)); // "1 - verdade"
    
    return 0;
}