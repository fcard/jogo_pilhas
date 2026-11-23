#include "cor.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#define WIN_COLOR_RESET 7

#define WIN_COLOR_RED     4
#define WIN_COLOR_GREEN   2
#define WIN_COLOR_BLUE    1
#define WIN_COLOR_YELLOW  6
#define WIN_COLOR_MAGENTA 5

#define WIN_COLOR_BG_RED     7+WIN_COLOR_RED*16
#define WIN_COLOR_BG_GREEN   7+WIN_COLOR_GREEN*16
#define WIN_COLOR_BG_BLUE    7+WIN_COLOR_BLUE*16
#define WIN_COLOR_BG_YELLOW  7+WIN_COLOR_YELLOW*16
#define WIN_COLOR_BG_MAGENTA 7+WIN_COLOR_MAGENTA*16

static void imprimir_para_arquivo_com_cor(FILE* file, const char* s, Cor cor) {
  int c;
  switch(cor) {
    case Cor_Vermelho: c = WIN_COLOR_RED; break;
    case Cor_Verde:    c = WIN_COLOR_GREEN; break;
    case Cor_Azul:     c = WIN_COLOR_BLUE; break;
    case Cor_Amarelo:  c = WIN_COLOR_YELLOW; break;
    case Cor_Roxo:     c = WIN_COLOR_MAGENTA; break;
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
  fprintf(file, "%s", s);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WIN_COLOR_RESET);
}

static void imprimir_para_arquivo_com_cor_de_fundo(FILE* file, const char* s, Cor cor) {
  int c;
  switch(cor) {
    case Cor_Vermelho: c = WIN_COLOR_BG_RED; break;
    case Cor_Verde:    c = WIN_COLOR_BG_GREEN; break;
    case Cor_Azul:     c = WIN_COLOR_BG_BLUE; break;
    case Cor_Amarelo:  c = WIN_COLOR_BG_YELLOW; break;
    case Cor_Roxo:     c = WIN_COLOR_BG_MAGENTA; break;
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
  fprintf(file, "%s", s);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WIN_COLOR_RESET);
}

#else
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"

#define ANSI_COLOR_BG_RED     "\x1b[41m"
#define ANSI_COLOR_BG_GREEN   "\x1b[42m"
#define ANSI_COLOR_BG_BLUE    "\x1b[44m"
#define ANSI_COLOR_BG_YELLOW  "\x1b[43m"
#define ANSI_COLOR_BG_MAGENTA "\x1b[45m"

#define ANSI_COLOR_RESET   "\x1b[0m"

static void imprimir_para_arquivo_com_cor(FILE* file, const char* s, Cor cor) {
  char* c;
  switch(cor) {
    case Cor_Vermelho: c = ANSI_COLOR_RED; break;
    case Cor_Verde:    c = ANSI_COLOR_GREEN; break;
    case Cor_Azul:     c = ANSI_COLOR_BLUE; break;
    case Cor_Amarelo:  c = ANSI_COLOR_YELLOW; break;
    case Cor_Roxo:     c = ANSI_COLOR_MAGENTA; break;
  }
  fprintf(file, "%s%s%s", c, s, ANSI_COLOR_RESET);
}

static void imprimir_para_arquivo_com_cor_de_fundo(FILE* file, const char* s, Cor cor) {
  char* c;
  switch(cor) {
    case Cor_Vermelho: c = ANSI_COLOR_BG_RED; break;
    case Cor_Verde:    c = ANSI_COLOR_BG_GREEN; break;
    case Cor_Azul:     c = ANSI_COLOR_BG_BLUE; break;
    case Cor_Amarelo:  c = ANSI_COLOR_BG_YELLOW; break;
    case Cor_Roxo:     c = ANSI_COLOR_BG_MAGENTA; break;
  }
  fprintf(file, "%s%s%s", c, s, ANSI_COLOR_RESET);
}

#endif

void imprimir_com_cor(const char* s, Cor cor) {
  imprimir_para_arquivo_com_cor(stdout, s, cor);
}

void imprimir_erro_com_cor(const char* s, Cor cor) {
  imprimir_para_arquivo_com_cor(stderr, s, cor);
}

void imprimir_com_cor_de_fundo(const char* s, Cor cor) {
  imprimir_para_arquivo_com_cor_de_fundo(stdout, s, cor);
}

static const size_t TAMANHO_COR[32] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
};


CorComIndice cor_aleatoria(Cor permitidas) {
  Cor aux = permitidas;
  Cor res = 1;
  size_t tamanho = TAMANHO_COR[permitidas];
  size_t numero_aleatorio = 1 + (rand() % tamanho);
  size_t indice = 0;
  while (numero_aleatorio > 0) {
    while ((aux & 1) == 0) {
      aux = aux >> 1;
      res = res << 1;
      indice += 1;
    }
    aux = aux >> 1;
    res = res << 1;
    indice += 1;
    numero_aleatorio -= 1;
  }
  return (CorComIndice){indice - 1, res >> 1};
}

