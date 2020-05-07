Bucket Sort
=================

Como vimos na Aula 8, o algoritmo *merge sort* usa uma estratégia de divisão e conquista: divide a entrada original em duas partes, ordena recursivamente cada parte e combina essas partes ordenadas.

    void merge_sort_r(int v[], int temp[], int l, int r) {
        if (l == r) {
            return;
        }
        int m = (l + r) / 2;
        merge_sort_r(v, temp, l, m);
        merge_sort_r(v, temp, m + 1, r);
        combine(v, temp, l, m, r);
    }

    

Com a divisão e conquista o *merge sort* facilita a ordenação de cada vetor, pois são vetores menores.

Ele não é o primeiro nem o ultimo algoritimo de divisão e conquista. Nesse handout pretendemos introduzir outro algoritmo conhecido como *Bucket Sort* ou *Bin Sort*

## Questão 1

Tente pensar em quantos mini-vetores você consegue sub-dividir um vetor de tamanho `5` com valores de 0 até 999 e qual seria a regra (intervalo) para cada mini-vetores.

> Dica use um vetor real, por exemplo: v = {50, 455, 578, 735, 109, 436}

###

Como você pode notar existem infinitas regras para sub-dividir esse vetor. Portanto precisamos definir uma maneira "padrão". Para isso primeiramente tomar em conta o intervalo dos valores do vetor original.

Por exemplo vamos levar em conta o vetor da Questão 1:

    v = {50, 455, 578, 735, 109, 436}

Considerando que temos um intervalo de 0 a 999 uma das soluções seria dividir em intervalos de centenas e para que tenhamos todos os intervalos das centenas ([0, 99], [100, 199], [200, 299], ..., [900, 999]) precisariamos de `10` mini-vetores que chamaremos de buckets.

## Questão 2

Agora que temos 10 buckets, podemos fazer um vetor de buckets dessa forma cada valor do nosso vetor de buckets sera uma lista apenas com valores com o mesmo valor na casa da centena.

Para facilitar o código criaremos um struct chamado int_bucket que contem o numero de elementos (size) dentro dele e um ponteiro para os valores (*data):

    typedef struct
    {
      int size;
      int *data;
    } int_bucket

Tente criar um pseudo-codigo que cria um vetor de buckets e separa os valores do vetor original nesses buckets.

###

    cria um vetor de int_bucket do tamanho do numero de buckets 
    Para um i de 0 até até numero de buckets - 1, iterando de 1 em 1:
      Define o tamano do buckets[i] = 0;
      Aloca memoria para o buckets[i];
      
    Para um j de 0 até n - 1 iterando de 1 em 1: //Sendo n o tamanho do nosso vetor original
      Pega a o numero mais a esqueda do nosso valor atual;
      Insere nosso valor no bucket correto;
      Aumenta o tamanho do bucket;

Podemos traduzir isso para *C* da seguinte maneira:

    #define bucket_numbers 10

    typedef struct
    {
      int size;
      int *data;
    } int_bucket;

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

    void bucket_sort(int v[], int n)
    {
      int_bucket buckets[bucket_numbers];

      //Cria Bucket
      create_bucket(buckets, n);

      //Separa em buckets
      merge_buckets(v, n, buckets);
    }

Se tivessemos rodado esse codigo com nosso vetor `V` teriamos buckets exatamente assim:

    Bucket [0,99]: {50}
    Bucket [100,199]: {109}
    Bucket [200,299]: {}
    Bucket [300,399]: {}
    Bucket [400,499]: {455, 436}
    Bucket [500,599]: {578}
    Bucket [600,699]: {}
    Bucket [700,799]: {735}
    Bucket [800,899]: {}
    Bucket [900,999]: {}

Agora que temos os buckets quse prontos, podemos notar que o Bucket[400, 499] não esta ordenado! Ou seja não podemos apenas juntar todos eles.

## Questão 3

Será necessario a ultilização de um método de ordenação para cada bucket podendo ser o mesmo para todos ou um para cada. Isso significa que dependendo da caracteristica do nosso bucket a ser ordernado podemos opitar por uma ou outra ordenação.

Nesse handout vamos ultilizar um mesmo algoritimo de ordenação, que já aprendemos, para todos os buckets.

Olhe a [tabela de ordeção](https://learn-us-east-1-prod-fleet01-xythos.s3.us-east-1.amazonaws.com/5e08d75562378/1587189?response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%27Tabela1%25281%2529.pdf&response-content-type=application%2Fpdf&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20200507T172221Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=AKIAZH6WM4PLTYPZRQMY%2F20200507%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=f3936156610ea760e6e55249f672a65ad309ec99b97b7668a3861498995b2049) e determine qual algoritimo é melhor para esta tarefa.

> Dica: Se ultilizado eficientemente, o bucket sort gera buckets pequenos.

NÃO CONTINUE SEM VALIDAR NO CANAL GERAL

###

Esperamos que voçe tenha esolhido utilizar o *Insertion Sort* para essa tarefa, tendo em vista que muitas vezes os buckets ja estarão ordenados ou até mesmo terão apenas um valor. sua complexidade para vetores pequenos ja ordernados seria O(n).

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
    }

Com o algoritimo definido faremos nossa função de ordenação que sera chamada a cada bucket.

    void sort(int *v, int_bucket buckets[]){
      int v_index = 0;
      for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
      {
        insertion_sort(buckets[bucket_index].data, buckets[bucket_index].size);
      }
    }

    

Apesar desta implementação não estar errada podemos melhorar ela com muita facilidade.

## Questão 4

Qual o problema com essa implementação?

NÃO CONTINUE SEM VIR VALIDAR NO CANAL GERAL

###

Estamos ultilizando o *Insetion Sort* em todos os buckets apesar de provavelmente existirem buckets vazios. Isso pode ser facilmente contornado com um simples *if*.

    void sort(int *v, int_bucket buckets[]){
      int v_index = 0;
      for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
      {
        insertion_sort(buckets[bucket_index].data, buckets[bucket_index].size);
      }
    }

Agora que temos todos os buckets ordenados podemos finalmente juntar-los em um vetor novamente. Essa implementação é muito simples por isso não vou pedir para você fazer nada.

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
      }
    }

###

## OPA PARABÉNS O IGOR ACABOU DE REPROVAR VOCÊ!

Se ulitilizamos o malloc para fazer os buckets não podemos esquecer de ultilizar o free! 

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

Codigo final 

    #define bucket_numbers 10

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

      //Ordena
      sort(v, buckets);
    }

###

## Complexidade



## Disafio:

Para que o Bucket Sort seja eficiente temos que nos certificar que os intervalos e o numero de buckets que definimos estão corretos.

Tente determinar intervalos para os seguintes vetores:

* v variando de 0 até 1

* v variando de 0 até 10

* v variando de 0 até 100

Qual parte do codigo mudariamos??

