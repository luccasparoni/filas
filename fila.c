#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

BANCO* criar_banco(int NUM_C){
  BANCO* banco = (BANCO*) malloc(sizeof(BANCO));
  banco->filas =(FILA**) malloc(sizeof(FILA*) * NUM_C);
  banco->caixas =(CLIENTE**) malloc(sizeof(CLIENTE*) * NUM_C);
  banco->temp_med = 0;
  banco->temp_max = 0;
  int i;
  for(i=0; i<NUM_C;i++){
    banco->filas[i] = criar_fila(NUM_C);
  }
  for(i=0; i<NUM_C;i++){
      banco->caixas[i] = NULL;
  }
  return banco;
}

FILA *criar_fila() {
  FILA *fila = (FILA *) malloc(sizeof(FILA));

  if (fila != NULL) {
    fila->primeiro_c = NULL;
    fila->ultimo_c = NULL;
    fila->tamanho = 0;
  }

  return fila;
}

int apagar_fila(FILA** fila) {
  if (fila != NULL && *fila != NULL) {
      CLIENTE *prem = (*fila)->primeiro_c;

     while(prem != NULL) {
        CLIENTE *pprox = prem->prox_c;
        apagar_cliente(&prem);
        prem = pprox;
     }

     free(*fila);
     *fila=NULL;
     return 1;
  }
  return 0;
}

int apagar_cliente(CLIENTE** cliente){
    if(cliente != NULL && *cliente != NULL){
        free(*cliente);
        *cliente =  NULL;
        return 1;
    }
    return 0;
}

int apagar_banco(BANCO** banco){
    if(banco != NULL && *banco != NULL){
        free(*banco);
        *banco =  NULL;
        return 1;
    }
    return 0;
}


CLIENTE* novo_cliente(double t_chegada, double t_atendimento){
  CLIENTE* cliente = (CLIENTE *)malloc(sizeof(CLIENTE));
  cliente->t_chegada = t_chegada;
  cliente->t_atendimento = t_atendimento;
  cliente->t_espera = 0;
  cliente->t_em_fila = 0;
  cliente->prox_c = NULL;
  return cliente;
}


//AQUI ACABAM AS FUNÇOES DO CASO 1 - E INICIAM-SE AS FUNÇÕES DO CASO 2. NO CASO ERRAMOS quase TUDO ACIMA;

void insere_cliente(FILA* fila, CLIENTE* cliente){
    if(fila->tamanho == 0){
        fila->primeiro_c = cliente;
        fila->ultimo_c = cliente;
        fila->tamanho = 1;
        return;
    }
    fila->ultimo_c->prox_c = cliente;//ultimmo cliente aponta para o novo cliente
    fila->ultimo_c = cliente;//o novo cliente é adicionado
    fila->tamanho++;//aumenta o tamanho da fila
}

void entrar_na_fila(BANCO* banco, FILA* fila, int NUM_C){
    int i, j, k;
    double menor_tempo;
    for(i = 0; i < NUM_C; i++){
        if((banco->caixas[i] == NULL || banco->caixas[i]->t_atendimento == 0) && banco->filas[i]->tamanho == 0){
            fila->primeiro_c->t_chegada = menor_tempo;
            for(j = 0; j < NUM_C; j++){
                if(banco->caixas[j] != NULL){
                    if(banco->caixas[j]->t_atendimento + banco->tempo_global < menor_tempo){
                        menor_tempo = banco->caixas[j]->t_atendimento + banco->tempo_global;
                    }
                }
            }
            int aux = banco->tempo_global;
            banco->tempo_global = menor_tempo;
            for(j=0; j<NUM_C;j++){
                if(banco->caixas[j] != NULL){
                    banco->caixas[j]->t_atendimento = menor_tempo - aux;
                    if(banco->caixas[j]->t_atendimento < 0){
                        banco->caixas[j]->t_atendimento = 0;
                    }
                }
            }
        }
    }
    while (banco->tempo_global >= fila->primeiro_c->t_chegada) {
        double menor_tamanho = 10000;
        for(i = 0; i < NUM_C; i++){
            if(banco->filas[i]->tamanho < menor_tamanho){
                menor_tamanho = banco->filas[i]->tamanho;
                k = i;
            }
        }
        banco->filas[k]->ultimo_c = fila->primeiro_c;
        banco->filas[k]->tamanho++;
        fila->primeiro_c = fila->primeiro_c->prox_c;

    }
}

void chama_prox_cl_caso_1(BANCO* banco, int NUM_C){
    int i;
    CLIENTE* remove_cliente;
    for(i = 0; i < NUM_C; i++){
        if(banco->caixas[i] == NULL && banco->filas[i]->tamanho > 0){
            banco->caixas[i] = banco->filas[i]->primeiro_c;
            banco->caixas[i]->prox_c = NULL;
            banco->filas[i]->primeiro_c = banco->filas[i]->primeiro_c->prox_c;
            banco->filas[i]->tamanho--;

        }
        else if(banco->caixas[i]->t_atendimento == 0 && banco->filas[i]->tamanho > 0){
            remove_cliente = banco->caixas[i];
            banco->caixas[i] = banco->filas[i]->primeiro_c;
            banco->caixas[i]->prox_c = NULL;
            apagar_cliente(&(remove_cliente));
            banco->filas[i]->primeiro_c = banco->filas[i]->primeiro_c->prox_c;
            banco->filas[i]->tamanho--;
        }
        banco->caixas[i]->t_espera = banco->tempo_global - banco->caixas[i]->t_chegada;
        banco->t_espera_total+= banco->caixas[i]->t_espera;
        if(banco->caixas[i]->t_espera > banco->temp_max)
            banco->temp_max = banco->caixas[i]->t_espera;
    }
}

void chama_prox_cl_caso_2(BANCO* banco, FILA* fila, int NUM_C){
    int i;
    CLIENTE* remove_cliente;
    for(i = 0; i < NUM_C; i++){
        if(banco->caixas[i] == NULL){
            banco->caixas[i] = fila->primeiro_c;
            fila->primeiro_c = fila->primeiro_c->prox_c;
        }
        else if(banco->caixas[i]->t_atendimento == 0){
            remove_cliente = banco->caixas[i];
            banco->caixas[i] = fila->primeiro_c;
            apagar_cliente(&(remove_cliente));
            if(fila->primeiro_c->prox_c != NULL)
                fila->primeiro_c = fila->primeiro_c->prox_c;
        }
        banco->caixas[i]->t_espera = banco->tempo_global - banco->caixas[i]->t_chegada;
        banco->t_espera_total+= banco->caixas[i]->t_espera;
        if(banco->caixas[i]->t_espera > banco->temp_max)
            banco->temp_max = banco->caixas[i]->t_espera;
    }
}

void atende_cliente(BANCO* banco, int NUM, int NUM_C){
    int i;
    double menor_tempo = 10000;//(media mais sigma)
    for(i = 0; i < NUM_C; i++){
        if(banco->caixas[i] != NULL){
            if(banco->caixas[i]->t_atendimento < menor_tempo){
                menor_tempo = banco->caixas[i]->t_atendimento;
                if(banco->caixas[i]->t_atendimento < 0){
                    banco->caixas[i]->t_atendimento = 0;
                }
            }
        }
        else{
            menor_tempo = 0;
        }
    }
    // if(menor_tempo < 0){
    //     menor_tempo = 0;
    // }
    banco->tempo_global = banco->tempo_global + menor_tempo;
    for(i = 0; i < NUM_C; i++){
        if(banco->caixas[i] != NULL){
            banco->caixas[i]->t_atendimento = banco->caixas[i]->t_atendimento - menor_tempo;
        }
    }
}

void  temp_medio(BANCO* banco, int NUM){
    banco->temp_med =  banco->t_espera_total/NUM;
}






//aqui comecam as funçoes unicas para o caso 1
//double inserir_cliente_caso1(BANCO* banco, CLIENTE* cliente, double t_espera_max, int NUM_C){//sempre deve receber como um dos parametros o cliente adocionado anteriormente;
    //   int i, menor_tamanho = 10000, aux;
    //   for(i = 0; i < NUM_C; i++){
    //       if(banco->filas[i]->tamanho < menor_tamanho){
    //           menor_tamanho = banco->filas[i]->tamanho;
    //           aux = i;
    //       }
    //   }
    //   if(banco->filas[aux]->tamanho == 0){
    //     banco->filas[aux]->ultimo_c = cliente;
    //     banco->t_espera_total = cliente->t_espera;
    //     banco->filas[aux]->tamanho++;
    //     return cliente->t_espera;
    //
    //   }
    //   if(banco->filas[aux]->ultimo_c->t_espera + banco->filas[aux]->ultimo_c->t_atendimento - (cliente->t_chegada - banco->filas[aux]->ultimo_c->t_chegada) >= 0){
    //   cliente->t_espera = banco->filas[aux]->ultimo_c->t_espera + banco->filas[aux]->ultimo_c->t_atendimento - (cliente->t_chegada - banco->filas[aux]->ultimo_c->t_chegada);
    //   }else{
    //         cliente->t_espera = 0;
    //   }
    //    banco->filas[aux]->ultimo_c->prox_c = cliente;
    //    banco->filas[aux]->ultimo_c = cliente;
    //    banco->filas[aux]->tamanho++;
    //    banco->t_espera_total += cliente->t_espera;
    //
    //   if(cliente->t_espera > t_espera_max){
    //     return cliente->t_espera;
    //   }
    //   return t_espera_max;
