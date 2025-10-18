#include <stdio.h>
// tetris.c
// Simulação da fila de peças futuras do jogo "Tetris Stack".
// Implementa uma fila circular com capacidade fixa e menu para
// jogar (dequeue) e inserir (enqueue) peças. Peças são geradas
// automaticamente pela função gerarPeca().

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Capacidade fixa da fila (número de peças futuras mantidas)
#define CAPACIDADE_FILA 5

// Tipos possíveis de peças
const char TIPOS_PECAS[] = {'I', 'O', 'T', 'L'};
const int NUM_TIPOS = sizeof(TIPOS_PECAS) / sizeof(TIPOS_PECAS[0]);

// Struct que representa uma peça
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único sequencial
} Peca;

// Fila circular simples
typedef struct {
    Peca dados[CAPACIDADE_FILA];
    int frente; // índice do elemento na frente
    int tamanho; // número de elementos atualmente na fila
} Fila;

// Funções da fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tamanho = 0;
}

int filaVazia(const Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(const Fila *f) {
    return f->tamanho == CAPACIDADE_FILA;
}

// Insere no final (enqueue). Retorna 1 se sucesso, 0 se cheia.
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int pos = (f->frente + f->tamanho) % CAPACIDADE_FILA;
    f->dados[pos] = p;
    f->tamanho++;
    return 1;
}

// Remove da frente (dequeue). Retorna 1 se sucesso, 0 se vazia.
int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) return 0;
    *out = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

// Gerador de peças: escolhe um tipo aleatório e atribui um id sequencial
Peca gerarPeca(int *contadorIds) {
    Peca p;
    int idx = rand() % NUM_TIPOS;
    p.nome = TIPOS_PECAS[idx];
    p.id = (*contadorIds)++;
    return p;
}

// Exibe a fila de forma legível
void mostrarFila(const Fila *f) {
    printf("\nFila de peças\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    // Imprime do elemento da frente para o final, respeitando circularidade
    for (int i = 0; i < f->tamanho; ++i) {
        int idx = (f->frente + i) % CAPACIDADE_FILA;
        Peca p = f->dados[idx];
        printf("[%c %d] ", p.nome, p.id);
    }
    printf("\n");
}

// Exibe o menu de ações
void mostrarMenu() {
    printf("\nOpções de ação:\n\n");
    printf("Código\tAção\n");
    printf("1\tJogar peça (dequeue)\n");
    printf("2\tInserir nova peça (enqueue)\n");
    printf("0\tSair\n");
}

int main() {
    // seed para geração aleatória
    srand((unsigned int) time(NULL));

    Fila fila;
    inicializarFila(&fila);
    int contadorIds = 0; // gera ids sequenciais

    // Inicializa a fila com CAPACIDADE_FILA peças
    for (int i = 0; i < CAPACIDADE_FILA; ++i) {
        Peca p = gerarPeca(&contadorIds);
        enqueue(&fila, p);
    }

    int opcao = -1;
    Peca removida;

    // Loop do menu
    while (1) {
        mostrarFila(&fila);
        mostrarMenu();
        printf("\nEscolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            // entrada inválida, descarta a linha
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
            // Jogar peça: remove da frente e automaticamente completa com nova peça
            if (dequeue(&fila, &removida)) {
                printf("\nJogou a peça: [%c %d]\n", removida.nome, removida.id);
                // Após remover, sempre reponha para manter a fila cheia
                Peca nova = gerarPeca(&contadorIds);
                if (enqueue(&fila, nova)) {
                    printf("Nova peça inserida automaticamente: [%c %d]\n", nova.nome, nova.id);
                } else {
                    // Na prática, essa situação não deve ocorrer porque repondo imediatamente
                    printf("Falha ao repor peça: fila cheia\n");
                }
            } else {
                printf("Fila vazia. Nada a jogar.\n");
            }
        } else if (opcao == 2) {
            // Inserir nova peça manual (mas peça é gerada automaticamente)
            if (filaCheia(&fila)) {
                printf("Fila está cheia. Não é possível inserir nova peça.\n");
            } else {
                Peca nova = gerarPeca(&contadorIds);
                if (enqueue(&fila, nova)) {
                    printf("Peça inserida: [%c %d]\n", nova.nome, nova.id);
                }
            }
        } else {
            printf("Opção não reconhecida. Use 0, 1 ou 2.\n");
        }
    }

    return 0;
}

