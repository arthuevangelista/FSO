Lista 1 - Deadlocks e Memória e Memória Virtual

DEADLOCKS
---------

1) Liste três exemplos de deadlocks não relacionados a um ambiente de
computação.

2) É possí­vel haver um deadlock envolvendo apenas um processo? Explique sua
resposta.


MEMÓRIA
-------

1)Considere um sistema cuja gerência de memória é feita através de partições
variáveis. Nesse momento, existem as seguintes lacunas (áreas livres): 10K,
4K, 20K, 18K, 7K, 9K, 12K e 13K, nessa ordem. Quais espaços serão ocupados
pelas solicitações: 5K, 10K e 6K, nessa ordem, se:
    - first-fit for utilizado?
    - best-fit for utilizado?
    - worst-fit for utilizado?

2)calcule o número de bits para página, quadro e deslocamento.
  - Memória lógica = 512K, Memória física = 512K, Tamanho de página = 8K
  - Tamanho de página = 4K, Número de páginas = 8, Número de quadros = 16

3) O que é paginação?
4) Qual é a diferença entre paginação e segmentação?

5)Considere que os processos da tabela abaixo devem ser executados em um SO
com paginação. A memória total é de 64K, o tamanho das páginas é de 4K e o
SO ocupa 8K. Mostre como seria a alocação de quadros para cada processo.
Processo | Mem. (K) | T. Cheg | T. Exec
P1         8          0          10
P2         6          1          15
P3         15         2          10
P4         5          3          14
P5         10         15         5

5) Qual a diferença entre fragmentação interna e externa da memória
principal?

6) Qual a principal diferença entre os sistemas que implementam paginação e
segmentação?

7) Para que serve o bit de validade nas tabelas de páginas e segmentos?

8) Nos sistemas com paginação, a rotina para tratamento de page faults está
residente na memória principal. Esta rotina pode ser removida da memória em
algum momento? O que aconteceria se esta rotina não estivesse na memória
principal durante a ocorrência de um page fault?

9) Descreva como ocorre a fragmentação interna em um sistema que implementa
paginação.

VIRTUAL
-------

1) O que é memória virtual?

2) Quais das seguintes técnicas e estruturas de programação são "boas" para
um ambiente de paginação por demanda? Quais delas são "más"? Explique suas
respostas.
    a. Pilha
    b. Tabela de sí­mbolos submetida à  função hash
    c. Busca sequêncial
    d. Busca Binária
    e. Código Puro
    f. Operações em vetor
    g. Simulação

3) Considere os seguintes algoritmos de substituição de páginas. Classifique
estes algoritmos em uma escala de cinco pontos do "ruim" ao "perfeito" de
acordo com a sua taxa de erros de página. Separe os algoritmos afetados pela
anomalia de Belady daqueles que não o são.
    a. Substituição LRU
    b. Substituição FIFO
    c. Substituição ótima
    d. Substituição da segunda chance

4) Considere a seguinte sequência de referências de páginas:
    1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6

Quantos erros de páginas iriam ocorrer para os seguintes algoritmos de
substituição , considerando um, dois, três, quatro, cinco, seis ou sete
quadros? Lembre-se de que todos os quadros estão inicialmente vazios, de
modo que a primeira página de cada um implicará em um erro de página.
    - Substituição LRU
    - Substituição ótima
    - Substituição do Relógio
    - Substituição WSClock


5) Você consegue imaginar alguma situação em que dar suporte à  memória
virtual seria uma má ideia e o que se ganha quando não é necessário o
suporte a memória virtual? Explique

6) Exercí­cios do capí­tulo 3 do Tanembaum
