# README

## Aluno
Márcio Vinícius de Carvalho Marinho  
GRR20204089

---

## Estruturas de Dados e Funções
Nenhuma estrutura de dados ou função previamente pronta foi modificada.

---

## Funções Auxiliares Criadas

### 1. `int eh_enzima(char *s);`
Função auxiliar para verificar se uma string representa uma enzima.

### 2. `vertice checa_e_adiciona_vertice(int *id_vertice, char *rotulo, int particao, grafo G);`
Adiciona um vértice ao grafo se ele não existir. Retorna o ponteiro para o vértice, seja o existente ou o recém-adicionado.

### 3. `int calcula_custo_reacao(vertice *v);`
Calcula e retorna o custo das reações (quantidade de enzimas).

---

## Detalhes sobre a Implementação da Função `processa`

A função `processa` foi implementada baseando-se no algoritmo de Dijkstra simplificado. Foi acrescentado um critério de desempate: para reações com o mesmo custo, o algoritmo avalia se o novo caminho analisado possui uma profundidade menor, priorizando assim o caminho mais direto.

---

## Execução do Programa

Para compilar e executar o programa, use os seguintes comandos:

```bash
$ make
$ ./analise rede.met < substratos.in
