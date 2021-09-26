#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct no_st{
	char command[100];
	struct no_st* prox;
};
typedef struct no_st no;

struct filacc_st{
	no* cabeca;
	no* fim;
	int tam_histoico;
};
typedef struct filacc_st filacc;

filacc* cria_filacc(){
	filacc* f = (filacc*) malloc(sizeof(filacc));
	
	f->cabeca = f->fim = (no*) malloc(sizeof(no));
	
	f->cabeca->prox = NULL;

	f->tam_histoico = 0;
	
	return f;
}

void desenfileira(filacc* f){
	no* p;
	
	p = f->cabeca->prox;
	
	if (p){
		
		f->cabeca->prox = p->prox;
		
		free(p);
		
		if (!f->cabeca->prox)// if (!f->fim) ou if (f->cabeca->prox == NULL)
			f->fim = f->cabeca;
	}
	else
		printf("Fila vazia!\n");
		
}


void enfileira(filacc* f, char *command){
	no* novo;
	
	novo = (no*) malloc(sizeof(no));
	
	strcpy(novo->command, command);
	novo->prox = NULL;
	
	f->fim->prox = novo;
	
	f->fim = novo;

	f->tam_histoico++;

	if(f->tam_histoico > 50)
		desenfileira(f);
}

void imprime_fila(filacc* f){
	no* ptr;
	int i = 1;
	if (!f->cabeca->prox)
		printf("Fila vazia!");
	else
		for (ptr = f->cabeca->prox; ptr != NULL; ptr = ptr->prox)
			printf("%2d. %s\n", i++, ptr->command);
			
	
}
