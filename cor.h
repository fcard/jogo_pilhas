#ifndef JGP_COR
#define JGP_COR

#include <stddef.h>

typedef enum cor Cor;
typedef struct corComIndice CorComIndice;

enum cor {
  Cor_Vermelho = 1,
  Cor_Verde = 2,
  Cor_Azul = 4,
  Cor_Amarelo = 8,
  Cor_Roxo = 16,
  Cor_Erro = -1,
  Cor_Nada = 0,
};

struct corComIndice {
  size_t indice;
  Cor cor;
};

void imprimir_com_cor(const char*, Cor);
void imprimir_com_cor_de_fundo(const char*, Cor);
void imprimir_erro_com_cor(const char*, Cor);
CorComIndice  cor_aleatoria(Cor);

#endif
