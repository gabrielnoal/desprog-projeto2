#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#include "sort.h"


void copy(int u[], int v[], int n)
{
  for (int i = 0; i < n; i++)
  {
    v[i] = u[i];
  }
}

int is_sorted(int v[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    if (v[i] > v[i + 1])
    {
      return 0;
    }
  }
  return 1;
}

double time()
{
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  return usage.ru_utime.tv_sec + (usage.ru_utime.tv_usec / 1000000.0);
}

int main(void)
{
  int n;
  printf("tamanho da entrada: ");
  if (scanf("%d", &n) != 1 || n < 0)
  {
    fprintf(stderr, "erro ao ler tamanho da entrada\n");
    return EXIT_FAILURE;
  }

  // int u[] = {9, 6, 8, 7, 5};
  int v[n], u[n];

  for (int i = 0; i < n; i++) {
      int random_number = rand() % 1000 + 0;
      u[i] = random_number;
  }

  double s, f;

  copy(u, v, n);
  s = time();
  bucket_sort(v, n);
  f = time();
  if (!is_sorted(v, n))
  {
    printf("bucket sort failed\n");
    return EXIT_FAILURE;
  }
  printf("Tempo: %lf s\n", f - s);

  return EXIT_SUCCESS;
}
