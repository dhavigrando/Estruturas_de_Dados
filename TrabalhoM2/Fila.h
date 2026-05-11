#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED
#include <stdexcept>
using namespace std;

// estrutura generica para um item da fila (no da lista encadeada)
template <typename TIPO>
struct TItem {
    TIPO dado;            // dado armazenado no no
    TItem<TIPO>* proximo; // ponteiro para o proximo item da fila
};

// estrutura generica da fila, guarda apenas o ponteiro para o primeiro elemento
template <typename TIPO>
struct TFila {
    TItem<TIPO>* primeiro; // ponteiro para o inicio da fila (primeiro a ser atendido)
};

// inicializa a fila deixando ela vazia, sem nenhum elemento
template <typename TIPO>
void inicializa_fila(TFila<TIPO>& fila) {
    fila.primeiro = NULL; // aponta para NULL indicando que a fila esta vazia
}

// conta e retorna quantos elementos estao na fila no momento
template <typename TIPO>
int tamanho_fila(TFila<TIPO>& fila) {
    int tam = 0;
    TItem<TIPO>* aux = fila.primeiro; // comeca do inicio da fila
    while (aux) {
        tam++;              // conta cada no encontrado
        aux = aux->proximo; // avanca para o proximo
    }
    return tam;
}

// insere um elemento no FINAL da fila (quem chega por ultimo espera no fim)
template <typename TIPO>
void entrar_na_fila(TFila<TIPO>& fila, TIPO d) {
    TItem<TIPO>* novo = new TItem<TIPO>; // aloca memoria para o novo item
    novo->dado = d;
    novo->proximo = NULL; // novo item vai para o final, entao proximo e NULL

    if (fila.primeiro == NULL) {
        fila.primeiro = novo; // fila estava vazia, novo item e o primeiro
    } else {
        TItem<TIPO>* aux = fila.primeiro;
        while (aux->proximo != NULL) { // percorre ate o ultimo elemento
            aux = aux->proximo;
        }
        aux->proximo = novo; // encadeia o novo ao final da fila
    }
}

// remove e retorna o elemento do INICIO da fila (quem espera ha mais tempo e atendido primeiro)
template <typename TIPO>
TIPO atender(TFila<TIPO>& fila) {
    if (fila.primeiro == NULL) {
        throw runtime_error("Fila vazia"); // nao pode atender fila sem ninguem
    }
    TItem<TIPO>* apagado = fila.primeiro;          // guarda ponteiro para liberar depois
    TIPO dado = apagado->dado;                     // salva o dado antes de deletar
    fila.primeiro = fila.primeiro->proximo;        // segundo elemento passa a ser o primeiro
    delete apagado;                                // libera a memoria do elemento removido
    return dado;
}

// retorna o primeiro elemento da fila sem remove-lo
template <typename TIPO>
TIPO primeiro(TFila<TIPO>& fila) {
    if (fila.primeiro == NULL) {
        throw runtime_error("Fila vazia");
    }
    return fila.primeiro->dado; // apenas retorna, nao remove
}

// verifica se a fila esta vazia
template <typename TIPO>
bool fila_vazia(TFila<TIPO>& fila) {
    return fila.primeiro == NULL;
}

// libera toda a memoria dos itens que ainda estao na fila, deixando ela vazia
// usado ao final do programa para evitar vazamento de memoria
template <typename TIPO>
void libera_fila(TFila<TIPO>& fila) {
    while (fila.primeiro != NULL) {
        TItem<TIPO>* aux = fila.primeiro;       // guarda o primeiro item para liberar
        fila.primeiro = fila.primeiro->proximo; // avanca o inicio para o proximo
        delete aux;                             // libera a memoria do item antigo
    }
}

#endif // FILA_H_INCLUDED
