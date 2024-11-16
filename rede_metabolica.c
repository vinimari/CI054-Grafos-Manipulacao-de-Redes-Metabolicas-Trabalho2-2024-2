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

// Função auxiliar para verificar se uma string representa uma enzima
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
  int id_vertice = 1;

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
    vertice reacao = checa_e_adiciona_vertice(&id_vertice, nome_reacao, REACAO, G);

    // Processa os substratos (podem ser metabólitos ou enzimas)
    token = strtok(NULL, " ");
    while (token && strcmp(token, "=>") != 0) {
      if (eh_enzima(token)) {
        vertice enzima = checa_e_adiciona_vertice(&id_vertice, token, ENZIMA, G);
        adiciona_aresta(id_aresta++, vertice_id(enzima), vertice_id(reacao), G);
      } else {
        vertice substrato = checa_e_adiciona_vertice(&id_vertice, token, METABOLITO, G);
        adiciona_aresta(id_aresta++, vertice_id(substrato), vertice_id(reacao), G);
      }
      token = strtok(NULL, " +");
    }

    // Processa os produtos
    token = strtok(NULL, " ");
    while (token && strcmp(token, ".") != 0) {
        vertice produto = checa_e_adiciona_vertice(&id_vertice, token, METABOLITO, G);
        adiciona_aresta(id_aresta++,  vertice_id(reacao), vertice_id(produto), G);
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
  char linha[256];
  
  // Lê a linha de substratos da entrada padrão
  if (fgets(linha, sizeof(linha), stdin) != NULL) {
    linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha, se presente

    // Divide a linha em substratos
    char *token = strtok(linha, " ");
    while (token && strcmp(token, "FIM") != 0) {
      // Busca o vértice correspondente ao substrato no grafo
      vertice v = (vertice)busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo);
      if (v) {
        empilha(v, L);
      } else {
        fprintf(stderr, "Aviso: substrato %s não encontrado na rede metabólica.\n", token);
      }
      token = strtok(NULL, " ");
    }
  } else {
    fprintf(stderr, "Erro ao ler a entrada dos substratos.\n");
    exit(EXIT_FAILURE);
  }

  return L;
}

// Função auxiliar que adiciona na rede metabólica uma reação falsa que gera
// todos os substratos iniciais (dado pela lista de vértices <substratos>).
// Deve adicionar um novo vértice com rótulo "SF" representando o substrato
// falso inicial e um novo vértice com rótulo "RF" representando a reação falsa.
// Deve também adicionar as arestas necessárias.
// ATENÇÃO: os rótulos "SF" e "RF" são usados no método imprime_reacoes_minimas().
void adiciona_reacao_falsa(lista substratos, grafo G) {
  // Cria o vértice "SF" (substrato falso inicial)
  adiciona_vertice(0, "SF", METABOLITO, G);
  vertice substrato_falso = (vertice)busca_chave_str("SF", vertices(G), (str_f_obj)vertice_rotulo);

  // Cria o vértice "RF" (reação falsa)
  adiciona_vertice(0, "RF", REACAO, G);
  vertice reacao_falsa = (vertice)busca_chave_str("RF", vertices(G), (str_f_obj)vertice_rotulo);

  // Conecta "SF" a "RF"
  adiciona_aresta(0, vertice_id(substrato_falso), vertice_id(reacao_falsa), G);

  // Conecta "RF" a todos os substratos iniciais na lista
  no n = primeiro_no(substratos);
  while (n) {
    vertice substrato = (vertice)conteudo(n);
    adiciona_aresta(0, vertice_id(reacao_falsa), vertice_id(substrato), G); // Aresta de "RF" para cada substrato
    n = proximo(n);
  }
}

// Função auxiliar que inicializa os custos e pais dos vértices do grafo G para
// iniciar busca em largura. O custo de uma reação é a quantidade de enzimas que
// a cataliza. 
// Devolve uma lista de vértices para ser a Fila inicial do algoritmo de busca.
lista inicializa_custos(grafo G) {
  lista F = cria_lista();

  for (no n = primeiro_no(vertices(G)); n; n = proximo(n)) {
    vertice v = conteudo(n);
    v->custo = INT_MAX;
    v->pai = NULL;
    
    if (!strcmp(vertice_rotulo(v), "SF")) {
      v->custo = 0;       
    }

    if (vertice_particao(v) == REACAO) {
      v->custo = calcula_custo_reacao(&v); 
    }
    
    if (vertice_particao(v) != ENZIMA) {
      empilha(v, F);
    }
  }

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
  
  // variante do algoritmo de Dijkstra para resolver o problema
  while (!vazio(F)) {
    vertice u = (vertice)remove_min(F, (int_f_obj)custo);
    if (!u) break;

    // Processa todos os vértices adjacentes a u
    no n = primeiro_no(fronteira_saida(u));
    while (n) {
      aresta e = (aresta)conteudo(n);
      vertice v = vertice_v(e); // Vértice adjacente

      // Se encontrar um caminho melhor, atualiza o custo e o pai de v
      if (custo(u) < custo(v)) {
        v->custo = custo(u);
        v->pai = u;
      } else if (custo(u) == custo(v)) {
        // Se o custo for igual, verifica o critério de desempate
        int profundidade_atual = calcula_profundidade_vertice(v->pai);
        int profundidade_novo = calcula_profundidade_vertice(u);

        if (profundidade_novo < profundidade_atual) {
          // Atualiza o pai se o novo caminho for mais raso (caminho mais curto)
          v->pai = u;
        }
      }

      n = proximo(n);
    }
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

// Calcula e retorna o custo das reações (quantidade de enzimas)
int calcula_custo_reacao(vertice *v) {
  int num_enzimas = 0;
  
  for (no n = primeiro_no(fronteira_entrada(*v)); n; n = proximo(n)) {
    aresta e = conteudo(n);
    vertice u = vertice_u(e);
    if (vertice_particao(u) == ENZIMA) {
      num_enzimas++;
    }
  }

  return num_enzimas;
}
