#include "tabuleiro.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>

struct jogador {
  Cor cor;
  bool** espacosOcupados;
  size_t numJogadas;
};

struct espacoTabuleiro {
  Coordenadas coord;
  Dir podeJogarPara;
  Pilha* pilha;
  size_t tamanho;
};

struct tabuleiro {
  size_t tamanho;
  size_t numJogadores;
  Jogador* jogadores;
  EspacoTabuleiro** espacos;
};

#define NUM_MAX_JOGADORES 5
#define INDICE_INVALIDO NUM_MAX_JOGADORES

static const Cor CORES[NUM_MAX_JOGADORES] = {Cor_Vermelho, Cor_Verde, Cor_Azul, Cor_Amarelo, Cor_Roxo};

static CorComIndice** aleatorizar_tabuleiro(size_t tamanho, size_t numJogadores, Erro* err) {
  size_t numPecas = tamanho*tamanho;
  CorComIndice** ptab = (CorComIndice**)malloc_matrix(sizeof(CorComIndice), tamanho, tamanho, err);
  if (ptab == nullptr) {
    return nullptr;
  }

  bool ladosRemovidos = false;
  bool meioRemovido = false;
  if (numPecas % numJogadores != 0) {
    numPecas -= 4;
    ladosRemovidos = true;
    if (numPecas % numJogadores != 0) {
      numPecas -= 1;
      meioRemovido = true;
      if (numPecas % numJogadores != 0) {
	numPecas += 4;
	ladosRemovidos = false;
      }
    }
  }
  size_t maxPecasPorJogador = numPecas / numJogadores;
  size_t* pecasPorJogador = malloc_validado(sizeof(size_t)*numJogadores, err);
  if (pecasPorJogador == nullptr) {
    return nullptr;
  }
  Cor cores_permitidas = Cor_Nada;
  for (size_t i = 0; i < numJogadores; i++) {
    pecasPorJogador[i] = maxPecasPorJogador;
    cores_permitidas = cores_permitidas | CORES[i];
  }

  for (size_t i = 0; i < tamanho; i++) {
    for (size_t j = 0; j < tamanho; j++) {
      if ((ladosRemovidos && ((i == 0 && (j == 0 || j == tamanho-1)) ||
	                      (i == tamanho-1 && (j == 0 || j == tamanho-1)))) ||
          (meioRemovido && i == tamanho/2 && j == tamanho/2)) {
        ptab[i][j] = (CorComIndice){INDICE_INVALIDO, Cor_Nada};	
      } else {
        CorComIndice ci = cor_aleatoria(cores_permitidas);
	ptab[i][j] = ci;
	pecasPorJogador[ci.indice] -= 1;
	if (pecasPorJogador[ci.indice] == 0) {
	  cores_permitidas = cores_permitidas - ci.cor;
	}
      }
    }
  }

  return ptab;
}

static void atualizarJogada(Dir* podeJogarPara, Dir dir, bool condicao) {
  if (condicao) {
    *podeJogarPara = *podeJogarPara | dir;
  }
}

static const size_t TAMANHO_DIRECAO[256] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

Tabuleiro* tabuleiro_criar(size_t tamanho, size_t numJogadores, Erro* err) {
  Tabuleiro* tab = malloc_validado(sizeof(Tabuleiro), err);
  if (tab != nullptr) {
    CorComIndice** cs = aleatorizar_tabuleiro(tamanho, numJogadores, err);
    if (cs == nullptr) {
      return nullptr;
    }

    tab->tamanho = tamanho;
    tab->numJogadores = numJogadores;

    tab->espacos = (EspacoTabuleiro**)malloc_matrix(sizeof(EspacoTabuleiro), tamanho, tamanho, err);
    if (tab->espacos == nullptr) {
      return nullptr;
    }
    for (size_t i = 0; i < tamanho; i++) {
      for (size_t j = 0; j < tamanho; j++) {
	EspacoTabuleiro* e = &tab->espacos[i][j];
	e->coord = COORD(j,i);
	e->pilha = pilha_criar(err);
	if (e->pilha == nullptr) {
	  return nullptr;
	}
	if (cs[i][j].cor != Cor_Nada) {
	  e->tamanho = 1;
	  pilha_inserir(e->pilha, cs[i][j], err);

	  Dir* p = &e->podeJogarPara;
          *p = 0;
	  atualizarJogada(p, Dir_Norte, i > 0 && cs[i-1][j].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Sul,   i < tamanho-1 && cs[i+1][j].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Oeste, j > 0 && cs[i][j-1].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Leste, j < tamanho-1 && cs[i][j+1].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Nordeste, i > 0 && j < tamanho-1 && cs[i-1][j+1].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Noroeste, i > 0 && j > 0 && cs[i-1][j-1].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Sudeste,  i < tamanho-1 && j < tamanho-1 && cs[i+1][j+1].cor != Cor_Nada);
	  atualizarJogada(p, Dir_Sudoeste, i < tamanho-1 && j > 0 && cs[i+1][j-1].cor != Cor_Nada);
	} else { 
	  e->tamanho = 0;
	  e->podeJogarPara = 0;
	}
      }
    }

    tab->jogadores = malloc_validado(sizeof(Jogador)*numJogadores, err);
    if (tab->jogadores == nullptr) {
      return nullptr;
    }
    for (size_t i = 0; i < numJogadores; i++) {
      Jogador* jg = &tab->jogadores[i];
      jg->cor = CORES[i];
      jg->numJogadas = 0;
      jg->espacosOcupados = (bool**)malloc_matrix(sizeof(bool), tamanho, tamanho, err);
      if (jg->espacosOcupados == nullptr) {
	return nullptr;
      }
      for (size_t y = 0; y < tamanho; y++) {
        for (size_t x = 0; x < tamanho; x++) {
	  jg->espacosOcupados[y][x] = cs[y][x].cor == jg->cor;
	  if (jg->espacosOcupados[y][x]) {
	    jg->numJogadas += TAMANHO_DIRECAO[tab->espacos[y][x].podeJogarPara];
	  }
        }
      }
    }
  }
  return tab;
}

void tabuleiro_mostrar(Tabuleiro* tab, Erro* err) {
  char p[21];
  printf("   |  ");
  for (size_t i = 0; i < tab->tamanho; i++) {
    printf(" %zu", i);
  }
  printf("  |\n");
  for (size_t i = 0; i < tab->tamanho; i++) {
    printf(" %zu |  ", i);
    for (size_t j = 0; j < tab->tamanho; j++) {
      size_t tamanho = tab->espacos[i][j].tamanho;
      if (tamanho == 0) {
        printf("  ");
      } else {
        Cor topo = pilha_topo(tab->espacos[i][j].pilha, err).cor;
	if (*err != Successo) {
	  return;
	}
	if (tamanho > 1) {
	  if (sprintf(p, "%2zu", tamanho) == 2) {
	    imprimir_com_cor_de_fundo(p, topo);
	  } else {
	    *err = Erro_TamanhoAcimaDoLimite;
            return;
	  }
	} else {
	  imprimir_com_cor_de_fundo("  ", topo);
	}
      }
    }
    printf("  |\n");
  }
}

static EspacoTabuleiro* tabuleiro_espaco_na_direcao(Tabuleiro* tab, Coordenadas coord, Dir dir) {
  size_t x = coord.x;
  size_t y = coord.y;
  Coordenadas dest;
  switch(dir) {
    case Dir_Norte: dest = COORD(x,y-1); break;
    case Dir_Sul:   dest = COORD(x,y+1); break;
    case Dir_Oeste: dest = COORD(x-1,y); break;
    case Dir_Leste: dest = COORD(x+1,y); break;
    case Dir_Nordeste: dest = COORD(x+1,y-1); break;
    case Dir_Noroeste: dest = COORD(x-1,y-1); break;
    case Dir_Sudeste:  dest = COORD(x+1,y+1); break;
    case Dir_Sudoeste: dest = COORD(x-1,y+1); break;
  }
  if (dest.x < tab->tamanho && dest.y < tab->tamanho) {
    return &tab->espacos[dest.y][dest.x];
  } else {
    return nullptr;
  }
}

static const Dir DIRECAO[8] = {
  Dir_Sul,
  Dir_Norte,
  Dir_Leste,
  Dir_Oeste,
  Dir_Nordeste,
  Dir_Noroeste,
  Dir_Sudeste,
  Dir_Sudoeste,
};

static const Dir DIRECAO_INVERSA[8] = {
  Dir_Norte,
  Dir_Sul,
  Dir_Oeste,
  Dir_Leste,
  Dir_Sudoeste,
  Dir_Sudeste,
  Dir_Noroeste,
  Dir_Nordeste,
};

bool tabuleiro_espaco_pertence_a(Tabuleiro* tab, Coordenadas coord, size_t jg, Erro* err) {
  return pilha_topo(tab->espacos[coord.y][coord.x].pilha, err).indice == jg;
}

bool tabuleiro_mover(Tabuleiro* tab, Coordenadas coord, Dir dir, Erro* err) {
  size_t x = coord.x;
  size_t y = coord.y;
  if ((tab->espacos[y][x].podeJogarPara & dir) != 0) {
    EspacoTabuleiro* orig = &tab->espacos[y][x];
    EspacoTabuleiro* dest = tabuleiro_espaco_na_direcao(tab, coord, dir);
    size_t jogadorDest = pilha_topo(dest->pilha, err).indice;
    pilha_unir(dest->pilha, orig->pilha, err);
    if (*err != Successo) {
      return false;
    }
    size_t jogador = pilha_topo(dest->pilha, err).indice;
    size_t jogadasRemovidas[NUM_MAX_JOGADORES];
    size_t jogadasNovas[NUM_MAX_JOGADORES];

    for (size_t jg = 0; jg < tab->numJogadores; jg++) {
      jogadasRemovidas[jg] = 0;
      jogadasNovas[jg] = 0;
    }

    jogadasRemovidas[jogador] += TAMANHO_DIRECAO[orig->podeJogarPara];
    jogadasRemovidas[jogadorDest] += TAMANHO_DIRECAO[dest->podeJogarPara];

    dest->tamanho = orig->tamanho * 2;
    dest->podeJogarPara = 0;

    orig->tamanho = 0;
    orig->podeJogarPara = 0;

    for (size_t n = 0; n < 8; n++) {
      Dir p = DIRECAO[n];
      EspacoTabuleiro* test = tabuleiro_espaco_na_direcao(tab, orig->coord, p);
      if (test != nullptr && test->tamanho > 0) {
        size_t jogadorTest = pilha_topo(test->pilha, err).indice;
	Dir q = DIRECAO_INVERSA[n];
	if ((test->podeJogarPara & q) != 0) {
	  test->podeJogarPara -= q;
	  jogadasRemovidas[jogadorTest] += 1;
	}
      }
    }

    for (size_t n = 0; n < 8; n++) {
      Dir p = DIRECAO[n];
      EspacoTabuleiro* test = tabuleiro_espaco_na_direcao(tab, dest->coord, p);
      if (test != nullptr && test->tamanho > 0) {
        size_t jogadorTest = pilha_topo(test->pilha, err).indice;
	Dir q = DIRECAO_INVERSA[n];
	if (test->tamanho == dest->tamanho) {
	  if ((test->podeJogarPara & q) == 0) {
	    jogadasNovas[jogadorTest] += 1;
	  }
	  jogadasNovas[jogador] += 1;
          dest->podeJogarPara = dest->podeJogarPara | p;
	  test->podeJogarPara = test->podeJogarPara | q;
	} else if ((test->podeJogarPara & q) != 0) {
	  test->podeJogarPara -= q;
	  jogadasRemovidas[jogadorTest] += 1;
	}
      }
    }

    for (size_t jg = 0; jg < tab->numJogadores; jg++) {
      tab->jogadores[jg].numJogadas += jogadasNovas[jg];
      tab->jogadores[jg].numJogadas -= jogadasRemovidas[jg];
    }

    return true;
  } else {
    return false;
  }
}

bool tabuleiro_pode_jogar(Tabuleiro* tab, size_t jogadorIndice) {
  return tab->jogadores[jogadorIndice].numJogadas > 0;
}

Pilha** tabuleiro_finalizar_resultado(Tabuleiro* tab, Erro* err) {
  Pilha** resultado = malloc_validado(sizeof(Pilha*)*tab->numJogadores, err);
  if (resultado != nullptr) {
    for (size_t jg = 0; jg < tab->numJogadores; jg++) {
      resultado[jg] = pilha_criar(err);
    }
    for (size_t i = 0; i < tab->tamanho; i++) {
      for (size_t j = 0; j < tab->tamanho; j++) {
	if (tab->espacos[i][j].tamanho > 0) {
	  size_t jg = pilha_topo(tab->espacos[i][j].pilha, err).indice;
	  pilha_unir(resultado[jg], tab->espacos[i][j].pilha, err);
	  tab->espacos[i][j].tamanho = 0;
	}
      }
    }
  }
  return resultado;
}

Jogada tabuleiro_jogar_cpu(Tabuleiro* tab, size_t jg, Erro* err) {
  static Coordenadas XY[100];
  size_t tamanho_xy = 0;
  size_t tamanho_max = 0;

  for (size_t i = 0; i < tab->tamanho; i++) {
    for (size_t j = 0; j < tab->tamanho; j++) {
      EspacoTabuleiro* e = &tab->espacos[i][j];
      if (e->tamanho > 0 && e->podeJogarPara != 0 && pilha_topo(e->pilha, err).indice == jg) {
	if (e->tamanho > tamanho_max) {
	  tamanho_xy = 1;
	  XY[0] = e->coord;
	  tamanho_max = e->tamanho;
	} else if (e->tamanho == tamanho_max) {
	  XY[tamanho_xy] = e->coord;
	  tamanho_xy += 1;
	}
      }
    }
  }

  size_t n = rand() % tamanho_xy;
  size_t x = XY[n].x;
  size_t y = XY[n].y;
  EspacoTabuleiro* e = &tab->espacos[y][x];

  Dir dir = 1;
  Dir aux = e->podeJogarPara;
  size_t m = 1 + (rand() % TAMANHO_DIRECAO[e->podeJogarPara]);
  while (m > 0) {
    while ((aux & 1) == 0) {
      dir = dir << 1;
      aux = aux >> 1;
    }
    dir = dir << 1;
    aux = aux >> 1;
    m -= 1;
  }
  dir = dir >> 1;

  return (Jogada){e->coord, dir};
}

