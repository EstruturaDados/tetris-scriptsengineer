// tetris.c
// Simulação da fila de peças futuras do jogo "Tetris Stack" com pilha de reserva.
// Implementa fila circular (CAPACIDADE_FILA) e pilha LIFO (CAPACIDADE_PILHA).

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Configurações
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Tipos possíveis de peças
const char TIPOS_PECAS[] = {'I', 'O', 'T', 'L'};
const int NUM_TIPOS = sizeof(TIPOS_PECAS) / sizeof(TIPOS_PECAS[0]);

// Representação de uma peça
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único sequencial
} Peca;

// Fila circular
typedef struct {
    Peca dados[CAPACIDADE_FILA];
    int frente; // índice do elemento da frente
    int tamanho; // número de elementos
} Fila;

// Pilha LIFO
typedef struct {
    Peca dados[CAPACIDADE_PILHA];
    int topo; // quantidade de elementos (0 = vazia)
} Pilha;

// --- Funções da fila ---
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tamanho = 0;
}

int filaVazia(const Fila *f) { return f->tamanho == 0; }
int filaCheia(const Fila *f) { return f->tamanho == CAPACIDADE_FILA; }

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int pos = (f->frente + f->tamanho) % CAPACIDADE_FILA;
    f->dados[pos] = p;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) return 0;
    *out = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

// --- Funções da pilha ---
void inicializarPilha(Pilha *p) { p->topo = 0; }
int pilhaVazia(const Pilha *p) { return p->topo == 0; }
int pilhaCheia(const Pilha *p) { return p->topo == CAPACIDADE_PILHA; }

int push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return 0;
    p->dados[p->topo++] = item;
    return 1;
}

int pop(Pilha *p, Peca *out) {
    if (pilhaVazia(p)) return 0;
    *out = p->dados[--p->topo];
    return 1;
}

// --- Gerador de peças ---
Peca gerarPeca(int *contadorIds) {
    Peca p;
    int idx = rand() % NUM_TIPOS;
    p.nome = TIPOS_PECAS[idx];
    p.id = (*contadorIds)++;
    return p;
}

// --- Exibição ---
void mostrarFila(const Fila *f) {
    printf("\nFila de peças\t");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < f->tamanho; ++i) {
        int idx = (f->frente + i) % CAPACIDADE_FILA;
        Peca p = f->dados[idx];
        printf("[%c %d] ", p.nome, p.id);
    }
    printf("\n");
}

void mostrarPilha(const Pilha *p) {
    printf("Pilha de reserva \t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo - 1; i >= 0; --i) {
        Peca item = p->dados[i];
        printf("[%c %d] ", item.nome, item.id);
    }
    printf("\n");
}

void mostrarMenu() {
    printf("\nOpções de Ação:\n\n");
    printf("Código\t Ação\n");
    printf("1\tJogar peça\n");
    printf("2\tReservar peça\n");
    printf("3\tUsar peça reservada\n");
    printf("0\tSair\n");
}

// --- Programa principal ---
int main(void) {
    srand((unsigned int) time(NULL));

    Fila fila;
    Pilha reserva;
    inicializarFila(&fila);
    inicializarPilha(&reserva);

    int contadorIds = 0;

    // Inicializa a fila com CAPACIDADE_FILA peças
    for (int i = 0; i < CAPACIDADE_FILA; ++i) {
        Peca p = gerarPeca(&contadorIds);
        enqueue(&fila, p);
    }

    int opcao;
    Peca temp;

    while (1) {
        mostrarFila(&fila);
        mostrarPilha(&reserva);
        mostrarMenu();
        printf("\nOpção: ");
        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) ;
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (opcao == 0) {
            printf("Saindo...\n");
            break;
        }

        if (opcao == 1) {
            // Jogar peça: remove da frente da fila
            if (dequeue(&fila, &temp)) {
                printf("\nJogou a peça: [%c %d]\n", temp.nome, temp.id);
            } else {
                printf("Fila vazia. Nada a jogar.\n");
            }
            // Repor nova peça para manter fila cheia
            Peca nova = gerarPeca(&contadorIds);
            if (enqueue(&fila, nova)) {
                printf("Nova peça inserida automaticamente: [%c %d]\n", nova.nome, nova.id);
            } else {
                printf("Fila cheia: não foi possível inserir nova peça.\n");
            }

        } else if (opcao == 2) {
            // Reservar peça: move da frente da fila para o topo da pilha
            if (filaVazia(&fila)) {
                printf("Fila vazia. Nada para reservar.\n");
            } else if (pilhaCheia(&reserva)) {
                printf("Pilha de reserva cheia. Não é possível reservar.\n");
            } else {
                dequeue(&fila, &temp);
                push(&reserva, temp);
                printf("Peça reservada: [%c %d] (enviada para o topo da pilha)\n", temp.nome, temp.id);
                // Repor
                Peca nova = gerarPeca(&contadorIds);
                if (enqueue(&fila, nova)) {
                    printf("Nova peça inserida automaticamente: [%c %d]\n", nova.nome, nova.id);
                } else {
                    printf("Falha ao repor peça: fila cheia.\n");
                }
            }

        } else if (opcao == 3) {
            // Usar peça reservada: remove do topo da pilha
            if (pop(&reserva, &temp)) {
                printf("\nUsou a peça reservada: [%c %d]\n", temp.nome, temp.id);
            } else {
                printf("Pilha de reserva vazia. Nada a usar.\n");
            }
            // Tentar repor uma nova peça na fila (se houver espaço)
            Peca nova = gerarPeca(&contadorIds);
            if (enqueue(&fila, nova)) {
                printf("Nova peça inserida automaticamente: [%c %d]\n", nova.nome, nova.id);
            } else {
                // Se fila estiver cheia, apenas informamos; a fila continua cheia
                printf("Fila já cheia; nova peça não foi inserida.\n");
            }

        } else {
            printf("Opção não reconhecida. Use 0, 1, 2 ou 3.\n");
        }
    }

    return 0;
}

