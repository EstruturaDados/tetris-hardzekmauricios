#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MAX_FILA   10   // capacidade total da fila
#define TAM_INICIAL    5    // quantidade inicial de pecas na fila
#define TAM_MAX_PILHA  3    // capacidade maxima da pilha de reserva

// -------------------------------------------------------
// Struct da peca do Tetris Stack
// -------------------------------------------------------
typedef struct {
    char nome;  // tipo da peca: 'I', 'O', 'T', 'L'
    int id;     // identificador unico
} Peca;

// -------------------------------------------------------
// Fila circular de pecas futuras
// -------------------------------------------------------
typedef struct {
    Peca dados[TAM_MAX_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// -------------------------------------------------------
// Pilha de reserva
// -------------------------------------------------------
typedef struct {
    Peca dados[TAM_MAX_PILHA];
    int topo;  // indice do topo (-1 = vazia)
} Pilha;

// -------------------------------------------------------
// Prototipos
// -------------------------------------------------------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void enfileirar(Fila *f, Peca p);
int desenfileirar(Fila *f, Peca *p);
void exibirFila(Fila *f);

void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
int empilhar(Pilha *p, Peca x);
int desempilhar(Pilha *p, Peca *x);
void exibirPilha(Pilha *p);

Peca gerarPeca(int *proxId);

// -------------------------------------------------------
// Implementacao da fila circular
// -------------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

int filaVazia(Fila *f) {
    return (f->qtd == 0);
}

int filaCheia(Fila *f) {
    return (f->qtd == TAM_MAX_FILA);
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        // Em teoria nao deveria acontecer, pois sempre mantemos fluxo
        printf("\n[ERRO] Fila cheia! Nao foi possivel enfileirar nova peca.\n");
        return;
    }
    f->dados[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_MAX_FILA;
    f->qtd++;
}

int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("\n[ERRO] Fila vazia! Nao ha pecas para remover.\n");
        return 0;
    }
    *p = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_MAX_FILA;
    f->qtd--;
    return 1;
}

void exibirFila(Fila *f) {
    printf("Fila de pecas   : ");
    if (filaVazia(f)) {
        printf("[fila vazia]");
    } else {
        int idx = f->inicio;
        for (int i = 0; i < f->qtd; i++) {
            Peca p = f->dados[idx];
            printf("[%c %d] ", p.nome, p.id);
            idx = (idx + 1) % TAM_MAX_FILA;
        }
    }
    printf("\n");
}

// -------------------------------------------------------
// Implementacao da pilha de reserva
// -------------------------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_MAX_PILHA - 1);
}

int empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n[ERRO] Pilha de reserva cheia! Nao e possivel reservar mais pecas.\n");
        return 0;
    }
    p->topo++;
    p->dados[p->topo] = x;
    return 1;
}

int desempilhar(Pilha *p, Peca *x) {
    if (pilhaVazia(p)) {
        printf("\n[ERRO] Pilha de reserva vazia! Nao ha pecas reservadas para usar.\n");
        return 0;
    }
    *x = p->dados[p->topo];
    p->topo--;
    return 1;
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva: ");
    if (pilhaVazia(p)) {
        printf("[sem pecas reservadas]");
    } else {
        printf("(Topo -> Base): ");
        for (int i = p->topo; i >= 0; i--) {
            Peca x = p->dados[i];
            printf("[%c %d] ", x.nome, x.id);
        }
    }
    printf("\n");
}

// -------------------------------------------------------
// Gera automaticamente uma nova peca
// -------------------------------------------------------
Peca gerarPeca(int *proxId) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int qtdTipos = sizeof(tipos) / sizeof(tipos[0]);
    int indice = rand() % qtdTipos;

    p.nome = tipos[indice];
    p.id   = (*proxId)++;

    return p;
}

// -------------------------------------------------------
// Funcao principal - Nivel Aventureiro
// -------------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int proxId = 0;   // controla ID unico das pecas
    Peca p;

    srand((unsigned)time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com TAM_INICIAL pecas
    for (int i = 0; i < TAM_INICIAL; i++) {
        Peca nova = gerarPeca(&proxId);
        enfileirar(&fila, nova);
    }

    printf("===== Nível Aventureiro - Tetris Stack (Fila + Pilha) =====\n");

    // Loop principal
    do {
        printf("\n=== ESTADO ATUAL ===\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peca
                if (desenfileirar(&fila, &p)) {
                    printf("\nPeca jogada: [%c %d]\n", p.nome, p.id);
                    // Gerar nova peca para manter a fila cheia
                    Peca nova = gerarPeca(&proxId);
                    enfileirar(&fila, nova);
                }
                break;

            case 2: // Reservar peca
                if (pilhaCheia(&pilha)) {
                    printf("\n[ERRO] Pilha de reserva cheia! Nao e possivel reservar.\n");
                } else if (desenfileirar(&fila, &p)) {
                    if (empilhar(&pilha, p)) {
                        printf("\nPeca [%c %d] movida da fila para a pilha de reserva.\n",
                               p.nome, p.id);
                        // reposicao na fila
                        Peca nova = gerarPeca(&proxId);
                        enfileirar(&fila, nova);
                    }
                }
                break;

            case 3: // Usar peca reservada
                if (desempilhar(&pilha, &p)) {
                    printf("\nPeca reservada usada: [%c %d]\n", p.nome, p.id);
                    // a peca usada nao volta; apenas geramos nova para fila
                    Peca nova = gerarPeca(&proxId);
                    enfileirar(&fila, nova);
                }
                break;

            case 0:
                printf("\nEncerrando simulacao do nivel Aventureiro. Ate a proxima rodada!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
