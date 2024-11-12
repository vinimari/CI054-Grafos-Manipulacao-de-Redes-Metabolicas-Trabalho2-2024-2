/*******************************************
 * Implementação simples de lista encadeada.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------------------------------------------
// getters:

no proximo(no n) {
  return n->proximo;
}
obj conteudo(no n) {
  return n->conteudo;
}
no primeiro_no(lista l) {
  return l->primeiro_no;
}

//---------------------------------------------------------
// funcoes da lista:

// cria lista vazia e a retorna
lista cria_lista() {
  lista l = (lista) malloc(sizeof(t_lista));
  if (!l)
    exit(_ERRO_MALLOC_);
  l->primeiro_no = NULL;
  return l;
}

// devolve 1 se a lista estiver vazia e 0 c.c.
int vazio(lista l) {
  return primeiro_no(l) == NULL;
}

// devolve o conteudo do inicio da lista e o mantem na lista
obj topo(lista l) {
  if (!vazio(l))
    return conteudo(primeiro_no(l));
  return NULL;
}

// devolve o conteudo do inicio da lista e o remove da lista
obj desempilha(lista l) {
  obj c = topo(l);
  if (c) {
    no aux = proximo(primeiro_no(l));
    free(primeiro_no(l));
    l->primeiro_no = aux;
  }
  return c;
}

// insere o objeto no inicio da lista
void empilha(obj c, lista l) {
  no n = (no) malloc(sizeof(t_no));
  if (!n)
    exit(_ERRO_MALLOC_);
  n->conteudo = c;
  n->proximo = primeiro_no(l);
  l->primeiro_no = n;
}

// devolve o conteudo com a chave <chave> e o mantem na lista
// [a funcao <f_chave(c)> deve devolver a chave do objeto c]
obj busca_chave_int(int chave, lista l, int f_chave(obj c)) {
  no n = primeiro_no(l);
  while (n && f_chave(conteudo(n)) != chave)
    n = proximo(n);
  if (n)
    return conteudo(n);
  return NULL;
}

obj busca_chave_str(char *chave, lista l, char *f_chave(obj c)) {
  no n = primeiro_no(l);
  while (n && strcmp(f_chave(conteudo(n)), chave))
    n = proximo(n);
  if (n)
    return conteudo(n);
  return NULL;
}

// devolve o conteudo com a chave <chave> e o remove da lista
// [a funcao <f_chave(c)> deve devolver a chave do objeto c]
obj remove_chave_int(int chave, lista l, int f_chave(obj c)) {
  no n = primeiro_no(l);
  no aux = NULL;
  while (n && f_chave(conteudo(n)) != chave) {
    aux = n;
    n = proximo(n);
  }
  if (!n)
    return NULL;
  if (aux)
    aux->proximo = proximo(n);
  else
    l->primeiro_no = proximo(n);
  obj c = conteudo(n);
  free(n);
  return c;
}

obj remove_chave_str(char *chave, lista l, char *f_chave(obj c)) {
  no n = primeiro_no(l);
  no aux = NULL;
  while (n && strcmp(f_chave(conteudo(n)), chave)) {
    aux = n;
    n = proximo(n);
  }
  if (!n)
    return NULL;
  if (aux)
    aux->proximo = proximo(n);
  else
    l->primeiro_no = proximo(n);
  obj c = conteudo(n);
  free(n);
  return c;
}

// devolve o conteudo com a menor chave e o mantem na lista
// [a funcao <f_chave(c)> deve devolver a chave do objeto c]
obj busca_min(lista l, int f_chave(obj c)) {
  if (vazio(l))
    return NULL;
  no n = primeiro_no(l);
  obj min = conteudo(n);
  while (n) {
    if (f_chave(conteudo(n)) < f_chave(min))
      min = conteudo(n);
    n = proximo(n);
  }
  return min;
}

// devolve o conteudo com a menor chave e o remove da lista
// [a funcao <f_chave(c)> deve devolver a chave do objeto c]
obj remove_min(lista l, int f_chave(obj c)) {
  if (vazio(l))
    return NULL;
  no n = primeiro_no(l);
  no aux = NULL;
  
  no min = n;
  no min_aux = aux;
  
  while (n) {
    if (f_chave(conteudo(n)) < f_chave(conteudo(min))) {
      min = n;
      min_aux = aux;
    }
    aux = n;
    n = proximo(n);
  }
  if (min_aux)
    min_aux->proximo = proximo(min);
  else
    l->primeiro_no = proximo(min);
  obj c = conteudo(min);
  free(min);
  return c;
}

// imprime a lista
// [a funcao <imprime(c)> deve imprimir o objeto c]
void imprime_lista(lista l, void imprime_conteudo(obj c)) {
  for (no n = primeiro_no(l); n; n = proximo(n)) {
    imprime_conteudo(conteudo(n));
    printf(" ");
  }
}
