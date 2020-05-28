#ifndef SORT_H
#define SORT_H

#define bucket_numbers 10

typedef struct
{
  int size;
  int *data;
} int_bucket;


void logBuckets(int_bucket buckets[bucket_numbers]);
void logV(const char *name, int v[], int n);
void bubble_sort(int v[], int n);
void selection_sort(int v[], int n);
void insertion_sort(int v[], int n);
void create_buckets(int_bucket *buckets, int n);
void buckets_partition(int v[], int n, int_bucket buckets[]);
void combine(int *v, int_bucket buckets[], int v_index, int bucket_index);
void bucket_sort(int v[], int n);
void sort(int *v, int_bucket buckets[]);

#endif
