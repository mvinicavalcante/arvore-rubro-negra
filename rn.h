#ifndef RN_H
#define RN_H

enum cor { VERMELHO, PRETO, DUPLO_PRETO };

typedef int tipo_dado;

typedef struct no_bst {
    tipo_dado dado;
    enum cor cor;
    struct no_bst *esq, *dir, *pai;
} no_bst;

typedef no_bst * arvore;

void inicializar(arvore *raiz);
void adicionar(int valor, arvore *raiz);
void remover(int valor, arvore *raiz);

enum cor cor(arvore elemento);
int eh_raiz(arvore elemento);
int eh_filho_esquerdo(arvore elemento);
int eh_filho_dir(arvore elemento);
arvore irmao(arvore elemento);
arvore tio(arvore elemento);

void rotacao_simples_direita(arvore *raiz, arvore pivo);
void rotacao_simples_esquerda(arvore *raiz, arvore pivo);

void ajustar(arvore *raiz, arvore elemento);

void pre_order(arvore raiz);
void in_order(arvore raiz);
void pos_order(arvore raiz);

#endif
