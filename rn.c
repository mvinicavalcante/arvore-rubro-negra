#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

arvore no_null;

void inicializar(arvore *raiz) {
	*raiz = NULL;
	no_null = (arvore) malloc(sizeof(struct no_bst));
	no_null->cor = DUPLO_PRETO;
	no_null->dado = 0;
    no_null->esq = NULL;
    no_null->dir = NULL;
    no_null->pai = NULL;
}

arvore adicionar(int valor, arvore *raiz) {
    arvore posicao, pai, novo;

    posicao = *raiz;
    pai = NULL;

    while(posicao != NULL) {

    }
}

