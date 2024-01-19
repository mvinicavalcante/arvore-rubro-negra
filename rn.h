#ifndef RB_H
#define RB_H

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
void remover (int valor, arvore *raiz);

#endif
