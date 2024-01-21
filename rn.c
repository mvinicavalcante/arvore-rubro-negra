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

enum cor cor(arvore elemento) {
	enum cor c;
	if(elemento==NULL)
		return PRETO;
	else
		return elemento->cor;
	return c;
}

int eh_raiz(arvore elemento) {
    return (elemento->pai == NULL);
}

int eh_filho_esquerdo(arvore elemento) {
	return (elemento->pai != NULL && elemento == elemento->pai->esq);
}

arvore irmao(arvore elemento) {
	if(eh_filho_esquerdo(elemento))
		return elemento->pai->dir;
	else
		return elemento->pai->esq;
}

arvore tio(arvore elemento) {
	return irmao(elemento->pai);
}

void rotacao_simples_direita(arvore *raiz, arvore pivo) {
    arvore u, t1;

    u = pivo->esq;
    t1 = u->dir;

    //verificar se é filho esquerdo ou direito para para ligar a raiz resultante com o pai
    int posicao_pivo_esq = eh_filho_esquerdo(pivo);
    pivo->esq = t1;

    if(t1 != NULL) {
        t1->pai = pivo;

        u->dir = pivo;
        u->pai = pivo->pai;
        pivo->pai = u;

        //Se é raiz, colocar u como raiz;
        if(eh_raiz(u)) {
            *raiz = u;
        } else {
        //Caso contrário (se existir) é preciso ligar o restante da árvore a esta sub-árvore, resultante da rotação
            if(posicao_pivo_esq) {
                u->pai->esq = u;
            } else {
                u->pai->dir = u;
            }
        }
    }
}

void ajustar(arvore *raiz, arvore elemento) {
    while(cor(elemento->pai) == VERMELHO && cor(elemento) == VERMELHO) {
        //caso 1: tio vermelho
        if(cor(tio(elemento)) == VERMELHO) {
            tio(elemento->cor) = PRETO;
            elemento->cor = PRETO;
            elemento->pai->pai->cor = VERMELHO;
            elemento = elemento->pai->pai;
            //Continua a verificação a partir do avô, que mudou para vermelho e pode ter
            //gerado uma sequência vermelho-vermelho
            continue;
        }
        if(eh_filho_esquerdo(elemento) && eh_filho_esquerdo(elemento->pai)) {
            //aqui ele e o pai são vermelhos, alem de ambos serem filhos esquerdos
            rotacao_simples_direita(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            elemento->pai->dir->cor = VERMELHO;
            continue;
        }
    }
}

arvore adicionar(int valor, arvore *raiz) {
    arvore posicao, pai, novo;

    posicao = *raiz;
    pai = NULL;

    while(posicao != NULL) {
        pai = posicao;
        if(valor > posicao->dado) {
            posicao = posicao->dir;
        } else {
            posicao = posicao->esq;
        }
    }

    //após achar a posição
    //insere o novo nó com a cor vermelha
    //o pai é a última posição antes de ser null
    novo = (arvore) malloc(sizeof(struct no_bst));
	novo->dado = valor;
	novo->esq = NULL;
	novo->dir = NULL;
	novo->pai = pai;
	novo->cor = VERMELHO;

	if(eh_raiz(novo)) {
        *raiz = novo;
	} else {
        if(valor > pai->dado) {
            pai->dir = novo;
        } else {
            pai->esq = novo;
        }
	}

	ajustar(raiz, novo);
}

