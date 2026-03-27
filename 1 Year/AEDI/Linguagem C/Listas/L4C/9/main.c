#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
   
   int n=1;
   int A[n];
   int B[n];
   int C[n];

   scanf("%d",&n);

     for(int i = 1 ; i <= n ; i++){

      scanf("%d", &A[i]);

    if(A[i] % 2 == 0){
      B[i] = A[i];
    }
    if(A[i] % 2 != 0){
      C[i] = A[i];
    }

   }
   
   for(int i = 0; i < n ; i++){

       printf("%d\n",&B[i]);
   }

   for(int i = 0; i < n ; i++){
       printf("%d\n",&C[i]);
   }

    system("pause");
    return 0;
}
