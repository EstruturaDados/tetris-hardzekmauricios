#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MAX_FILA 10   // capacidade máxima da fila
#define INICIAL 5         // quantidade inicial de peças

// -------------------------------------------------------
// Struct da peça do Tetris Stack
// -------------------------------------------------------
typedef struct {
    char nome;  // tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

// -------------------------------------------------------
// Estrutura de fila circular
// -------------------------------------------------------
typedef struct {
    Peca dados[TAM_MAX_FILA]; // array de peças
    int inicio;               // índice do primeiro elemento
    int fim;                  // índice da próxima posição livre
    int qtd;                  // quantidade de elementos na fila
} Fila;

// -------------------------------------------------------
// Protótipos de funções
// -------------------------------------------------------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void enfileirar(Fila *f, Peca p);
int desenfileirar(Fila *f, Peca *p);
void exibirFila(Fila *f);
Peca gerarPeca(int *proxId);

// -------------------------------------------------------
// Inicializa a fila vazia
// -------------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return (f->qtd == 0);
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return (f->qtd == TAM_MAX_FILA);
}

// -------------------------------------------------------
// Insere uma peça no final da fila (enqueue)
// -------------------------------------------------------
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n[ERRO] Fila cheia! Nao e possivel inserir nova peca.\n");
        return;
    }

    f->dados[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_MAX_FILA; // avança circularmente
    f->qtd++;
}

// -------------------------------------------------------
// Remove uma peça da frente da fila (dequeue)
// Retorna 1 se removeu, 0 se fila vazia
// -------------------------------------------------------
int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("\n[ERRO] Nao ha pecas para jogar. Fila vazia.\n");
        return 0;
    }

    *p = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_MAX_FILA;
    f->qtd--;

    return 1;
}

// -------------------------------------------------------
// Exibe o estado atual da fila
// -------------------------------------------------------
void exibirFila(Fila *f) {
    printf("\n=== Fila de pecas futuras ===\n");

    if (filaVazia(f)) {
        printf("[fila vazia]\n");
        return;
    }

    int i;
    int idx = f->inicio;

    for (i = 0; i < f->qtd; i++) {
        Peca p = f->dados[idx];
        printf("[%c %d] ", p.nome, p.id);
        idx = (idx + 1) % TAM_MAX_FILA;
    }
    printf("\n");
}

// -------------------------------------------------------
// Gera automaticamente uma nova peça
// nome: um dos caracteres do array tipos[]
// id: incrementado a cada nova peça
// -------------------------------------------------------
Peca gerarPeca(int *proxId) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int qtdTipos = sizeof(tipos) / sizeof(tipos[0]);

    int indice = rand() % qtdTipos; // sorteia tipo
    p.nome = tipos[indice];
    p.id = (*proxId)++;

    return p;
}

// -------------------------------------------------------
// Função principal - Nível Novato Tetris Stack
// -------------------------------------------------------
int main() {
    Fila fila;
    int opcao;
    int proxId = 0; // controla o id único das peças
    Peca p;

    // Inicializa semente de números aleatórios
    srand((unsigned)time(NULL));

    // Inicializa a fila
    inicializarFila(&fila);

    // Preenche a fila com INICIAL peças automáticas
    for (int i = 0; i < INICIAL; i++) {
        Peca nova = gerarPeca(&proxId);
        enfileirar(&fila, nova);
    }

    printf("===== Nível Novato - Tetris Stack (Fila de Pecas) =====\n");
    exibirFila(&fila);

    // Menu simples com três opções
    do {
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Jogar peça: remover da frente
                if (desenfileirar(&fila, &p)) {
                    printf("\nPeca jogada: [%c %d]\n", p.nome, p.id);
                }
                exibirFila(&fila);
                break;

            case 2: {
                // Inserir nova peça: gerar automaticamente
                if (filaCheia(&fila)) {
                    printf("\n[ERRO] Fila cheia! Nao e possivel inserir nova peca.\n");
                } else {
                    Peca nova = gerarPeca(&proxId);
                    enfileirar(&fila, nova);
                    printf("\nNova peca gerada e inserida: [%c %d]\n", nova.nome, nova.id);
                }
                exibirFila(&fila);
                break;
            }

            case 0:
                printf("\nEncerrando simulacao da fila de pecas. Ate a proxima partida!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
