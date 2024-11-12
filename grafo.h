/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#ifndef _GRAFO_
#define _GRAFO_

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // INT_MAX

// flags para vertice->estado (usado em algoritmos de busca)
#define ABERTO 1
#define PROCESSADO 2
#define FECHADO 3

typedef struct t_vertice *vertice;
typedef struct t_aresta *aresta;
typedef struct t_grafo *grafo;

typedef struct t_vertice {
  int id;
  char rotulo[10];
  int particao;     // para grafos multi-partidos
  int custo;        // para algoritmos de busca
  int estado;
  vertice pai;
  lista fronteira_entrada;
  lista fronteira_saida;
} t_vertice;

typedef struct t_aresta {
  int id;
  vertice u, v;     // direcionada de u para v
} t_aresta;

typedef struct t_grafo {
  int id;
  lista vertices;
  lista arestas;
} t_grafo ;

//---------------------------------------------------------
// getters:

int vertice_id(vertice v);
char* vertice_rotulo(vertice v);
int vertice_particao(vertice v);
int custo(vertice v);
int estado(vertice v);
vertice pai(vertice v);
lista fronteira_entrada(vertice v);
lista fronteira_saida(vertice v);
int aresta_id(aresta e);
vertice vertice_u(aresta e);
vertice vertice_v(aresta e);
int grafo_id(grafo G);
lista vertices(grafo G);
lista arestas(grafo G);

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo(int id);

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G);

// cria novo vertice com id <id>, rotulo <rotulo>, particao <particao>
// e adiciona ao grafo G
void adiciona_vertice(int id, char *rotulo, int particao, grafo G);

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G);

// cria aresta com id <id> incidente a vertices com ids
// <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G);

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G);

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve os graus do vertice v
int grau_entrada(vertice v);
int grau_saida(vertice v);

// imprime o grafo G
void imprime_grafo(grafo G);

// imprime o vertice v
void imprime_vertice(vertice v);

// imprime a aresta e
void imprime_aresta(aresta e);

// imprime aresta e no formato de entrada para
// https://graphonline.ru/en/create_graph_by_edge_list
void imprime_estrutura_aresta(aresta e);

#endif
