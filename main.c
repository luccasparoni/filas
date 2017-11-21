#include "fila.h"
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "Normal.h"
#include <stdio.h>

int NUM_C = 4;
int NUM = 20;

void main(){
	int i;
	double t_chegada = 0, t_atendimento;
	BANCO* banco = criar_banco(NUM_C);
	printf("%d ", banco->filas[0]->tamanho);
	FILA* pre_fila = criar_fila();
	CLIENTE* cliente_aux;

	for(i = 0; i < NUM; i++){
		t_chegada = t_chegada + Normal(1, 0.3);
		t_atendimento = Normal(4, 0.3);
		cliente_aux = novo_cliente(t_chegada, t_atendimento);
		insere_cliente(pre_fila, cliente_aux);
		printf("tche: %.2f\n", t_chegada);
		printf("tati:%.2f\n", t_atendimento);
	}
	while(banco->atendidos < NUM-1){
		chama_prox_cl_caso_2(banco, pre_fila, NUM_C);
		atende_cliente(banco, NUM, NUM_C);
		printf("word taime <- %.2f\n",banco->tempo_global);
	}
	temp_medio(banco, NUM);
	double tempo_medio = banco->temp_med;
	printf("%f\n", temp_medio);
	double tempo_maximo = banco->temp_max;
	for(i = 0; i < NUM_C; i++){
	 	apagar_fila(&(banco->filas[i]));
	}
	apagar_banco(&banco);
}
