#ifndef JGP_ERRO
#define JGP_ERRO

typedef enum erro Erro;

enum erro {
  Successo,
  Erro_FaltaDeMemoria,
  Erro_PilhaVazia,
  Erro_PilhaNula,
  Erro_TamanhoAcimaDoLimite,
};

#define VALIDAR(ERR, INST)\
  INST;\
  if (ERR != Successo) {\
    imprimir_erro(ERR);\
    exit(200 + ERR);\
  }\

void imprimir_erro(Erro);

#endif

