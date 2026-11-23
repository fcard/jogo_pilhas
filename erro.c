#include "cor.h"
#include "erro.h"
#include <stdio.h>

void imprimir_erro(Erro erro) {
  char* msg;
  switch (erro) {
    case Erro_FaltaDeMemoria: msg = "Memoria insuficiente."; break;
    case Erro_PilhaVazia: msg = "Pilha vazia."; break;
    case Erro_PilhaNula: msg = "Pilha nula."; break;
    case Erro_TamanhoAcimaDoLimite: msg = "Tamanho acima do limite."; break;
  }
  imprimir_erro_com_cor(msg, Cor_Vermelho);
}

