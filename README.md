# SISTEMA DE GERENCIAMENTO DE TAREFAS

Sistema em linguagem C para cadastro, ordenação, filas e histórico de execução de tarefas.

## Visão geral

Este projeto implementa um sistema de gestão de tarefas em memória, utilizando estruturas encadeadas em C puro, sem bibliotecas externas para manipulação de dados. O programa permite:

- cadastrar tarefas com ID, descrição, prioridade e tempo estimado;
- listar tarefas ativas;
- ordenar por prioridade ou tempo estimado;
- enfileirar tarefas para execução;
- executar tarefas da fila e registrar o histórico em pilha;
- liberar memória corretamente ao encerrar o programa.

## Estruturas principais

- `Tarefa`: representa a tarefa com os campos básicos do domínio.
- `Nodo`: nó da lista encadeada usado em fila e pilha.
- `Fila`: estrutura com ponteiros `inicio` e `fim`.
- `Pilha`: estrutura com ponteiro `topo`.

## Funcionalidades

### Cadastro

A tarefa é cadastrada com:

- `id`
- `descricao`
- `prioridade` (faixa válida: 1 a 10)
- `tempo_estimado` (em minutos)

A validação impõe que a prioridade esteja no intervalo correto e que o ID não seja duplicado.

### Ordenação

O sistema oferece ordenação em-place com os algoritmos requisitados:

- `Bubble Sort`
- `Insertion Sort`

Com critério:

- prioridade
- tempo estimado

### Fila e pilha

- `inserirFila()` enfileira a tarefa pelo fim.
- `removerFila()` desenfileira pela cabeça.
- `pushPilha()` empilha a tarefa concluída no topo.
- `popPilha()` remove o topo da pilha.
- `listarPilha()` mostra o histórico em ordem LIFO.

## Compilação

```bash
gcc main.c -o sistema_tarefas
```

## Execução

```bash
./sistema_tarefas
```

## Menu principal

O programa oferece as seguintes opções:

1. Cadastrar tarefa
2. Listar tarefas
3. Ordenar tarefas
4. Adicionar tarefa à fila
5. Executar tarefa da fila
6. Mostrar histórico (pilha)
0. Sair

## Observações

- A leitura de entrada usa `scanf` com limpeza do buffer via `getchar`.
- A descrição usa `fgets` para evitar `gets`, com remoção da quebra de linha com `strcspn`.
- A memória alocada para nós de fila e pilha é liberada antes da saída.

