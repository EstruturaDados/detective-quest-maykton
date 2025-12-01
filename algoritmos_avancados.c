#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// --- Constantes ---
#define TAM_NOME_SALA 50

// --- Estrutura de Dados ---
// Nó da Árvore Binária (Representa uma Sala ou Cômodo)
typedef struct Sala {
    char nome[TAM_NOME_SALA];
    struct Sala *esquerda; // Ponteiro para o cômodo à esquerda
    struct Sala *direita;  // Ponteiro para o cômodo à direita
} Sala;

// --- Protótipos das Funções ---
Sala* criarSala(const char *nome);
void montarMapaEstatico(Sala **raiz);
void explorarSalas(Sala *raiz);
void liberarMapa(Sala *raiz);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    Sala *mansao = NULL;
    
    printf("--- Detective Quest: Mapa da Mansão (Nível Novato) ---\n");
    
    // 1. Montagem do Mapa (Criação da Árvore Binária)
    montarMapaEstatico(&mansao);
    
    if (mansao == NULL) {
        fprintf(stderr, "Erro ao montar o mapa.\n");
        return 1;
    }

    // 2. Iniciar a Exploração Interativa
    printf("Exploração iniciada no Hall de Entrada.\n");
    explorarSalas(mansao);

    // 3. Limpeza de Memória
    liberarMapa(mansao);
    printf("\n🧹 Mapa liberado. Programa encerrado.\n");

    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

/**
 * @brief Cria e aloca dinamicamente uma nova sala (nó da árvore).
 * @param nome O nome da sala.
 * @return Ponteiro para a nova Sala.
 */
Sala* criarSala(const char *nome) {
    // Aloca memória para a nova estrutura Sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        perror("Erro ao alocar memória para a sala");
        exit(EXIT_FAILURE);
    }
    
    // Inicializa os campos
    strncpy(novaSala->nome, nome, TAM_NOME_SALA - 1);
    novaSala->nome[TAM_NOME_SALA - 1] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Monta a estrutura estática da Árvore Binária.
 * A estrutura é passada por referência dupla (Sala **raiz) para modificar o ponteiro principal.
 */
void montarMapaEstatico(Sala **raiz) {
    // Nível 0
    *raiz = criarSala("Hall de Entrada");

    // Nível 1
    (*raiz)->esquerda = criarSala("Sala de Jantar");
    (*raiz)->direita = criarSala("Biblioteca");

    // Nível 2
    (*raiz)->esquerda->esquerda = criarSala("Cozinha");
    (*raiz)->esquerda->direita = criarSala("Sala de Estar");

    (*raiz)->direita->esquerda = criarSala("Escritório");
    (*raiz)->direita->direita = criarSala("Quarto Principal");

    // Nível 3 (Nós-folha, fim dos caminhos)
    (*raiz)->esquerda->esquerda->esquerda = criarSala("Despensa (FIM)");
    (*raiz)->esquerda->esquerda->direita = criarSala("Jardim de Inverno (FIM)");

    (*raiz)->direita->direita->esquerda = criarSala("Banheiro (FIM)");
}

/**
 * @brief Permite que o jogador explore a mansão interativamente.
 * @param raiz O nó inicial de onde começar a exploração (Hall de Entrada).
 */
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: ➡️ **%s**\n", atual->nome);

        // Se for um nó-folha (fim do caminho), encerra a exploração
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("----------------------------------------\n");
            printf("🚪 Este é um beco sem saída. Exploração encerrada para este caminho.\n");
            printf("----------------------------------------\n");
            return; 
        }

        // Exibe opções e solicita a escolha
        printf("Opções disponíveis:\n");
        if (atual->esquerda != NULL) {
            printf(" [E] Esquerda (Ir para: %s)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf(" [D] Direita (Ir para: %s)\n", atual->direita->nome);
        }
        printf(" [S] Sair da exploração\n");
        printf("Sua escolha (E/D/S): ");

        if (scanf(" %c", &escolha) != 1) {
             // Limpar o buffer em caso de entrada inválida
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        switch (escolha) {
            case 'e':
            case 'E':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("❌ Não há caminho para a esquerda a partir daqui.\n");
                }
                break;
            
            case 'd':
            case 'D':
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("❌ Não há caminho para a direita a partir daqui.\n");
                }
                break;
            
            case 's':
            case 'S':
                printf("Encerrando a exploração atual.\n");
                return;
            
            default:
                printf("Opção inválida. Use E, D ou S.\n");
                break;
        }
    }
}

/**
 * @brief Libera recursivamente toda a memória alocada para a árvore (Mapa).
 * @param raiz O nó raiz da sub-árvore a ser liberada.
 */
void liberarMapa(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Libera sub-árvore esquerda (Pós-ordem)
    liberarMapa(raiz->esquerda);
    // Libera sub-árvore direita
    liberarMapa(raiz->direita);
    
    // Libera o nó atual
    free(raiz);
}
    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca

    //#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes ---
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3 // Nova constante

// --- Estruturas de Dados ---
typedef struct {
    char nome[5]; 
    int id;       
} Peca;

// Fila (Próximas Peças)
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente; 
    int tras;   
    int contador; 
} FilaCircular;

// Pilha (Reserva de Peças)
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; 
} PilhaReserva;

// --- Protótipos das Funções ---
// Fila:
void inicializarFila(FilaCircular *f);
void enqueue(FilaCircular *f, Peca novaPeca);
Peca dequeue(FilaCircular *f);
int estaVaziaFila(const FilaCircular *f);
void visualizarFila(const FilaCircular *f);
// Pilha:
void inicializarPilha(PilhaReserva *p);
int push(PilhaReserva *p, Peca peca);
Peca pop(PilhaReserva *p);
void visualizarPilha(const PilhaReserva *p);
// Utilitárias:
Peca gerarPeca(int *idAtual);
void exibirMenuAventureiro();

// --- Variáveis Globais de Controle ---
int idGlobalPeca = 1;

// ============================================================================
// --- Função Principal (main) - Nível Aventureiro ---
// ============================================================================

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    FilaCircular fila;
    PilhaReserva pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    int opcao;
    Peca tempPeca;

    printf("Iniciando Tetris Stack - Nível Aventureiro (Fila e Pilha de Reserva)\n");

    do {
        printf("\n--- ESTADO ATUAL ---\n");
        visualizarFila(&fila);
        visualizarPilha(&pilha);
        
        exibirMenuAventureiro();
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            while(getchar() != '\n'); 
        }

        switch (opcao) {
            case 1: // Jogar (Dequeue e Enqueue)
                if (estaVaziaFila(&fila)) {
                    printf("❌ Erro: Fila vazia.\n");
                } else {
                    tempPeca = dequeue(&fila);
                    printf("✅ Peça JOGADA: [Nome: %s, ID: %d]\n", tempPeca.nome, tempPeca.id);
                    enqueue(&fila, gerarPeca(&idGlobalPeca));
                }
                break;

            case 2: { // Reservar peça (Push)
                if (estaVaziaFila(&fila)) {
                    printf("❌ Fila vazia! Não há peça para reservar.\n");
                    break;
                }
                tempPeca = dequeue(&fila); // Tenta remover da fila
                if (push(&pilha, tempPeca)) { // Tenta adicionar na pilha
                    printf("✅ Peça [Nome: %s, ID: %d] reservada na pilha.\n", tempPeca.nome, tempPeca.id);
                    enqueue(&fila, gerarPeca(&idGlobalPeca)); // Reabastece a fila
                } else {
                    enqueue(&fila, tempPeca); // Devolve à fila se a pilha estiver cheia
                }
                break;
            }

            case 3: // Usar peça reservada (Pop)
                tempPeca = pop(&pilha);
                if (tempPeca.id != -1) {
                    printf("✅ Peça [Nome: %s, ID: %d] usada da reserva.\n", tempPeca.nome, tempPeca.id);
                } else {
                    printf("❌ Pilha de Reserva vazia! Nada para usar.\n");
                }
                break;
            
            case 0:
                printf("Encerrando o jogo. Até logo!\n");
                break;
            
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// --- Implementação das Funções (Fila, Pilha e Utilitárias) ---
// ============================================================================

// --- Funções Comuns e Utilitárias ---
Peca gerarPeca(int *idAtual) {
    const char *tipos[] = {"I", "O", "T", "L", "J", "S", "Z"};
    Peca nova;
    nova.id = (*idAtual)++;
    int indice = rand() % (sizeof(tipos) / sizeof(tipos[0]));
    strncpy(nova.nome, tipos[indice], 4);
    nova.nome[4] = '\0';
    return nova;
}

void exibirMenuAventureiro() {
    printf("\n--- Menu Aventureiro ---\n");
    printf("1 - Jogar peça (Dequeue/Enqueue Automático)\n");
    printf("2 - Reservar peça (Push)\n");
    printf("3 - Usar peça reservada (Pop)\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

// --- Funções da Fila ---
void inicializarFila(FilaCircular *f) {
    f->frente = 0; f->tras = 0; f->contador = 0;
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        Peca p = gerarPeca(&idGlobalPeca);
        // Usa enqueue simples para inicialização
        f->itens[f->tras] = p;
        f->tras = (f->tras + 1) % CAPACIDADE_FILA;
        f->contador++;
    }
}
int estaVaziaFila(const FilaCircular *f) { return f->contador == 0; }
void enqueue(FilaCircular *f, Peca novaPeca) {
    if (f->contador == CAPACIDADE_FILA) return;
    f->itens[f->tras] = novaPeca;
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->contador++;
}
Peca dequeue(FilaCircular *f) {
    if (f->contador == 0) { Peca nula = {"NULA", -1}; return nula; }
    Peca pecaRemovida = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->contador--;
    return pecaRemovida;
}
void visualizarFila(const FilaCircular *f) {
    printf("Fila Circular de Peças (%d/%d): ", f->contador, CAPACIDADE_FILA);
    if (f->contador == 0) { printf("[Fila Vazia]\n"); return; }
    printf("FRENTE -> ");
    int i = f->frente; int count = 0;
    while (count < f->contador) {
        printf("[%s ID:%d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % CAPACIDADE_FILA;
        count++;
    }
    printf("<- FINAL\n");
}

// --- Funções da Pilha ---
void inicializarPilha(PilhaReserva *p) { p->topo = -1; }
int push(PilhaReserva *p, Peca peca) {
    if (p->topo >= CAPACIDADE_PILHA - 1) {
        printf("❌ Erro: Pilha de Reserva cheia! (Máx: %d)\n", CAPACIDADE_PILHA);
        return 0; // Falha
    }
    p->topo++;
    p->itens[p->topo] = peca;
    return 1; // Sucesso
}
Peca pop(PilhaReserva *p) {
    if (p->topo < 0) { Peca nula = {"NULA", -1}; return nula; }
    Peca pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}
void visualizarPilha(const PilhaReserva *p) {
    printf("Pilha de Reserva (%d/%d): ", p->topo + 1, CAPACIDADE_PILHA);
    if (p->topo < 0) { printf("[Pilha Vazia]\n"); return; }
    printf("TOPO -> ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%s ID:%d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("<- BASE\n");
}
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// --- Constantes ---
#define TAM_NOME_SALA 50
#define TAM_NOME_PISTA 50
#define TAM_NOME_SUSPEITO 50
#define TAM_TABELA_HASH 7 // Tamanho primo para a tabela hash

// ============================================================================
// --- Estruturas de Dados ---
// ============================================================================

// 1. Nó da Árvore Binária (Mapa - Cômodo)
typedef struct Sala {
    char nome[TAM_NOME_SALA];
    int temPista;
    char pistaEncontrada[TAM_NOME_PISTA]; 
    struct Sala *esquerda; 
    struct Sala *direita;  
} Sala;

// 2. Nó da Árvore Binária de Busca (Pistas)
typedef struct PistaNode {
    char nome[TAM_NOME_PISTA];
    struct PistaNode *esquerda; 
    struct PistaNode *direita;  
} PistaNode;

// 3. Estrutura para Tabela Hash (Vínculo Pista -> Suspeito)
// Nó da Lista Encadeada (usada para encadeamento da Hash)
typedef struct HashNode {
    char pista[TAM_NOME_PISTA];
    char suspeito[TAM_NOME_SUSPEITO];
    struct HashNode *proximo;
} HashNode;

// Tabela Hash (Array de ponteiros para HashNode)
typedef HashNode* TabelaHash[TAM_TABELA_HASH];


// ============================================================================
// --- Protótipos das Funções ---
// ============================================================================

// Funções do Mapa (Árvore Binária)
Sala* criarSala(const char *nome, int temPista, const char *pista);
void montarMapaEstatico(Sala **raiz, TabelaHash hashSuspeitos);
void explorarSalas(Sala *raiz, PistaNode **pistasColetadas, TabelaHash hashSuspeitos);
void liberarMapa(Sala *raiz);

// Funções da BST (Árvore de Busca)
PistaNode* criarPistaNode(const char *nome);
PistaNode* inserirPista(PistaNode *raiz, const char *nome);
PistaNode* buscarPista(PistaNode *raiz, const char *nome);
void listarPistasEmOrdem(PistaNode *raiz);
void liberarPistas(PistaNode *raiz);

// Funções da Tabela Hash
void inicializarHash(TabelaHash hash);
int funcaoHash(const char *chave);
void inserirNaHash(TabelaHash hash, const char *pista, const char *suspeito);
const char* buscarSuspeito(TabelaHash hash, const char *pista);
void analisarSuspeitos(TabelaHash hash, PistaNode *pistasColetadas);
void liberarHash(TabelaHash hash);


// ============================================================================
// --- Função Principal (main) ---
// ============================================================================

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    Sala *mansao = NULL;
    PistaNode *pistasColetadas = NULL;
    TabelaHash hashSuspeitos;
    
    inicializarHash(hashSuspeitos);
    
    printf("--- Detective Quest: Nível Mestre (Hash e Integração) ---\n");
    
    // 1. Montagem do Mapa e Associação Pista -> Suspeito na Hash
    montarMapaEstatico(&mansao, hashSuspeitos);
    
    if (mansao == NULL) {
        fprintf(stderr, "Erro ao montar o mapa.\n");
        return 1;
    }

    // 2. Iniciar a Exploração Interativa
    printf("Exploração iniciada no Hall de Entrada.\n");
    explorarSalas(mansao, &pistasColetadas, hashSuspeitos);
    
    // 3. Análise Final e Solução
    printf("\n\n**************************************************\n");
    printf("🕵️ **ANÁLISE FINAL DE EVIDÊNCIAS** 🕵️\n");
    printf("**************************************************\n");
    analisarSuspeitos(hashSuspeitos, pistasColetadas);
    printf("**************************************************\n");

    // 4. Limpeza de Memória
    liberarMapa(mansao);
    liberarPistas(pistasColetadas);
    liberarHash(hashSuspeitos);
    printf("\n🧹 Memória liberada. Programa encerrado.\n");

    return 0;
}

// ============================================================================
// --- Implementação das Funções da Tabela Hash ---
// ============================================================================

void inicializarHash(TabelaHash hash) {
    for (int i = 0; i < TAM_TABELA_HASH; i++) {
        hash[i] = NULL;
    }
}

/**
 * @brief Função de espalhamento simples: soma dos valores ASCII dos 3 primeiros caracteres.
 * @return Índice da tabela hash.
 */
int funcaoHash(const char *chave) {
    int valor = 0;
    int len = strlen(chave);
    
    // Soma os primeiros 3 caracteres para o hash (se existirem)
    for (int i = 0; i < len && i < 3; i++) {
        valor += (int)chave[i];
    }
    
    return valor % TAM_TABELA_HASH;
}

/**
 * @brief Insere um novo par (pista, suspeito) na Tabela Hash (Encadeamento Separado).
 */
void inserirNaHash(TabelaHash hash, const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    
    HashNode *novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar HashNode");
        exit(EXIT_FAILURE);
    }
    
    strncpy(novoNo->pista, pista, TAM_NOME_PISTA - 1);
    novoNo->pista[TAM_NOME_PISTA - 1] = '\0';
    
    strncpy(novoNo->suspeito, suspeito, TAM_NOME_SUSPEITO - 1);
    novoNo->suspeito[TAM_NOME_SUSPEITO - 1] = '\0';
    
    // Insere no início da lista encadeada (encadeamento)
    novoNo->proximo = hash[indice];
    hash[indice] = novoNo;
}

/**
 * @brief Busca o suspeito associado a uma pista na Tabela Hash.
 * @return O nome do suspeito ou NULL se a pista não for encontrada.
 */
const char* buscarSuspeito(TabelaHash hash, const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = hash[indice];
    
    while (atual != NULL) {
        // Usa strcasecmp para comparação de strings sem distinção de maiúsculas/minúsculas
        if (strcasecmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Encontrado!
        }
        atual = atual->proximo;
    }
    
    return NULL; // Não encontrado
}

/**
 * @brief Percorre a BST de pistas coletadas e usa a Hash para determinar o suspeito mais citado.
 */
void analisarSuspeitos(TabelaHash hash, PistaNode *pistasColetadas) {
    // Array para contar as ocorrências de cada suspeito (Simplificado para 3 suspeitos)
    struct Contagem {
        char nome[TAM_NOME_SUSPEITO];
        int count;
    } contadores[] = {
        {"Sr. Blackwood", 0},
        {"Sra. Scarlet", 0},
        {"Mordomo James", 0},
        {"", 0} // Sentinela
    };

    printf("Pistas e Associações:\n");

    // Usa um percurso InOrder (ou Pré-ordem) na BST para verificar todas as pistas
    // Função auxiliar recursiva para percorrer a BST e contar
    void contarSuspeito(PistaNode *node) {
        if (node == NULL) return;
        
        contarSuspeito(node->esquerda);
        
        const char *suspeitoEncontrado = buscarSuspeito(hash, node->nome);
        
        if (suspeitoEncontrado != NULL) {
            printf("  - %s: Associado a **%s**\n", node->nome, suspeitoEncontrado);
            
            // Incrementa o contador do suspeito
            for (int i = 0; contadores[i].nome[0] != '\0'; i++) {
                if (strcasecmp(contadores[i].nome, suspeitoEncontrado) == 0) {
                    contadores[i].count++;
                    break;
                }
            }
        } else {
            printf("  - %s: Associa\u00e7\u00e3o n\u00e3o encontrada na Hash.\n", node->nome);
        }

        contarSuspeito(node->direita);
    }
    
    contarSuspeito(pistasColetadas);
    
    // Determinar o Suspeito Mais Citado
    int maxContagem = -1;
    const char *culpado = "Indeterminado";

    for (int i = 0; contadores[i].nome[0] != '\0'; i++) {
        if (contadores[i].count > maxContagem) {
            maxContagem = contadores[i].count;
            culpado = contadores[i].nome;
        }
    }
    
    printf("\n🚨 **CONCLUS\u00c3O DA INVESTIGA\u00c7\u00c3O**\n");
    printf("   O Suspeito mais citado nas pistas \u00e9: **%s** (%d evid\u00eancias).\n", culpado, maxContagem);
}

void liberarHash(TabelaHash hash) {
    for (int i = 0; i < TAM_TABELA_HASH; i++) {
        HashNode *atual = hash[i];
        HashNode *temp;
        while (atual != NULL) {
            temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        hash[i] = NULL;
    }
}

// ============================================================================
// --- Implementação das Funções do Mapa (Árvore Binária) ---
// ============================================================================

Sala* criarSala(const char *nome, int temPista, const char *pista)
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

