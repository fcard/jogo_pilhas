#include "pilha.h"
#include "util.h"
#include <stdlib.h>

struct pilha {
  CorComIndice cor;
  Pilha proximo;
};

Pilha* pilha_criar(Erro* err) {
  Pilha* pilha = malloc_validado(sizeof(Pilha), err);
  if (pilha != nullptr) {
    *pilha = nullptr;
  }
  return pilha;
}

bool pilha_vazia(Pilha* pilha) {
  return pilha == nullptr || *pilha == nullptr;
}

void pilha_inserir(Pilha* pilha, CorComIndice cor, Erro* err) {
  if (pilha == nullptr) {
    *err = Erro_PilhaNula;
  } else {
    Pilha nova_celula = malloc_validado(sizeof(CelulaPilha), err);
    if (nova_celula != nullptr)  {
      nova_celula->cor = cor;
      nova_celula->proximo = *pilha;
      *pilha = nova_celula;
    }
  }
}

static void pilha_passar_inversa(Pilha* pa, Pilha* pb, Erro* err) {
  while (*pb != nullptr) {
    Pilha cel = *pb;
    *pb = cel->proximo;
    cel->proximo = *pa;
    *pa = cel;
  }
}

void pilha_unir(Pilha* pa, Pilha* pb, Erro* err) {
  if (pa == nullptr || pb == nullptr) {
    *err = Erro_PilhaNula;
  } else {
    Pilha* p = pilha_criar(err);
    if (p != nullptr) {
      pilha_passar_inversa(p, pb, err);
      pilha_passar_inversa(p, pa, err);
      pilha_passar_inversa(pa, p, err);
      free(p);
    }
  }
}

CorComIndice pilha_topo(Pilha* pilha, Erro* err) {
  if (pilha == nullptr) {
    *err = Erro_PilhaNula;
    return (CorComIndice){-1, Cor_Erro};
  } else if (*pilha == nullptr) {
    *err = Erro_PilhaVazia;
    return (CorComIndice){-1, Cor_Erro};
  } else {
    return (*pilha)->cor;
  }
}

size_t pilha_tamanho(Pilha* pilha, Erro* err) {
  if (pilha == nullptr) {
    *err = Erro_PilhaNula;
    return 0;
  } else { 
    size_t tamanho = 0;
    while (*pilha != nullptr) {
      pilha = &(*pilha)->proximo;
      tamanho += 1;
    }
    return tamanho;
  }
}

void pilha_destruir(Pilha* pilha, Erro* err) {
  pilha_limpar(pilha, err);
  free(pilha);
}

void pilha_limpar(Pilha* pilha, Erro* err) {
  if (pilha == nullptr) {
    *err = Erro_PilhaNula;
  } else { 
    Pilha aux;
    while (*pilha != nullptr) {
      aux = (*pilha)->proximo;
      free(*pilha);
      *pilha = aux;
    }
  }
}

void pilha_imprimir(Pilha* pilha, const char* fg, Erro* err) {
  if (pilha == nullptr) {
    *err = Erro_PilhaNula;
  } else {
    Pilha aux = *pilha;
    while (aux != nullptr) {
      imprimir_com_cor_de_fundo(fg, aux->cor.cor);
      aux = aux->proximo;
    }
  }
}

