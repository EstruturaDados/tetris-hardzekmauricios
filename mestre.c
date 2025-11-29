#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA   5   // capacidade da fila
#define TAM_PILHA  3   // capacidade da pilha

// -------------------------------------------------------
// Struct da peça
// -------------------------------------------------------
typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

// -------------------------------------------------------
// Fila circular de peças futuras
// -------------------------------------------------------
typedef struct {
    Peca dados[TAM_FILA];
    int inicio;  // índice do primeiro elemento
    int fim;     // índice da próxima posição livre
    int qtd;     // quantidade de elementos na fila
} Fila;

// -------------------------------------------------------
// Pilha de peças reservadas
// -------------------------------------------------------
typedef struct {
    Peca dados[TAM_PILHA];
    int topo;  // -1 = vazia
} Pilha;

// -------------------------------------------------------
// Protótipos
// -------------------------------------------------------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int enfileirar(Fila *f, Peca p);
int desenfileirar(Fila *f, Peca *p);
void exibirFila(Fila *f);

void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
int empilhar(Pilha *p, Peca x);
int desempilhar(Pilha *p, Peca *x);
void exibirPilha(Pilha *p);

Peca gerarPeca(int *proxId);

void trocarPecaAtual(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

// -------------------------------------------------------
// Implementação da fila circular
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
    return (f->qtd == TAM_FILA);
}

int enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        // Em teoria, não deveria enfileirar além da capacidade
        return 0;
    }
    f->dados[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->qtd++;
    return 1;
}

int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("\n[ERRO] Fila vazia! Nao ha pecas para remover.\n");
        return 0;
    }
    *p = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
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
            idx = (idx + 1) % TAM_FILA;
        }
    }
    printf("\n");
}

// -------------------------------------------------------
// Implementação da pilha
// -------------------------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_PILHA - 1);
}

int empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n[ERRO] Pilha cheia! Nao e possivel reservar mais pecas.\n");
        return 0;
    }
    p->topo++;
    p->dados[p->topo] = x;
    return 1;
}

int desempilhar(Pilha *p, Peca *x) {
    if (pilhaVazia(p)) {
        printf("\n[ERRO] Pilha vazia! Nao ha pecas reservadas para usar.\n");
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
        printf("(Topo -> base): ");
        for (int i = p->topo; i >= 0; i--) {
            Peca x = p->dados[i];
            printf("[%c %d] ", x.nome, x.id);
        }
    }
    printf("\n");
}

// -------------------------------------------------------
// Geração automática de peça
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
// Trocar peça atual: frente da fila <-> topo da pilha
// -------------------------------------------------------
void trocarPecaAtual(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\n[ERRO] Nao ha peca na frente da fila para trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("\n[ERRO] Nao ha peca na pilha de reserva para trocar.\n");
        return;
    }

    int idxFrente = f->inicio;
    int idxTopo   = p->topo;

    Peca temp = f->dados[idxFrente];
    f->dados[idxFrente] = p->dados[idxTopo];
    p->dados[idxTopo]   = temp;

    printf("\nTroca realizada entre a peca da frente da fila e o topo da pilha.\n");
}

// -------------------------------------------------------
// Troca múltipla: 3 primeiras da fila <-> 3 da pilha
// -------------------------------------------------------
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->qtd < 3) {
        printf("\n[ERRO] A fila precisa ter pelo menos 3 pecas para troca multipla.\n");
        return;
    }
    if (p->topo + 1 < 3) {
        printf("\n[ERRO] A pilha precisa ter 3 pecas para troca multipla.\n");
        return;
    }

    // Índices das 3 primeiras posições da fila
    int idxFila[3];
    for (int i = 0; i < 3; i++) {
        idxFila[i] = (f->inicio + i) % TAM_FILA;
    }

    // Índices das 3 posições da pilha: topo, topo-1, topo-2
    int idxPilha[3];
    for (int i = 0; i < 3; i++) {
        idxPilha[i] = p->topo - i;
    }

    // Troca par a par
    for (int i = 0; i < 3; i++) {
        Peca temp = f->dados[idxFila[i]];
        f->dados[idxFila[i]] = p->dados[idxPilha[i]];
        p->dados[idxPilha[i]] = temp;
    }

    printf("\nTroca multipla realizada entre as 3 primeiras pecas da fila e as 3 da pilha.\n");
}

// -------------------------------------------------------
// Função principal - Nível Mestre
// -------------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int proxId = 0;
    Peca p;

    srand((unsigned)time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com TAM_FILA peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca(&proxId);
        enfileirar(&fila, nova);
    }

    printf("===== Nível Mestre - Tetris Stack (Fila + Pilha + Trocas) =====\n");

    do {
        printf("\n=== ESTADO ATUAL ===\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça da fila
                if (desenfileirar(&fila, &p)) {
                    printf("\nPeca jogada: [%c %d]\n", p.nome, p.id);
                    // gera nova para manter a fila cheia
                    Peca nova = gerarPeca(&proxId);
                    enfileirar(&fila, nova);
                }
                break;

            case 2: // Reservar peça (fila -> pilha)
                if (pilhaCheia(&pilha)) {
                    printf("\n[ERRO] Pilha cheia! Nao e possivel reservar.\n");
                } else if (desenfileirar(&fila, &p)) {
                    if (empilhar(&pilha, p)) {
                        printf("\nPeca [%c %d] movida da fila para a pilha de reserva.\n",
                               p.nome, p.id);
                        // repor fila
                        Peca nova = gerarPeca(&proxId);
                        enfileirar(&fila, nova);
                    }
                }
                break;

            case 3: // Usar peça reservada (topo da pilha)
                if (desempilhar(&pilha, &p)) {
                    printf("\nPeca reservada usada: [%c %d]\n", p.nome, p.id);
                    // gerar nova apenas para a fila (a peça usada sai do jogo)
                    Peca nova = gerarPeca(&proxId);
                    enfileirar(&fila, nova);
                }
                break;

            case 4: // Trocar peça atual (frente da fila <-> topo da pilha)
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5: // Troca múltipla (3 primeiras da fila <-> 3 da pilha)
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando simulacao do nivel Mestre. GG!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
