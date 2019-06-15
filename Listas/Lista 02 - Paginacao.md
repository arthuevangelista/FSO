Lista 2 - Paginação

1- Nos sistemas com paginação, a rotina para tratamento de
page faults está residente na memória principal. Esta rotina pode ser
removida da memória em algum momento? O que aconteceria se esta rotina não
estivesse na memória principal durante a ocorrência de um page fault?

2- Um certo computador oferece a seus usuários um espaço de
memória virtual de $2^{32}$ bytes. O computador tem $2^{18}$ bytes de
memória física. A memória virtual é implementada por paginação e o tamanho
da página é de $4096$ bytes. Um processo de usuário gera o endereço virtual
$11123456$. Explique como o sistema estabelece a correspondente posição
física. Faça distinção entre operações de software e de hardware.

3- Você imaginou um novo algoritmo de substituição de
páginas que você acha que possa ser ótimo. Em alguns casos de teste
distorcidos, ocorreu anomalia de Belady. O novo algoritmo é ótimo? Explique
a sua resposta.

4- Para que serve o bit de validade nas tabelas de páginas?

5- A alocação contígua de arquivos leva a uma fragmentação
do disco, explique como essa fragmentação ocorre? Essa fragmentação í©
interna ou externa? Faça uma analogia com a gerência de memória.

6- É possível criar um link simbólico para um arquivo que
não existe? E um hard link? Justifique sua resposta.

7- Compare e explique as vantagens e desvantagens de se utilizar o
armazenamento dos arquivos por lista ligada e por tabela de alocação em
memória.

8- Comente vantagens da utilização de i-nodes, sobre o uso das tabelas em
memória.

9- Alguns sistemas operacionais fornecem uma chamada de
sistema \textit{rename} para atribuir um novo nome a um arquivos. Há alguma
diferença enter usar essa chamada para dar um novo nome a um arquivo e
apenas copiá-lo para um novo arquivo com o nome nome e depois remover o
antig? Explique a sua resposta.

10- Por que razão o sistema UNIX não permite hard links de
diretórios?

11- Exercícios 1 a 18 do capítulo 4 (página 200 e 201) do livro texto da
disciplina (Sistemas Operacionais Modernos)
