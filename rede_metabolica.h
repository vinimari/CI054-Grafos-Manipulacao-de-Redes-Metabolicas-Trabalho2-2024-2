/*******************************************
 * Biblioteca para manipulação de redes
 * metabólicas.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#ifndef _REDE_METABOLICA_
#define _REDE_METABOLICA_

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// flags para tipos possíveis de vértices na rede
#define REACAO 1
#define ENZIMA 2
#define METABOLITO 3

// Cria grafo G e lê a rede metabólica do arquivo apontado por <f_met>,
// adicionando em G as reações, enzimas e metabólitos, com as respectivas conexões.
// Devolve o grafo G.
grafo le_rede_metabolica(FILE *f_met);

// Cria lista L e lê da entrada padrão os substratos iniciais, adicionando
// em L os vértices que representam os substratos.
// Devolve a lista de vértices L.
lista le_substratos(grafo G);

// Função auxiliar que adiciona na rede metabólica uma reação falsa que gera
// todos os substratos iniciais (dado pela lista de vértices <substratos>).
// Deve adicionar um novo vértice com rótulo "SF" representando o substrato
// falso inicial e um novo vértice com rótulo "RF" representando a reação falsa.
// Deve também adicionar as arestas necessárias.
// ATENÇÃO: os rótulos "SF" e "RF" são usados no método imprime_reacoes_minimas().
void adiciona_reacao_falsa(lista substratos, grafo G);

// Função auxiliar que inicializa os custos e pais dos vértices do grafo G para
// iniciar busca em largura. O custo de uma reação é a quantidade de enzimas que
// a cataliza.
// Devolve uma lista de vértices para ser a Fila inicial do algoritmo de busca.
lista inicializa_custos(grafo G);

// Algoritmo variante de Dijkstra que encontra o conjunto de reações de custo
// mínimo (número de enzimas) que produz todos os metabólitos da rede, considerando
// como substratos iniciais os metabólitos representados pelos vértices na lista
// substratos.
// Ao final do algoritmo, cada metabólito deve ter como pai (na estrutura vértice->pai)
// a reação "ótima" que o produz.
void processa(lista substratos, grafo G);

// Imprime resultados com base nos pais dos vértices (na estrutura vértice->pai),
// calculados pelo método processa().
void imprime_reacoes_minimas(grafo G);

#endif
