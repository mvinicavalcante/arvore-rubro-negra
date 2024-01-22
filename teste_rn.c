#include <stdio.h>
#include <stdlib.h>
#include "rn.h"

int main(int argc, char * argv[]) {
	arvore a;
	int opcao;
	inicializar(&a);

	while(1) {
        printf("Selecione uma opcao: \n1- Adicionar \n2 - Pre-order \n3 - In-order \n4- Pos-order \n5- Remover \n");
		scanf("%d", &opcao);

		switch(opcao) {
				int valor;
				case 1:
						scanf("%d", &valor);
						adicionar(valor, &a);
						break;
				case 2:
						pre_order(a);
						printf("\n");
						break;
				case 3:
						in_order(a);
						printf("\n");
						break;
				case 4:
						pos_order(a);
						printf("\n");
						break;
				case 5:
						scanf("%d", &valor);
						remover(valor, &a);
						break;

				case 99:
						exit(0);
		}
	}
}
