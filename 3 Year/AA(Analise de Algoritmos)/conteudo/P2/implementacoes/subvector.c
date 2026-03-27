#include <stdio.h>
#include <stdlib.h>
#define N 16

// Algoritmo de divisão e conquista - Problema do subvetor máximo

void find_max_crossing_subarray(int A[N], unsigned int low, unsigned int mid, unsigned int high);
void find_maximum_subarray(int A[N], unsigned int low, unsigned int high);

int main()
{
  // entrada com N elementos
  int vec[N] = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};

  return 0;
}

void find_max_crossing_subarray(int A[N], unsigned int low, unsigned int mid, unsigned int high)
{
}
void find_maximum_subarray(int A[N], unsigned int low, unsigned int high)
{
  if(low==high) return;
}