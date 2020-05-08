#include <stdio.h>
#include <stdlib.h>
#include "sort.h"


void logBuckets(int_bucket buckets[bucket_numbers])
{
  for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
  {
    printf("Bucket [%d,%d]: {",
           bucket_index == 0 ? 0 : bucket_index * 100,
           (bucket_index + 1) * 100 - 1);

    for (int data_index = 0; data_index < buckets[bucket_index].size; data_index++)
    {
      printf("%d", buckets[bucket_index].data[data_index]);
      if (data_index != buckets[bucket_index].size - 1)
        printf(", ");
    }
    printf("}\n");
  }
  printf("\n");
}

void logV(const char *name, int v[], int n)
{
  printf("%s", name);

  printf(": {");
  for (int i = 0; i < n; i++)
  {
    printf("%d", v[i]);
    if (i != n - 1)
      printf(", ");
  }
  printf("}\n");
}

void bubble_sort(int v[], int n)
{
  printf("bubble_sort: ");

  for (int i = n - 1; i > 0; i--)
  {
    int swapped = 0;

    for (int j = 0; j < i; j++)
    {
      if (v[j] > v[j + 1])
      {
        int temp = v[j];
        v[j] = v[j + 1];
        v[j + 1] = temp;
        swapped = 1;
      }
    }
    if (!swapped)
    {
      break;
    }
  }
  logV("selection_sort: ", v, n);
}

void selection_sort(int v[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    int m = i;

    for (int j = i + 1; j < n; j++)
    {
      if (v[m] > v[j])
      {
        m = j;
      }
    }

    if (m != i)
    {
      int temp = v[m];
      v[m] = v[i];
      v[i] = temp;
    }
  }
  logV("selection_sort: ", v, n);
}

void insertion_sort(int v[], int n)
{
  for (int i = 1; i < n; i++)
  {
    int temp = v[i];

    int h = i;
    while (h > 0 && temp < v[h - 1])
    {
      v[h] = v[h - 1];
      h--;
    }

    v[h] = temp;
  }
  //logV("insertion_sort: ", v, n);
}

void create_bucket(int_bucket *buckets, int n)
{
  for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
  {
    buckets[bucket_index].size = 0;
    buckets[bucket_index].data = (int *)malloc(sizeof(int) * n);
  }
}

void merge_buckets(int v[], int n, int_bucket buckets[])
{
  for (int v_index = 0; v_index < n; v_index++)
  {
    int bucket_index = v[v_index] / 100;
    buckets[bucket_index].data[buckets[bucket_index].size] = v[v_index];
    buckets[bucket_index].size++;
  }
}

void combine(int *v, int_bucket buckets[], int v_index, int bucket_index)
{
  for (int data_index = 0; data_index < buckets[bucket_index].size; data_index++)
    v[v_index + data_index] = buckets[bucket_index].data[data_index];
}

void sort(int *v, int_bucket buckets[]){
  int v_index = 0;
  for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
  {
    if (buckets[bucket_index].size)
    {
      insertion_sort(buckets[bucket_index].data, buckets[bucket_index].size);

      combine(v, buckets, v_index, bucket_index);

      v_index += buckets[bucket_index].size;
    }

    free(buckets[bucket_index].data);
  }
}

void bucket_sort(int v[], int n)
{
  int_bucket buckets[bucket_numbers];

  //Cria Bucket
  create_bucket(buckets, n);

  //Separa em buckets
  merge_buckets(v, n, buckets);

  //Loga os buckets
  logBuckets(buckets);

  //Ordena
  sort(v, buckets);

  logV("bucket_sort", v, n);
}