#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "rn.h"

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

void pre_order(arvore raiz) {
    if(raiz != NULL) {
        imprimir_elemento(raiz);
        pre_order(raiz->esq);
        pre_order(raiz->dir);
    }
}

void in_order(arvore raiz) {
    if(raiz != NULL) {
        in_order(raiz->esq);
        printf("[%d]", raiz->dado);
        in_order(raiz->dir);
    }
}

void pos_order(arvore raiz) {
    if(raiz != NULL) {
        pos_order(raiz->esq);
        pos_order(raiz->dir);
        printf("[%d]", raiz->dado);
    }
}

enum cor cor(arvore elemento) {
	enum cor c;
	if(elemento == NULL) {
        return PRETO;
	}
	else {
        return elemento->cor;
	}
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
    arvore u, t2;
    u = pivo->esq;
    t2 = u->dir;

    int posicao_pivo_esq = eh_filho_esquerdo(pivo);

    u->dir = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;
    pivo->esq = t2;
    if(t2 != NULL)
        t2->pai = pivo;

    if(eh_raiz(u))
        *raiz = u;
    else {
        if(posicao_pivo_esq)
            u->pai->esq = u;
        else
            u->pai->dir = u;
    }
}

void rotacao_simples_esquerda(arvore *raiz, arvore pivo) {
    arvore u, t2;
    u = pivo->dir;
    t2 = u->esq;

    int posicao_pivo_esq = eh_filho_esquerdo(pivo);

    u->esq = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;
    pivo->dir = t2;
    if(t2 != NULL)
        t2->pai = pivo;

    if(eh_raiz(u))
        *raiz = u;
    else {
        if(posicao_pivo_esq)
            u->pai->esq = u;
        else
            u->pai->dir = u;
    }
}

void ajustar(arvore *raiz, arvore elemento) {
    while(cor(elemento->pai) == VERMELHO && cor(elemento) == VERMELHO) {
        //caso 1: tio vermelho
        if(cor(tio(elemento)) == VERMELHO) {
            tio(elemento)->cor = PRETO;
            elemento->pai->cor = PRETO;
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
        if(!eh_filho_esquerdo(elemento) && !eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_esquerda(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            elemento->pai->esq->cor = VERMELHO; //tio vermelho
            continue;
        }
        //rotação dupla esquerda
        if(eh_filho_esquerdo(elemento) && !eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_direita(raiz, elemento->pai);
            rotacao_simples_esquerda(raiz, elemento->pai);
            elemento->cor = PRETO;
            elemento->esq->cor = VERMELHO;
            continue;
        }
        if(!eh_filho_esquerdo(elemento) && eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_esquerda(raiz, elemento->pai);
            rotacao_simples_direita(raiz, elemento->pai);
            elemento->cor = PRETO;
            elemento->dir->cor = VERMELHO;
            continue;
        }

    }
    (*raiz)->cor = PRETO;
}

void adicionar(int valor, arvore *raiz) {
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

void remover(int valor, arvore *raiz) {
    printf("remove function");
}

void imprimir_elemento(arvore raiz) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    switch(raiz->cor) {
        case PRETO:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE); // Preto
            printf("[%d]", raiz->dado);
            break;
        case VERMELHO:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Vermelho
            printf("[%d]", raiz->dado);
            break;
        case DUPLO_PRETO:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // Verde
            printf("[%d]", raiz->dado);
            break;
    }

    SetConsoleTextAttribute(hConsole, 7); // Retorna à cor padrão
}


void remover(int valor, arvore *raiz) {
    arvore posicao;
    posicao = *raiz;

    while(posicao != NULL) {
        if(valor == posicao->dado) {
            if(posicao->esq != NULL && posicao->dir != NULL) {

            }
        }
    }
}

