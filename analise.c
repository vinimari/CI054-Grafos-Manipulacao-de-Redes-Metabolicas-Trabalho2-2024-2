/*******************************************
 * Análise de redes metabólicas.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include "rede_metabolica.h"
#include <stdio.h>
#include <stdlib.h>

// ./analise <arq.met> < <substratos.in>
int main(int argc, char **argv) {
  
  // le arquivo .met e cria rede metabolica
  FILE *f_met = fopen(argv[1], "r");
  grafo G = le_rede_metabolica(f_met);
  fclose(f_met);
  
  // imprime rede metabólica
  imprime_grafo(G);
  
  // le substratos iniciais
  lista substratos = le_substratos(G);

  // processa rede metabolica
  processa(substratos, G);
  
  // imprime resultados
  imprime_reacoes_minimas(G);
  
  return 0;
}
