/*******************************************
 * Biblioteca para manipulação de redes
 * metabólicas.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include "rede_metabolica.h"
#include <stdio.h>
#include <stdlib.h>

int eh_enzima(char *s) {
  return s[0] == '_';
}

// Cria grafo G e lê a rede metabólica do arquivo apontado por <f_met>,
// adicionando em G as reações, enzimas e metabólitos, com as respectivas conexões.
// Devolve o grafo G.
grafo le_rede_metabolica(FILE *f_met) {
  if (!f_met) {
    fprintf(stderr, "Erro ao abrir o arquivo.\n");
    exit(EXIT_FAILURE);
  }

  grafo G = cria_grafo(1);
  char linha[256];
  int id_aresta = 1;

  while (fgets(linha, sizeof(linha), f_met)) {
    // Remove o caractere de nova linha, se presente
    linha[strcspn(linha, "\n")] = '\0';

    // Se a linha contém "FIM", interrompe a leitura
    if (strcmp(linha, "FIM") == 0) {
      break;
    }

    // Parse da linha
    char *token = strtok(linha, " ");
    char *nome_reacao = token;

    // Adiciona a reação ao grafo
    adiciona_vertice(atoi(&nome_reacao[1]), nome_reacao, REACAO, G); // REACAO partição
    vertice reacao = (vertice)busca_chave_str(nome_reacao, G->vertices, (str_f_obj)vertice_rotulo);

    // Processa os substratos (podem ser metabólitos ou enzimas)
    token = strtok(NULL, " ");
    while (token && strcmp(token, "=>") != 0) {
      if (eh_enzima(token)) {
        adiciona_vertice(atoi(&token[2]), token, ENZIMA, G); // ENZIMA partição
        vertice enzima = (vertice)busca_chave_str(token, G->vertices, (str_f_obj)vertice_rotulo);
        adiciona_aresta(id_aresta++, enzima->id, reacao->id, G);
      } else {
        adiciona_vertice(atoi(&token[1]), token, METABOLITO, G); // METABOLITO partição
        vertice substrato = (vertice)busca_chave_str(token, G->vertices, (str_f_obj)vertice_rotulo);
        adiciona_aresta(id_aresta++, substrato->id, reacao->id, G);
      }
      token = strtok(NULL, " +");
    }

    // Processa os produtos (apenas metabólitos)
    token = strtok(NULL, " ");
    while (token && strcmp(token, ".") != 0) {
        adiciona_vertice(atoi(&token[1]), token, METABOLITO, G); // METABOLITO partição (produto)
        vertice produto = (vertice)busca_chave_str(token, G->vertices, (str_f_obj)vertice_rotulo);
        adiciona_aresta(id_aresta++, reacao->id, produto->id, G);
        token = strtok(NULL, " +");
    }
  }

  return G;
}

// Cria lista L e lê da entrada padrão os substratos iniciais, adicionando
// em L os vértices que representam os substratos.
// Devolve a lista de vértices L.
lista le_substratos(grafo G) {

  lista L = cria_lista();
  
  //TODO: implementar...
  
  return L;
}

// Função auxiliar que adiciona na rede metabólica uma reação falsa que gera
// todos os substratos iniciais (dado pela lista de vértices <substratos>).
// Deve adicionar um novo vértice com rótulo "SF" representando o substrato
// falso inicial e um novo vértice com rótulo "RF" representando a reação falsa.
// Deve também adicionar as arestas necessárias.
// ATENÇÃO: os rótulos "SF" e "RF" são usados no método imprime_reacoes_minimas().
void adiciona_reacao_falsa(lista substratos, grafo G) {
  
  //TODO: implementar...
  
}

// Função auxiliar que inicializa os custos e pais dos vértices do grafo G para
// iniciar busca em largura. O custo de uma reação é a quantidade de enzimas que
// a cataliza.
// Devolve uma lista de vértices para ser a Fila inicial do algoritmo de busca.
lista inicializa_custos(grafo G) {
  lista F = cria_lista();
  
  //TODO: implementar...
  
  return F;
}

// Algoritmo variante de Dijkstra que encontra o conjunto de reações de custo
// mínimo (número de enzimas) que produz todos os metabólitos da rede, considerando
// como substratos iniciais os metabólitos representados pelos vértices na lista
// substratos.
// Ao final do algoritmo, cada metabólito deve ter como pai (na estrutura vértice->pai)
// a reação "ótima" que o produz.
void processa(lista substratos, grafo G) {
  
  // adiciona uma reação falsa para iniciar a busca
  adiciona_reacao_falsa(substratos, G);
  
  // inicializa custos, pais e fila inicial da busca F
  lista F = inicializa_custos(G);
  
  // variante do Algoritmo de Dijkstra para resolver o problema
  while (!vazio(F)) {
    
    //TODO: implementar
    
  }
}

// Imprime resultados com base nos pais dos vértices (na estrutura vértice->pai),
// calculados pelo método processa().
void imprime_reacoes_minimas(grafo G) {
  
  // Para cada metabólito, imprime as reações "ótimas"
  for (no n = primeiro_no(vertices(G)); n; n = proximo(n)) {
    vertice v = conteudo(n);
    if (vertice_particao(v) == METABOLITO && pai(v)) {
      printf("%s: ", vertice_rotulo(v));
      
      // cria lista de reações necessárias para sua produção
      lista R = cria_lista();
      empilha(pai(v), R);
      
      // seta todas as reações como ABERTOS, para não repetir
      for (no p = primeiro_no(vertices(G)); p; p = proximo(p)) {
        vertice w = conteudo(p);
        if (vertice_particao(w) == REACAO)
          w->estado = ABERTO;
      }
      
      // Enquanto tiver reações ainda não processadas, imprime
      while (!vazio(R)) {
        vertice w = desempilha(R);
        if (estado(w) == ABERTO && strcmp(vertice_rotulo(w), "RF")) { // não inclui a reação falsa
          printf("%s ", vertice_rotulo(w));
          w->estado = FECHADO;
          
          // coloca na lista as reações antecessoras
          for (no p = primeiro_no(fronteira_entrada(w)); p; p = proximo(p)) {
            aresta e = conteudo(p);
            vertice u = vertice_u(e);
            if (vertice_particao(u) == METABOLITO && pai(u))
              empilha(pai(u), R);
          }
        }
      }
      printf("\n");
      free(R);
    }
  }
}
