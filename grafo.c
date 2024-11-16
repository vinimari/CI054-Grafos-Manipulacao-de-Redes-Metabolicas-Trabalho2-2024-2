/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------
// getters:

int vertice_id(vertice v) {
  return v->id;
}
char* vertice_rotulo(vertice v) {
  return v->rotulo;
}
int vertice_particao(vertice v) {
  return v->particao;
}
int custo(vertice v) {
  return v->custo;
}
int estado(vertice v) {
  return v->estado;
}
vertice pai(vertice v) {
  return v->pai;
}
lista fronteira_entrada(vertice v) {
  return v->fronteira_entrada;
}
lista fronteira_saida(vertice v) {
  return v->fronteira_saida;
}
int aresta_id(aresta e) {
  return e->id;
}
vertice vertice_u(aresta e) {
  return e->u;
}
vertice vertice_v(aresta e) {
  return e->v;
}
int grafo_id(grafo G) {
  return G->id;
}
lista vertices(grafo G) {
  return G->vertices;
}
lista arestas(grafo G) {
  return G->arestas;
}

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo(int id) {
  grafo G = (grafo)malloc(sizeof(struct t_grafo));
  if (!G) {
    fprintf(stderr, "Erro ao alocar memória para o grafo.\n");
    exit(EXIT_FAILURE);
  }
  G->id = id;
  G->vertices = cria_lista(); 
  G->arestas = cria_lista();
  return G;
}

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G) {
  if (!G) return;

  // Remove e desaloca todos os vértices
  while (!vazio(G->vertices)) {
    vertice v = (vertice) desempilha(G->vertices);
    if (v) {
      while (!vazio(v->fronteira_entrada))
        free(desempilha(v->fronteira_entrada)); 
      
      while (!vazio(v->fronteira_saida))
        free(desempilha(v->fronteira_saida));
      free(v);
    }
  }
  
  // Remove e desaloca todas as arestas
  while (!vazio(G->arestas))
    free(desempilha(G->arestas));
  
  free(G);
}

// Adiciona um vértice ao grafo se ele não existir.
// retorna o ponteiro para o vértice, seja o existente ou o recém-adicionado.
vertice checa_e_adiciona_vertice(int *id_vertice, char *rotulo, int particao, grafo G) {
  vertice v = (vertice)busca_chave_str(rotulo, vertices(G), (str_f_obj)vertice_rotulo);
  if (!v) {
    adiciona_vertice((*id_vertice)++, rotulo, particao, G);
    v = (vertice)busca_chave_str(rotulo, vertices(G), (str_f_obj)vertice_rotulo);
  }
  return v;
}

// cria novo vertice com id <id>, rotulo <rotulo>, particao <particao>
// e adiciona ao grafo G
void adiciona_vertice(int id, char *rotulo, int particao, grafo G) {
  vertice v = (vertice) malloc(sizeof(struct t_vertice));
  if (!v) {
    fprintf(stderr, "Erro ao alocar memória para o vértice.\n");
    exit(EXIT_FAILURE);
  }

  v->id = id;
  strncpy(v->rotulo, rotulo, sizeof(v->rotulo) - 1);
  v->rotulo[sizeof(v->rotulo) - 1] = '\0'; // garante a terminação nula
  v->particao = particao;
  v->custo = 0;
  v->estado = 0;
  v->pai = NULL;
  v->fronteira_entrada = cria_lista(); 
  v->fronteira_saida = cria_lista();  
  empilha(v, G->vertices); 
}

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G) {
  vertice v = (vertice) remove_chave_int(id, G->vertices, (int_f_obj)vertice_id);
  if (!v) return;
  
  // Remove e desaloca todas as arestas incidentes
  while (!vazio(fronteira_entrada(v)))
    free(desempilha(fronteira_entrada(v)));
  while (!vazio(fronteira_saida(v)))
    free(desempilha(fronteira_saida(v)));
  
  free(v);
}

// cria aresta com id <id> incidente a vertices com ids
// <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G) {
  vertice u = (vertice) busca_chave_int(u_id, vertices(G), (int_f_obj)vertice_id);
  vertice v = (vertice) busca_chave_int(v_id, vertices(G), (int_f_obj)vertice_id);
  if (!u || !v) {
    fprintf(stderr, "Erro: vértices não encontrados para criar a aresta.\n");
    return;
  }

  aresta e = (aresta) malloc(sizeof(struct t_aresta));
  if (!e) {
    fprintf(stderr, "Erro ao alocar memória para a aresta.\n");
    exit(EXIT_FAILURE);
  }

  e->id = id;
  e->u = u;
  e->v = v;
  empilha(e, arestas(G));           
  empilha(e, fronteira_saida(u));  
  empilha(e, fronteira_entrada(v));
}

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G) {
  aresta e = (aresta) remove_chave_int(id, G->arestas, (int_f_obj)aresta_id);
  if (!e) return;
  
  remove_chave_int(id, e->u->fronteira_saida, (int_f_obj)aresta_id);
  remove_chave_int(id, e->v->fronteira_entrada, (int_f_obj)aresta_id);
  free(e);
}

// Conta e retorna a profundidade (ou o numero de pais) de um vértice.
int calcula_profundidade_vertice(vertice v) {
  int profundidade = 0;
  while (v != NULL) {
    v = pai(v);
    profundidade++;
  }
  return profundidade;
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve os graus do vertice v
int grau_entrada(vertice v) {
  int d_v = 0;
  for (no n = primeiro_no(fronteira_entrada(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}
int grau_saida(vertice v) {
  int d_v = 0;
  for (no n = primeiro_no(fronteira_saida(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}

// imprime o grafo G
void imprime_grafo(grafo G) {
  /* imprimindo apenas a estrutura do grafo
  printf("%d\n", grafo_id(G));
  printf("\nVertices: ");
  imprime_lista(vertices(G), (void_f_obj) imprime_vertice);
  printf("\nArestas: ");
  imprime_lista(arestas(G), (void_f_obj) imprime_aresta);
  printf("\n");
  printf("\nEstrutura:\n");
  */
  imprime_lista(arestas(G), (void_f_obj) imprime_estrutura_aresta);
}

// imprime o vertice v
void imprime_vertice(vertice v) {
  printf("(id:%d, rotulo:%s, grau_entrada:%d, fronteira_entrada:{ ", vertice_id(v), vertice_rotulo(v), grau_entrada(v));
  imprime_lista(fronteira_entrada(v), (void_f_obj) imprime_aresta);
  printf("}, grau_saida:%d, fronteira_saida:{ ", grau_saida(v));
  imprime_lista(fronteira_saida(v), (void_f_obj) imprime_aresta);
  printf("})");
}

// imprime a aresta e
void imprime_aresta(aresta e) {
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("(id:%d {%d,%d})", aresta_id(e), u_id, v_id);
}

// imprime aresta e no formato de entrada para
// https://graphonline.ru/en/create_graph_by_edge_list
void imprime_estrutura_aresta(aresta e) {
  char *u_rot = vertice_rotulo(vertice_u(e));
  char *v_rot = vertice_rotulo(vertice_v(e));
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("%d:%s > %d:%s\n", u_id, u_rot, v_id, v_rot);
}
