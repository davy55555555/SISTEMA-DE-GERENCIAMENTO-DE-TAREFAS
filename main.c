#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char descricao[256];
    int prioridade;
    int tempo_estimado;
    int ativa; // 1 = disponivel no cadastro, 0 = enviada para fila
} Tarefa;

typedef struct Nodo {
    Tarefa dado;
    struct Nodo* proximo;
} Nodo;

typedef struct {
    Nodo* inicio;
    Nodo* fim;
} Fila;

typedef struct {
    Nodo* topo;
} Pilha;

int verificarIdDuplicado(Tarefa* lista, int total, int id) {
    for (int i = 0; i < total; i++) {
        if (lista[i].id == id) {
            return 1;
        }
    }
    return 0;
}

void inserirFila(Fila* f, Tarefa t) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    if (novo == NULL) {
        printf("Erro: falha de alocacao na fila.\n");
        exit(1);
    }

    novo->dado = t;
    novo->proximo = NULL;

    if (f->inicio == NULL) {
        f->inicio = novo;
    } else {
        f->fim->proximo = novo;
    }
    f->fim = novo;
}

Tarefa removerFila(Fila* f) {
    Tarefa t;
    if (f->inicio == NULL) {
        printf("Erro: Fila vazia (Underflow).\n");
        t.id = -1;
        return t;
    }

    Nodo* temp = f->inicio;
    t = temp->dado;
    f->inicio = temp->proximo;

    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    free(temp);
    return t;
}

void pushPilha(Pilha* p, Tarefa t) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    if (novo == NULL) {
        printf("Erro: falha de alocacao na pilha.\n");
        exit(1);
    }

    novo->dado = t;
    novo->proximo = p->topo;
    p->topo = novo;
}

Tarefa popPilha(Pilha* p) {
    Tarefa t;
    if (p->topo == NULL) {
        printf("Erro: Pilha vazia.\n");
        t.id = -1;
        return t;
    }

    Nodo* temp = p->topo;
    t = temp->dado;
    p->topo = temp->proximo;
    free(temp);
    return t;
}

void listarPilha(Pilha* p) {
    if (p->topo == NULL) {
        printf("Pilha vazia.\n");
        return;
    }

    printf("\n=== HISTORICO (LIFO) ===\n");
    Nodo* atual = p->topo;
    int index = 1;
    while (atual != NULL) {
        printf("%d. ID: %d | Descricao: %s | Prioridade: %d | Tempo: %d min\n",
               index,
               atual->dado.id,
               atual->dado.descricao,
               atual->dado.prioridade,
               atual->dado.tempo_estimado);
        atual = atual->proximo;
        index++;
    }
}

void ordenarBubbleSort(Tarefa* array, int tamanho, int criterio) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            int comparacao = (criterio == 0)
                ? array[j].prioridade > array[j + 1].prioridade
                : array[j].tempo_estimado > array[j + 1].tempo_estimado;

            if (comparacao) {
                Tarefa temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void ordenarInsertionSort(Tarefa* array, int tamanho, int criterio) {
    for (int i = 1; i < tamanho; i++) {
        Tarefa chave = array[i];
        int j = i - 1;

        while (j >= 0) {
            int comparacao = (criterio == 0)
                ? array[j].prioridade > chave.prioridade
                : array[j].tempo_estimado > chave.tempo_estimado;

            if (!comparacao) {
                break;
            }

            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = chave;
    }
}

int main() {
    Tarefa* lista_base = (Tarefa*)malloc(sizeof(Tarefa) * 100);
    if (lista_base == NULL) {
        printf("Erro: falha ao alocar lista base.\n");
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        lista_base[i].ativa = 0;
    }

    int total_tarefas = 0;
    Fila fila = {NULL, NULL};
    Pilha pilha = {NULL};
    int opcao;

    while (1) {
        printf("\n=== SISTEMA DE GERENCIAMENTO DE TAREFAS ===\n");
        printf("1. Cadastrar tarefa\n");
        printf("2. Listar tarefas\n");
        printf("3. Ordenar tarefas\n");
        printf("4. Adicionar tarefa a fila\n");
        printf("5. Executar tarefa da fila\n");
        printf("6. Mostrar historico (pilha)\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            if (total_tarefas >= 100) {
                printf("Erro: Limite de tarefas atingido.\n");
                continue;
            }

            printf("ID: ");
            scanf("%d", &lista_base[total_tarefas].id);
            getchar();

            if (verificarIdDuplicado(lista_base, total_tarefas, lista_base[total_tarefas].id)) {
                printf("Erro: ID ja cadastrado. Escolha outro identificador.\n");
                continue;
            }

            printf("Descricao: ");
            fgets(lista_base[total_tarefas].descricao, 256, stdin);
            lista_base[total_tarefas].descricao[strcspn(lista_base[total_tarefas].descricao, "\n")] = 0;

            do {
                printf("Prioridade (1-10): ");
                scanf("%d", &lista_base[total_tarefas].prioridade);
                getchar();
                if (lista_base[total_tarefas].prioridade < 1 || lista_base[total_tarefas].prioridade > 10) {
                    printf("Erro: Prioridade invalida. Deve ser entre 1 e 10.\n");
                }
            } while (lista_base[total_tarefas].prioridade < 1 || lista_base[total_tarefas].prioridade > 10);

            printf("Tempo estimado (minutos): ");
            scanf("%d", &lista_base[total_tarefas].tempo_estimado);
            getchar();

            lista_base[total_tarefas].ativa = 1;
            printf("Tarefa cadastrada com sucesso.\n");
            total_tarefas++;

        } else if (opcao == 2) {
            printf("\n=== TAREFAS CADASTRADAS ===\n");
            int encontrou = 0;
            for (int i = 0; i < total_tarefas; i++) {
                if (lista_base[i].ativa == 1) {
                    printf("%d. ID: %d | Descricao: %s | Prioridade: %d | Tempo: %d min\n",
                           i + 1,
                           lista_base[i].id,
                           lista_base[i].descricao,
                           lista_base[i].prioridade,
                           lista_base[i].tempo_estimado);
                    encontrou = 1;
                }
            }
            if (!encontrou) {
                printf("Nenhuma tarefa ativa.\n");
            }

        } else if (opcao == 3) {
            if (total_tarefas == 0) {
                printf("Nenhuma tarefa para ordenar.\n");
                continue;
            }

            printf("\nCriterio de ordenacao:\n");
            printf("0. Prioridade\n");
            printf("1. Tempo estimado\n");
            printf("Escolha: ");
            int criterio;
            scanf("%d", &criterio);
            getchar();

            printf("\nAlgoritmo:\n");
            printf("0. Bubble Sort\n");
            printf("1. Insertion Sort\n");
            printf("Escolha: ");
            int algoritmo;
            scanf("%d", &algoritmo);
            getchar();

            if (algoritmo == 0) {
                ordenarBubbleSort(lista_base, total_tarefas, criterio);
            } else if (algoritmo == 1) {
                ordenarInsertionSort(lista_base, total_tarefas, criterio);
            } else {
                printf("Algoritmo invalido.\n");
                continue;
            }

            printf("Tarefas ordenadas com sucesso.\n");

        } else if (opcao == 4) {
            printf("\n=== TAREFAS DISPONIVEIS ===\n");
            int encontrou = 0;
            for (int i = 0; i < total_tarefas; i++) {
                if (lista_base[i].ativa == 1) {
                    printf("%d. ID: %d | Descricao: %s\n",
                           i + 1,
                           lista_base[i].id,
                           lista_base[i].descricao);
                    encontrou = 1;
                }
            }

            if (!encontrou) {
                printf("Nenhuma tarefa disponivel.\n");
                continue;
            }

            printf("Selecione o numero da tarefa: ");
            int indice;
            scanf("%d", &indice);
            getchar();

            if (indice < 1 || indice > total_tarefas || lista_base[indice - 1].ativa == 0) {
                printf("Indice invalido.\n");
                continue;
            }

            inserirFila(&fila, lista_base[indice - 1]);
            lista_base[indice - 1].ativa = 0;
            printf("Tarefa adicionada a fila.\n");

        } else if (opcao == 5) {
            Tarefa t = removerFila(&fila);
            if (t.id != -1) {
                pushPilha(&pilha, t);
                printf("Tarefa executada: %s\n", t.descricao);
            }

        } else if (opcao == 6) {
            listarPilha(&pilha);

        } else if (opcao == 0) {
            Nodo* atual = fila.inicio;
            while (atual != NULL) {
                Nodo* temp = atual;
                atual = atual->proximo;
                free(temp);
            }

            atual = pilha.topo;
            while (atual != NULL) {
                Nodo* temp = atual;
                atual = atual->proximo;
                free(temp);
            }

            free(lista_base);
            printf("Programa finalizado.\n");
            exit(0);

        } else {
            printf("Opcao invalida.\n");
        }
    }

    return 0;
}

