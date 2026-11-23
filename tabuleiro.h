#ifndef JGP_TABULEIRO
#define JGP_TABULEIRO

#include "cor.h"
#include "pilha.h"
#include "erro.h"
#include <stddef.h>

typedef struct tabuleiro Tabuleiro;
typedef struct coordenadas Coordenadas;
typedef struct jogador Jogador;
typedef struct espacoTabuleiro EspacoTabuleiro;
typedef enum dir Dir;

enum dir {
  Dir_Sul = 1,
  Dir_Norte = 2,
  Dir_Leste = 4,
  Dir_Oeste = 8,
  Dir_Nordeste = 16,
  Dir_Noroeste = 32,
  Dir_Sudeste = 64,
  Dir_Sudoeste = 128 
};

#define COORD(X,Y) (Coordenadas){X,Y}

struct coordenadas {
  size_t x;
  size_t y;
};

Tabuleiro* tabuleiro_criar(size_t, size_t, Erro*);
void tabuleiro_mostrar(Tabuleiro*, Erro*);
bool tabuleiro_mover(Tabuleiro*, Coordenadas, Dir, Erro*);
bool tabuleiro_espaco_pertence_a(Tabuleiro*, Coordenadas, size_t, Erro*);
bool tabuleiro_pode_jogar(Tabuleiro*, size_t);
bool tabuleiro_checar_vitoria(Tabuleiro*, Cor*);
Pilha** tabuleiro_finalizar_resultado(Tabuleiro*, Erro*);

#endif
