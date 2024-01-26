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
    arvore u, t1;
    u = pivo->esq;
    t1 = u->dir;

    int posicao_pivo_esq = eh_filho_esquerdo(pivo);

    //atualização dos ponteiros
    pivo->esq = t1;

    if(t1 != NULL)
        t1->pai = pivo;

    u->dir = pivo;

    u->pai = pivo->pai;
    pivo->pai = u;

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
    arvore u, t1;
    u = pivo->dir;
    t1 = u->esq;

    int posicao_pivo_esq = eh_filho_esquerdo(pivo);

    u->esq = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;
    pivo->dir = t1;
    if(t1 != NULL)
        t1->pai = pivo;

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

int maior_elemento(arvore raiz) {
	if(raiz == NULL)
        return -1;
	if(raiz->dir == NULL)
        return raiz->dado;
	else
        return maior_elemento(raiz->dir);
}

void remover(int valor, arvore *raiz) {
    arvore posicao;
    posicao = *raiz;

    while(posicao != NULL) {
        if(valor == posicao->dado) {
            if(posicao->esq != NULL && posicao->dir != NULL) {
                //caso tenha dois filhos, a posição atual será igual ao maior elemento da esquerda e, obviamente, será excluída a anterior
                posicao->dado = maior_elemento(posicao->esq);
                //manda remover o menor valor da esquerda
                remover(posicao->dado, &(posicao->esq));
                break;
            }

            //apenas filho direito
            if(posicao->esq == NULL && posicao->dir != NULL) {
                //O seu filho direito sobe para a posição do elemento  a ser removido e recebe a cor preta
				posicao->dir->cor = PRETO;
                posicao->dir->pai = posicao->pai;

				if(eh_raiz(posicao)) {
					*raiz = posicao->dir;
				} else {
					if(eh_filho_esquerdo(posicao)) {
    					posicao->pai->esq = posicao->dir;
					} else {
						posicao->pai->dir = posicao->dir;
    				}
				}
                free(posicao);
				break;
			}
			//Apenas um filho esquerdo
			if(posicao->esq != NULL  && posicao->dir == NULL) {
                posicao->esq->cor = PRETO;
                posicao->esq->pai = posicao->pai;

                if(eh_raiz(posicao)) {
                    *raiz = posicao->esq;
                } else {
                    if(eh_filho_esquerdo(posicao)) {
                        posicao->pai->esq = posicao->esq;
                    } else {
                        posicao->pai->dir = posicao->esq;
                    }
                }
                free(posicao);
				break;
			}
			if(posicao->esq == NULL && posicao->dir == NULL) {
				//Remover raiz sem filhos
				if(eh_raiz(posicao)) {
					*raiz = NULL;
                    free(posicao);
					break;
                }

                //Remover elemento que não possui filhos e não é raiz
				//Se for vermelho, apenas remove atualizando o ponteiro
				//correspondente do pai
                if(posicao->cor == VERMELHO) {
					if(eh_filho_esquerdo(posicao))
						posicao->pai->esq = NULL;
					else
						posicao->pai->dir = NULL;
                    free(posicao);
					break;
				} else {
                    //Se o elemento for preto, substitui pelo duplo preto e depois ajusta a árvore
                    no_null->cor = DUPLO_PRETO;
                    no_null->pai = posicao->pai;

                    if(eh_filho_esquerdo(posicao))
                        posicao->pai->esq = no_null;
                    else
                        posicao->pai->dir = no_null;

                    free(posicao);
                    reajustar(raiz, no_null);
                    break;
				}
            }
            if(valor > posicao->dado) {
                posicao = posicao->dir;
            } else {
                posicao = posicao->esq;
            }
        }
    }
}

/*Realiza a correção da árvore após a remoção de um elemento preto que não possui filhos, ou seja, elimina o nó null o duplo-preto.*/
void reajustar(arvore *raiz, arvore elemento) {
    //caso 1: elemento é raiz
    if(eh_raiz(elemento)) {
		elemento->cor = PRETO;
        if(elemento == no_null) {
            *raiz = NULL;
		}
		return;
	}

	//caso 2
	if(cor(elemento->pai) == PRETO &&
        cor(irmao(elemento)) == VERMELHO &&
        cor(irmao(elemento)->dir) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO) {
            if(eh_filho_esquerdo(elemento))
                rotacao_simples_esquerda(raiz, elemento->pai);
            else
                rotacao_simples_direita(raiz, elemento->pai);

            elemento->pai->pai->cor = PRETO;
            elemento->pai->cor = VERMELHO;

            //Atenção à chamada recursiva do reajustar.
            //O caso 2 não remove o duplo-preto
            reajustar(raiz, elemento);
            return;
    }

    //caso 3
    if(cor(elemento->pai) == PRETO &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO) {

            irmao(elemento)->cor = VERMELHO;
            elemento->pai->cor = DUPLO_PRETO;
            retira_duplo_preto(raiz, elemento);

            reajustar(raiz, elemento->pai);
            return;

        }

    //caso 4
    if(cor(elemento->pai) == VERMELHO &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO) {
        retira_duplo_preto(raiz, elemento);
        elemento->pai->cor = PRETO;
        irmao(elemento)->cor = VERMELHO;
        return;
    }

    //caso 5a
	if(cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == VERMELHO && cor(irmao(elemento)->esq) == PRETO )  {
        rotacao_simples_esquerda(raiz, irmao(elemento));
        irmao(elemento)->cor = PRETO;
        irmao(elemento)->dir->cor = VERMELHO;
        return;
	}

	//caso 5b
	if(cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == VERMELHO && cor(irmao(elemento)->dir) == PRETO )  {
        rotacao_simples_esquerda(raiz, irmao(elemento));
        irmao(elemento)->cor = PRETO;
        irmao(elemento)->esq->cor = VERMELHO;
        return;
	}

	//caso 6a
	if(cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == VERMELHO) {
        retira_duplo_preto(raiz, elemento);
        rotacao_simples_esquerda(raiz, irmao(elemento));
        tio(elemento)->cor = PRETO;
        return;
	}

	//caso 6b
	if(cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == VERMELHO) {
        retira_duplo_preto(raiz, elemento);
        rotacao_simples_esquerda(raiz, irmao(elemento));
        tio(elemento)->cor = PRETO;
        return;
	}

}

void retira_duplo_preto(arvore *raiz, arvore elemento) {
	if(elemento == no_null)
		if(eh_filho_esquerdo(elemento))
			elemento->pai->esq = NULL;
		else
			elemento->pai->dir = NULL;
    else
		elemento->cor = PRETO;
}

