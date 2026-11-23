#include "tabuleiro.h"
#include "erro.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void flush_stdin(bool* p) {
  if (*p) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    *p = false;
  }
}

static size_t ler_inteiro(const char* desc, size_t min, size_t max) {
  size_t r;
  printf("%s; Digite um numero :> ", desc);
  
  if (scanf(" %zu", &r) == 1 && (r >= min && r <= max)) {
    return r;
  } else {
    printf("Valor invalido!\n\n");
    return ler_inteiro(desc, min, max);
  }
}

static Coordenadas ler_coordenadas(const char* desc, size_t min, size_t max, size_t* linhasPuladas) {
  size_t rx, ry;
  printf("%s; Digite coordenadas :> ", desc);
 
  if (scanf(" %zu %zu", &rx, &ry) == 2 && (rx >= min && rx <= max) && (ry >= min && ry <= max)) {
    return COORD(rx,ry);
  } else {
    *linhasPuladas += 2;
    printf("Valores invalidos!\n\n");
    return ler_coordenadas(desc, min, max, linhasPuladas);
  }
}

static bool endOfStr(char p) {
  return p == '\0' || p == '\n';
}

static Dir ler_direcao(const char* desc, size_t* linhasPuladas, bool* p) {
  char d[4];
  printf("%s; Digite uma direcao :> ", desc);

  flush_stdin(p);
  fgets(d, 3, stdin);
  *p = d[1] != '\n' && d[2] != '\n' && d[3] != '\n';

  #define FLUSH_AND_RETURN(X) flush_stdin(p); return X

  if (d[0] == 'N') {
    if (endOfStr(d[1])) {
      FLUSH_AND_RETURN(Dir_Norte);
    } else if (d[1] == 'E' && endOfStr(d[2])) {
      FLUSH_AND_RETURN(Dir_Nordeste);
    } else if ((d[1] == 'O' || d[1] == 'W') && endOfStr(d[2])) {
      FLUSH_AND_RETURN(Dir_Noroeste);
    }
  } else if (d[0] == 'S') {
    if (endOfStr(d[1])) {
      FLUSH_AND_RETURN(Dir_Sul);
    } else if (d[1] == 'E' && endOfStr(d[2])) {
      FLUSH_AND_RETURN(Dir_Sudeste);
    } else if ((d[1] == 'O' || d[1] == 'W') && endOfStr(d[2])) {
      FLUSH_AND_RETURN(Dir_Sudoeste);
    }
  } else if ((d[0] == 'E' || d[0] == 'L') && endOfStr(d[1])) {
    FLUSH_AND_RETURN(Dir_Leste);
  } else if ((d[0] == 'W' || d[0] == 'O') && endOfStr(d[1])) {
    FLUSH_AND_RETURN(Dir_Oeste);
  }

  *linhasPuladas += 2;
  printf("Valor invalido!\n\n");
  return ler_direcao(desc, linhasPuladas, p);
}

int main() {
  srand(time(nullptr));
  Erro err = Successo;

  size_t linhasPuladas = 0;

  size_t numJogadores = ler_inteiro("Quantidade de Jogadores (2=>5)", 1, 5);
  size_t tamanhoTabuleiro = ler_inteiro("Tamanho do Tabuleiro (5=>10)", 5, 10);

  VALIDAR(err, auto tab = tabuleiro_criar(tamanhoTabuleiro, numJogadores, &err));

  bool fimDeJogo = false;
  while (!fimDeJogo) {
    printf("\n");
    fimDeJogo = true;
    for (size_t jg = 0; jg < numJogadores; jg++) {
      if (tabuleiro_pode_jogar(tab, jg)) {
	Cor corDoJogador = Cor_Vermelho << jg;
        VALIDAR(err, tabuleiro_mostrar(tab, &err));
	printf("Turno do jogador [");
	imprimir_com_cor_de_fundo("  ", corDoJogador);
	printf("]\n");
        fimDeJogo = false;
	bool fimDaJogada;
	do {
          Coordenadas de = ler_coordenadas("Posicao da jogada", 0, tamanhoTabuleiro, &linhasPuladas); 
	  if (tabuleiro_espaco_pertence_a(tab, de, jg, &err)) {
	    bool px = true;
	    Dir para = ler_direcao("Direcao da jogada", &linhasPuladas, &px);
	    VALIDAR(err, fimDaJogada = tabuleiro_mover(tab, de, para, &err));
	    if (!fimDaJogada) {
	      printf("Jogada invalida!\n\n");
	      if (linhasPuladas > 32) {
	        linhasPuladas = 0;
                VALIDAR(err, tabuleiro_mostrar(tab, &err));
	        printf("Turno do jogador [");
	        imprimir_com_cor_de_fundo("  ", corDoJogador);
	        printf("]\n");
	      }
	    }
	  } else {
	    linhasPuladas += 2;
	    printf("Espaco nao pertence ao jogador!\n\n");
	    fimDaJogada = false;
	  }
	} while (!fimDaJogada);
      }
    }
  }

  VALIDAR(err, Pilha* vencedores = pilha_criar(&err));
  size_t pontuacao;
  size_t pontuacaoVencedor = 0;

  VALIDAR(err, Pilha** resultado = tabuleiro_finalizar_resultado(tab, &err));
  for (size_t jg = 0; jg < numJogadores; jg++) {
    VALIDAR(err, pontuacao = pilha_tamanho(resultado[jg], &err));
    VALIDAR(err, pilha_imprimir(resultado[jg], " ", &err));
    printf(": %zu\n", pontuacao);
    if (pontuacao > pontuacaoVencedor) {
      pontuacaoVencedor = pontuacao;
      VALIDAR(err, pilha_limpar(vencedores, &err));
      VALIDAR(err, pilha_inserir(vencedores, pilha_topo(resultado[jg], &err), &err));
    } else if (pontuacao > 0 && pontuacao == pontuacaoVencedor) {
      VALIDAR(err, pilha_inserir(vencedores, pilha_topo(resultado[jg], &err), &err));
    }
  }
  if (pilha_tamanho(vencedores, &err) == 1) {
    printf("Vencedor: [");
  } else {
    printf("Empate: [");
  }
  VALIDAR(err, pilha_imprimir(vencedores, "  ", &err));
  printf("]\n");
  return 0;
}

