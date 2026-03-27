# Instruções:
   gcc -o main main.c  -Wall -Wextra -pedantic
   ./main <numero_método> <nome_arquivo_entrada> <nome_arquivo_saida>

## O programa usou como base os algoritmos passados na aula e os algoritmos dos livros.
## Livros:
# Algoritmos - algoritmos-teoria-e-prc3a1tica-3ed-thomas-cormen
# Szwarcfiter, Jayme Luiz - Estruturas De Dados E Seus Algoritmos (2010)

#Trablaho:

Faça um único programa em C para Linux que implemente os seguintes algoritmos de ordenação estudados:

    1 - Bubble-sort original;
    2 - Bubble-sort melhorado;
    3 - Insertion-sort;
    4 - Mergesort;
    5 - Quicksort com pivô sendo o último elemento;
    6 - Quicksort com pivô sendo um elemento aleatório;
    7 - Quicksort com pivô sendo a mediana de três;
    8 - Heapsort.

A entrada do programa consiste de um arquivo binário contendo apenas uma sequência de números inteiros do tipo int32_t. A sequência ordenada deve ser armazenada em outro arquivo binário. Os arquivos de entrada/saída não terão nenhuma outra informação além da sequência de entrada/saída, sendo que a quantidade de elementos nesses arquivos deve ser calculada considerando o tamanho do arquivo em bytes (veja página de manual da função fstat() ou stat()) e a quantidade de bytes que cada elemento ocupa.  O programa deve receber três argumentos da linha de comando, nessa ordem: 1) o número do algoritmo escolhido (1 a 8); 2) o nome do arquivo de entrada; 3) o nome do arquivo de saída. Não deve haver interação com o usuário e, ao final, deve ser apresentado o tempo de execução gasto apenas pelo algoritmo de ordenação (desconsiderando tempo de E/S como, por exemplo, leitura/gravação em arquivos, impressão em tela, leitura de teclado, etc.) em segundos com precisão de 3 casas decimais (milissegundos). Para cálculo do tempo, consulte funções como gettimeofday(), clock_gettime(), dentre outras.

Teste cada uma das versões dos algoritmos implementados com três tipos de entrada: em ordem crescente, em ordem decrescente e em ordem aleatória. Devem ser consideradas entradas dos seguintes tamanhos nos testes:

    - 10, 100, 1.000, 10.000 e 100.000 (todas as versões de todos os algoritmos);
    - 500.000, 5.000.000 e 50.000.000: Mergesort, Quicksort com pivô sendo um elemento aleatório, Quicksort com pivô sendo a mediana de três, Heapsort.

Faça um relatório de aproximadamente 3 páginas contendo tabelas, gráficos e discussões sobre os resultados obtidos nos testes. O relatório deve conter os dados do aluno e da disciplina, uma breve introdução, uma seção com os resultados obtidos e uma seção com a discussão sobre os resultados e conclusão do relatório. Os tempos de execução para elaboração do relatório devem ser coletados no Laboratório de Computação 4.

O programa gera-in-out.c pode ser utilizado para testar os algoritmos implementados. São gerados arquivos de entrada com uma quantidade de elementos informada pelo usuário, que também deve especificar se a entrada deve estar em ordem crescente (será gerado um arquivo com a terminação .c), decrescente (.d) ou aleatória (.a).  Para cada arquivo de entrada solicitado também é gerado um arquivo de saída (.out), que contém os mesmos elementos do arquivo de entrada, porém sempre em ordem crescente.

O código deve estar devidamente organizado, identado, comentado e sem warnings usando as opções -Wall -Wextra -pedantic do gcc.

 Cópias e similares: nota zero para todos os envolvidos.
