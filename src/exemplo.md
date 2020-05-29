Bucket Sort
=================

Para entendermos a ideia do bucket sort primeiro faremos uma viagem para o passado até o [hangout da Aula 9](https://ensino.hashi.pro.br/desprog/aula9/handout.html) onde analisamos o algoritimo *quick sort*, imagino que você lembre a ideia basíca dele.

Para aqueles com a memória mais limitada, o *quick sort* se baseia em separar o vetor inicial em duas partes, após a escolha de um pivô, sendo uma parte maior a outra menor, com isso o algoritimo ordena as duas partes separadamente e junta elas tendo em vista que os valores de uma já são maiores que a outra.

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

Mas e se soubessemos qual esse intervalo? Por exemplo, nossos valores estão sempre entre 0 e 1000.

## Questão 1

Você consegue pensar em uma regra que divida sempre esses valores em maior e menor?

###

Nesse caso fica facil, pois podemos dividir nosso vetor no intevalo de até 500 e maiores que 500.

Mas e se ao invés de dividir em 2 partes quisessemos dividir em 3 ou mais? Nesse caso achar a mediana não seria o suficiente, teriamos que dividir nosso vetor em varios intervalos pré-definidos, para depois ordena-los. A vantagem disso é que quanto mais intervalos tivermos, menor fica o intervalo a ser ordenado.

Consegue ver onde quero chegar?

Essa é basicamente a ideia do bucket sort. Uma vez que sabemos que nossos valores estão distribuido em um intervalo, podemos usar o conceito do *quick sort* de dividi-lo em pedacinhos e ordernar esses pedaços separadamente. Depois de separados e ordenados, fica facil juntar de novo.


## Questão 2

Tente pensar em quantos intervalos você consegue sub-dividir um vetor de tamanho `6` com valores de 0 até 999 e qual seria a regra (intervalo) para cada intervalos.

> Dica use um vetor real, por exemplo: v = {50, 455, 578, 735, 109, 436}

###

Como você pode notar existem infinitas regras para sub-dividir esse vetor. Portanto precisamos definir uma maneira "padrão". Para isso primeiramente tomar em conta o intervalo dos valores do vetor original.

Por exemplo vamos levar em conta o vetor da Questão 2

    v = {50, 455, 578, 735, 109, 436}

Considerando que temos um intervalo de 0 a 999 uma das soluções seria dividir em intervalos de centenas e para que tenhamos todos os intervalos das centenas ([0, 99], [100, 199], [200, 299], ..., [900, 999]) precisariamos de `10` intervalos.

Chamaremos esses intervalos de buckets, pois são como baldes onde inserimos apenas os valores que estão dentro de cada respectivo intervalo.


## Questão 3

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

Se rodarmos esse codigo com nosso vetor `V` teremos buckets exatamente assim:

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

Será necessario a ultilização de um método de ordenação para cada bucket podendo ser o mesmo para todos ou um para cada. Isso significa que dependendo da caracteristica do nosso bucket a ser ordernado podemos optar por uma ou outra ordenação.

Nesse handout vamos ultilizar um mesmo algoritimo de ordenação, que já aprendemos, para todos os buckets.

Olhe a [tabela de ordeção](https://learn-us-east-1-prod-fleet01-xythos.s3.us-east-1.amazonaws.com/5e08d75562378/1587189?response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%27Tabela1%25281%2529.pdf&response-content-type=application%2Fpdf&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20200507T172221Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=AKIAZH6WM4PLTYPZRQMY%2F20200507%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=f3936156610ea760e6e55249f672a65ad309ec99b97b7668a3861498995b2049) e determine qual algoritimo é melhor para esta tarefa.

> Dica: Se ultilizado eficientemente, o bucket sort gera apenas buckets pequenos.

###

Esperamos que voçe tenha escolhido utilizar o *Insertion Sort* para essa tarefa, tendo em vista que muitas vezes como eles são pequenos, os buckets já estarão ordenados ou até mesmo terão apenas um valor. sua complexidade para vetores pequenos ja ordernados seria O(n).

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

## Questão 5

Qual o problema com essa implementação?


###

Estamos ultilizando o *Insetion Sort* em todos os buckets apesar de provavelmente existirem buckets vazios. Isso pode ser facilmente contornado com um simples *if*.

Agora que temos todos os buckets ordenados podemos finalmente juntar-los em um vetor novamente. Essa implementação é muito simples por isso não vou pedir para você fazer nada, basta usar essa função combine.

    void combine(int *v, int_bucket buckets[], int v_index, int bucket_index)
    {
      for (int data_index = 0; data_index < buckets[bucket_index].size; data_index++)
        v[v_index + data_index] = buckets[bucket_index].data[data_index]; 
    }

## Questão 6

Muita calma nessa hora, sem fechar a aba ainda. Como de costume ainda falta uma parte essencial do handout, a tão adorada análise de complexidade!

Para entender o funcionamento da complexidade deste método, iremos separar o entendimento em 4 partes, sendo elas 2 partes visando um funcionamento macro e micro da complexidade e a outra parte, sendo: 

  1 - Analise do tempo de execução para distribuição do vetor de entrada em buckets.

  2 - Analise do tempo de execução para ordernação de cada bucket.

Começarmos a análise tanto na parte macro quanto micro do caso ideal e desejado pelo Bucket Sort com um tempo linear de ordenação igual a O(n), este que espera buckets com valores já ordenados, consequentemente deixando o ideal para casos em que há poucos valores em cada balde, dado que quanto menos elementos no array dentro de cada balde, mais facil sera de os mesmos ja se encontrarem ordenados, fazendo o tempo de operação ser O(n).

Em nossa analise, consideraremos 2 parâmetros n ( numero de elementos ) e k ( numero de buckets ).

Qual é a complexidade do bucket sort quando se diz a respeito de organizar a entrada em diferentes baldes?

###  

   Não nos leva muito tempo ate entender que será uma complexidade de O(n), tendo em vista que todos os elementos do vetor devem ser percorridos.
   Entretanto, ao tentar compreender a parte 2 da complexidade deste modelo algumas incertezas são criadas.

## Questão 6.a

   Qual seria seu primeiro palpite a respeito da segunda parte da complexidade do modelo?
   
###

   Se você imaginou que a resposta seria $$\ 0(\frac{n}{k})$$ para cada bucket, ou seja $$\ O(k*($$\frac{n}{k}$$))$$, parabens você esta no caminho correto, entretanto ainda faltsam extras para acertar a complexidade.

   A razão para que o que foi dito acima não esta 100% correto é porque falta um fator constante no calculo da complexidade. Quando cada bucket é visitado e cada um dos elementos é analisado, é nitido que não se leva um tempo de execução de $$\frac{n}{k}$$, muito menos algum multiplo constante de $$\frac{n}{k}$$.

## Questão 6.b

   Tente adivinhar o que aconteceria caso o balde estivesse vazio.
   
###

  No caso de um balde vazio, o codigo ainda gasta tempo de execução com o mesmo ja que deve analisar se possui algum valor dentro de cada um dos buckets presentes sendo necessario adicionar a formula um elemento constante c0 e c1 que nada mais são que constantes especificas de implementação, passando a formula para $$\ O(C_0\frac{n}{k}+C_1)$$.
  Ao ver a formula a primeira coisa que provavelmente ira pensar é que nada ira mudar ja que $$\ O(C_0\frac{n}{k}+C_1) = O(\frac{n}{k})$$ , entretanto, tudo ira mudar ao considerarmos o fator k de multiplicação a complexidade, observe:
  
  $$$\ O(n) = O(k*(C_0\frac{n}{k}+C_1)) = O(C_0*n+k*C_1) $$$
   
  Percebemos então que a formula é totalmente dependente de k, fazendo com que o tempo de execução seja $$\ O(n + k)$$

  Agora que entendemos como funciona a análise da complexidade do bucket sort em sua forma esperada (linear), vamos entender o que acontece nos piores casos para o bucket sort, quando ele não é linear.

## Questão 6.c

  Tente adivinhar para quais ou qual caso o bucket sort não ira operar em tempo linear O(n + k).

###

  Por ser um algoritmo de ordenação que depende de outro algoritmo de ordenação, o bucket sort esta sucetivel a considerações extras do outro algoritmo em questão. Como o que utilizamos foi o Insertion Sort, iremos discorrer a respeito do mesmo. O insertion sort é um algoritmo que pode ser levado como quadratico ou linear (no seu caso ideal), dado que para listas relativamente ordenadas, o mesmo opera em tempo de O(n), enquanto para listas que estao pouco ordenadas, opera em O(n²).

  
## Questão 6.d

  Levando isso em consideração, porque um bucket muito desordenado possui maior tempo de operação?

##

  Considerando que teriamos um tempo de operação quadratico e não mais linear podemos considerar os topicos a seguir:
    1. A lista não estara distribuida de forma uniforme. Como por exemplo: [1001,662,331,4,553,63,732,8111,2923,103,1144,26,342,55,77,15623,11137,1382,4557,456,1006]. Tendo em vista que o custo de tempo para cada mudança no array é alta, ao termos uma lista muito desorganizada, teremos muitas mudanças a serem feitas no array, gastando mais tempo conforme for maior a desorganização.
    
    2. A lista esta em completa ordem decrescente, estando sujeito as mesmas condições citas no topico 1,


## Extra:

Disponibilizamos o [código fonte](https://github.com/gabrielnoal/desprog-projeto2/tree/master/codigo) para que você possa brincar.