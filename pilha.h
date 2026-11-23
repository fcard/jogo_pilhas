#ifndef JGP_PILHA
#define JGP_PILHA

#include "cor.h"
#include "erro.h"
#include <stddef.h>

typedef struct pilha* Pilha;
typedef struct pilha  CelulaPilha;

Pilha* pilha_criar(Erro*);
bool pilha_vazia(Pilha*);
void pilha_inserir(Pilha*, CorComIndice, Erro*);
void pilha_unir(Pilha*, Pilha*, Erro*);
CorComIndice pilha_topo(Pilha*, Erro*);
size_t pilha_tamanho(Pilha*, Erro*);
void pilha_destruir(Pilha*, Erro*);
void pilha_limpar(Pilha*, Erro*);
void pilha_imprimir(Pilha*, const char*, Erro*);

#endif

