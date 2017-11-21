#ifndef FILA_H
#define FILA_H


typedef struct cliente{
  double t_chegada;
  double t_atendimento;
  double t_espera;
  double t_em_fila;
  struct cliente* prox_c;
} CLIENTE;

typedef struct fila{
  CLIENTE* ultimo_c;
  CLIENTE* primeiro_c;
  int tamanho;
} FILA;

typedef struct banco{
  FILA** filas;
  CLIENTE** caixas;
  double t_espera_total;
  double temp_med;
  double temp_max;
  double tempo_global;
  int atendidos;
}BANCO;

BANCO* criar_banco(int NUM_C);
FILA* criar_fila();
int apagar_fila(FILA** fila);
int apagar_cliente(CLIENTE** cliente);
int apagar_banco(BANCO** banco);
CLIENTE* novo_cliente(double t_chegada, double t_atendimento);
void insere_cliente(FILA* fila, CLIENTE* cliente);
void entrar_na_fila(BANCO* banco, FILA* fila, int NUM_C);
void atende_cliente(BANCO* banco, int NUM, int NUM_C);
void chama_prox_cl_caso_1(BANCO* banco, int NUM_C);
void chama_prox_cl_caso_2(BANCO* banco, FILA* fila, int NUM_C);
void temp_medio(BANCO* banco, int NUM);
#endif
