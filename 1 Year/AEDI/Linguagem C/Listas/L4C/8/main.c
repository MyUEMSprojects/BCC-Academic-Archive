#include <stdio.h>
#include <stdlib.h>

int main()
{
   int vetorA[5] = {1, 2, 3, 4, 5}, vetorB[8] = {1, 2, 3, 4, 5, 6, 7, 8};
   int tamA = sizeof(vetorA) / sizeof(int), tamB = sizeof(vetorB) / sizeof(int), tamTOT = tamA + tamB;
   int vetorC[tamTOT], i;

   for (i = 0; i < tamTOT; i++)
   {
      vetorC[i] = vetorA[i];
      if (i >= tamA)
         vetorC[i] = vetorB[i];
   }

   printf("\nVetor A: ");
   for (i = 0; i < tamA; i++)
      printf("%d", vetorA[i]);

   printf("\nVetor B: ");
   for (i = 0; i < tamB; i++)
      printf("%d", vetorB[i]);

   printf("\nVetor C: ");
   for (i = 0; i < tamTOT; i++)
      printf("%d", vetorC[i]);

   return 0;
}