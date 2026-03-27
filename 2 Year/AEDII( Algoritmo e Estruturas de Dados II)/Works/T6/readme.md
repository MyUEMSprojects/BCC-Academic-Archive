# Instruções:
   gcc -o main main.c  -Wall -Wextra -pedantic
   ./main <numero_método> <nome_arquivo_entrada> <nome_arquivo_saida>

## O programa usou como base os algoritmos passados na aula e os algoritmos dos livros.
## Livros:
# Algoritmos - algoritmos-teoria-e-prc3a1tica-3ed-thomas-cormen
# Szwarcfiter, Jayme Luiz - Estruturas De Dados E Seus Algoritmos (2010)

#Trablaho:

Faça um programa em C que compacte e descompacte arquivos (texto ou binários) usando o algoritmo de Huffman. O seu programa deve receber 2 argumentos de linha de comando:

1) O primeiro argumento é a opção, que deve ser "c" ou "d", para compactar ou descompactar um arquivo, respectivamente;

2) O segundo argumento é o nome de um arquivo a ser compactado ou descompactado. No caso da compactação, será gerado um arquivo binário de saída com o mesmo nome do arquivo de entrada seguido do sufixo ".cmp". Esse arquivo conterá tanto a árvore de Huffman quanto os dados compactados.  No caso da descompactação, será gerado um arquivo binário de saída com o mesmo nome do arquivo de entrada seguido do sufixo ".dcmp". Esse arquivo conterá os dados descompactados do arquivo de entrada.

O código deve estar devidamente organizado, identado, comentado e sem warnings usando as opções -Wall -Wextra -pedantic do gcc.