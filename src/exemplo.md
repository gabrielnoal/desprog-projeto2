Bucket Sort
=================

Para entendermos a ideia do bucket sort primeiro faremos uma viagem para o passado até o [handout da Aula 9](https://ensino.hashi.pro.br/desprog/aula9/handout.html) onde analisamos o algorítimo *quick sort*, imagino que você lembre a ideia básica dele.

Para aqueles com a memória mais limitada, o *quick sort* se baseia em separar o vetor inicial em duas partes, após a escolha de um pivô, sendo uma parte maior a outra menor, com isso o algorítimo ordena as duas partes separadamente e junta, elas tendo em vista que os valores de uma já são maiores que a outra.

> Aqui esta o código do *quick sort* caso queira relembrar.

    void quick_sort_r(int v[], int l, int r) {
    if (l >= r) {
    return;
    }
    int p = partition(v, l, r); //Separa
    quick_sort_r(v, l, p - 1); //Ordena
    quick_sort_r(v, p + 1, r); //Ordena
    }

    void quick_sort(int v[], int n) {
    quick_sort_r(v, 0, n - 1);
    }

O problema do *quick sort* é acharmos o pivô que representa a mediana do vetor, pois não sabemos o intervalo dos nossos valores.

Mas e se soubéssemos qual esse intervalo? Por exemplo, nossos valores estão sempre entre 0 e 1000.

## Questão 1

O que parece ser um bom chute para uma regra que divida esses valores em maior e menor?

###

Nesse caso fica fácil, pois podemos dividir nosso vetor no intervalo de até 500 e maiores que 500.

Porém raramente os dois intervalos teriam aproximadamente o mesmo número de elementos, na maioria dos casos um intervalo teria mais elementos que o outro, podendo até mesmo ter todos os elementos do vetor original. O que só nós prejudicaria pois acabariamos com o mesmo problema que tentamos resolver.

Mas e se ao invés de dividir em 2 partes quiséssemos dividir em 3 ou mais?

Consegue ver onde quero chegar?

Essa é basicamente a ideia do *bucket sort*. Uma vez que sabemos que nossos valores estão distribuídos em um intervalo, podemos usar o conceito do *quick sort* de dividi-lo em pedaços menores e ordenar esses pedaços separadamente, tendos os pedaços menores ja organizados, podemos junta-los e obter nosso produto final.

Chamaremos esses pedaços menores de buckets, pois são como baldes onde inserimos apenas os valores que estão dentro de cada respectivo intervalo.


## Questão 2

Para que o *bucket sort* seja eficiante, temos que garantir que nossos elementos estejão divididos uniformemente em varios buckets.

Tente pensar em quantos buckets você consegue sub-dividir um vetor de tamanho `6` com valores de 0 até 999 e qual seria a regra (intervalo) para cada buckets.

> Dica use um vetor real, por exemplo: v = {50, 455, 578, 735, 109, 436}

###

Como você pode notar existem infinitas regras para sub-dividir esse vetor. Portanto precisamos definir uma maneira "padrão". Para isso primeiramente tomar em conta o intervalo dos valores do vetor original.

Por exemplo vamos levar em conta o vetor da Questão 2

    v = {50, 455, 578, 735, 109, 436}

Considerando que temos um intervalo de 0 a 999 uma das soluções seria dividir cada bucket em intervalos de centenas e para que tenhamos todos os intervalos das centenas ([0, 99], [100, 199], [200, 299], ..., [900, 999]) precisaríamos de `10` buckets.


## Questão 3

Agora que temos 10 buckets, podemos fazer um vetor de buckets dessa forma cada valor do nosso vetor de buckets sera uma lista apenas com valores com o mesmo valor na casa da centena.

Para facilitar o código criaremos um struct chamado int_bucket que contem o numero de elementos (size) dentro dele e um ponteiro para os valores (*data):

    typedef struct
    {
      int size;
      int *data;
    } int_bucket

Tente criar um pseudo-código que cria um vetor de buckets e separa os valores do vetor original nesses buckets.

###

    cria um vetor de int_bucket do tamanho do número de buckets 
    Para um i de 0 até numero de buckets - 1, iterando de 1 em 1:
      Define o tamanho do buckets[i] = 0;
      Aloca memoria para o buckets[i];
      
    Para um j de 0 até n - 1 iterando de 1 em 1: //Sendo n o tamanho do nosso vetor original
      Pega a o numero mais a esquerda do nosso valor atual;
      Insere nosso valor no bucket correto;
      Aumenta o tamanho do bucket;

Podemos traduzir isso para *C* da seguinte maneira:

    #define bucket_numbers 10

    typedef struct
    {
      int size;
      int *data;
    } int_bucket;

    void create_buckets(int_bucket *buckets, int n)
    {
      for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
      {
        buckets[bucket_index].size = 0;
        buckets[bucket_index].data = (int *)malloc(sizeof(int) * n);
      }
    }

    void buckets_partition(int v[], int n, int_bucket buckets[])
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
      create_buckets(buckets, n);

      //Separa em buckets
      buckets_partition(v, n, buckets);
    }

Se rodarmos esse código com nosso vetor `V` teremos buckets exatamente assim:

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

Agora que temos os buckets quase prontos, podemos notar que o Bucket[400, 499] não esta ordenado! Ou seja não podemos apenas juntar todos eles.

## Questão 4

Será necessário a utilização de um método de ordenação para cada bucket podendo ser o mesmo para todos ou um para cada. Isso significa que dependendo da característica do nosso bucket a ser ordenado podemos optar por uma ou outra ordenação.

Nesse handout vamos utilizar um mesmo algorítimo de ordenação, que já aprendemos, para todos os buckets.

Olhe a [tabela de ordenação](https://learn-us-east-1-prod-fleet01-xythos.s3.us-east-1.amazonaws.com/5e08d75562378/1587189?response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%27Tabela1%25281%2529.pdf&response-content-type=application%2Fpdf&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20200507T172221Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=AKIAZH6WM4PLTYPZRQMY%2F20200507%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=f3936156610ea760e6e55249f672a65ad309ec99b97b7668a3861498995b2049) e determine qual algorítimo é melhor para esta tarefa.

> Dica: Se ultilizado eficientemente, o bucket sort gera apenas buckets pequenos.

###

Esperamos que você tenha escolhido utilizar o *Insertion Sort* para essa tarefa, tendo em vista que muitas vezes como eles são pequenos, os buckets já estarão ordenados ou até mesmo terão apenas um valor. sua complexidade para vetores pequenos já ordenados seria O(n).

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

Com o algorítimo definido faremos nossa função de ordenação que sera chamada a cada bucket.

    void sort(int *v, int_bucket buckets[]){
      int v_index = 0; 
      for (int bucket_index = 0; bucket_index < bucket_numbers; bucket_index++)
      {
        insertion_sort(buckets[bucket_index].data, buckets[bucket_index].size); 
      }

    }


Agora que temos todos os buckets ordenados podemos finalmente juntar-los em um vetor novamente. Essa implementação é muito simples por isso não vou pedir para você fazer nada, basta usar essa função combine.

    void combine(int *v, int_bucket buckets[], int v_index, int bucket_index)
    {
      for (int data_index = 0; data_index < buckets[bucket_index].size; data_index++)
        v[v_index + data_index] = buckets[bucket_index].data[data_index]; 
    }


Muita calma nessa hora, sem fechar a aba ainda. Como de costume ainda falta uma parte essencial do handout, a tão adorada análise de complexidade!

###

Para entender o funcionamento da complexidade deste método, iremos comecar com o entendimento de duas partes cruciais para o metodo em questão, sendo elas: 

  1 - Analise do tempo de execução para distribuição do vetor de entrada em buckets.

  2 - Analise do tempo de execução para ordenação de cada bucket.

Em nossa analise, consideraremos 2 parâmetros n ( numero de elementos ) e k ( numero de buckets ).

## Questão 6.a
 

Qual é a complexidade do bucket sort quando se diz a respeito de organizar a entrada em diferentes buckets?

###  

Não nos leva muito tempo até entender que será uma complexidade de O(n), tendo em vista que todos os elementos do vetor devem ser percorridos.
Entretanto, ao tentar compreender a parte 2 da complexidade deste modelo algumas incertezas são criadas.
Por exemplo, um primeiro palpite comum a ser dado para a complexidade do modelo pode ser: $$\ 0(\frac{n}{k})$$ 
para cada bucket, ou seja $$\ O(k*(\frac{n}{k}))$$

A razão para que o que foi dito acima não esta 100% correto é porque falta um fator constante no calculo da complexidade. Quando cada bucket é visitado e cada um dos elementos é analisado, é nítido que não se leva um tempo de execução de $$\frac{n}{k}$$, muito menos algum múltiplo constante de $$\frac{n}{k}$$.

## Questão 6.a

Tente adivinhar o que aconteceria caso o balde estivesse vazio.

###

No caso de um balde vazio, o código ainda gasta tempo de execução com o mesmo já que deve analisar se possui algum valor dentro de cada um dos buckets presentes sendo necessário adicionar a fórmula um elemento constante c0 e c1 que nada mais são que constantes especificas de implementação, passando a fórmula para $$\ O(C_0\frac{n}{k}+C_1)$$.
Ao ver a formula a primeira coisa que provavelmente ira pensar é que nada ira mudar já que $$\ O(C_0\frac{n}{k}+C_1) = O(\frac{n}{k})$$ , entretanto, tudo ira mudar ao considerarmos o fator k de multiplicação a complexidade, observe:


$$\ O(n)=O(k*(C_0*\frac{n}{k}+C_1)) = O((C_0)*(n) + (k)*(C_1))$$

Percebemos então que a formula é totalmente dependente de k, fazendo com que o tempo de execução seja $$\ O(n + k)$$

Agora que entendemos como funciona a análise da complexidade do bucket sort em sua forma esperada (linear), vamos entender o que acontece nos outros casos para o bucket sort, quando ele não é linear.

## Questão 6.b

Tente adivinhar para quais ou qual caso o bucket sort não ira operar em tempo linear O(n + k).

###

Por ser um algoritmo de ordenação que depende de outro algoritmo de ordenação, o bucket sort esta suscetível a considerações extras do outro algoritmo em questão. Como o que utilizamos foi o Insertion Sort, iremos discorrer a respeito do mesmo. O insertion sort é um algoritmo que pode ser levado como quadrático ou linear (no seu caso ideal), dado que para listas ordenadas, o mesmo opera em tempo de O(n), enquanto para listas que não estão ordenadas, opera em O(n²).

  
## Questão 6.c

Levando isso em consideração, o que causaria um bucket a ter uma maior complexidade?

##

Voltando assim as duas partes da analise de complexidade. Temos a parte de criação de cada bucket, e como já vimos, a mesma é $$\ O(n + k)$$ e em seguida temos a parte de ordenação de cada bucket, que é delimitada pelo algorítimo do Insertion sort. Este que pode ser linear(melhor caso) ou quadrático(médio e pior caso).

Portanto, a complexidade do método bucket sort será basicamente determinada por como os buckets já estarão previamente ordenados. Por isso uma utilização otimizada do bucket sort seria em vetores que geram buckets pequenos.

## Por hoje é só

![balde](balde.jpg)

## Extra:

Disponibilizamos o [código fonte](https://github.com/gabrielnoal/desprog-projeto2/tree/master/codigo) para que você possa brincar.


## Cada bucket ser ordenado 
O((n/k)^2)
Tendo K buckets 
O((n)^2/k)