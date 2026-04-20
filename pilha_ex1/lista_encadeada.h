template <typename TIPO>
struct TElementoE{
    TIPO dado;
    TElementoE<TIPO>*proximo;
};

template <typename TIPO>
struct TListaE{
    TElementoE<TIPO>*Inicio;
    int tamanho;
};

template<typename TIPO>
void inicializa_lista(TListaE<TIPO> &lista){
    lista.Inicio = NULL;
    lista.tamanho = 0;
}

// Função para criar um novo elemento da lista encadeada
template <typename TIPO>
TElementoE<TIPO> * novo_elemento_lista_de(TIPO dado){
    TElementoE<TIPO>*novoNo = new TElementoE<TIPO>;//cria a caixa
    novoNo->dado = dado;
    novoNo->proximo = NULL;
    return novoNo;
}

template <typename TIPO>
bool insere_inicio(TListaE<TIPO> &lista, TIPO dado){
    //cria a caixa
    TElementoE<TIPO>*novoNo = novo_elemento_lista_de(dado);
    //verifica falha na insercao
    if (novoNo == NULL) {
        return false;
    }
    //preenche caixa
    novoNo->proximo = lista.Inicio; // O próximo do novo nó é o atual início da lista
    lista.Inicio = novoNo; // Atualiza o início da lista para o novo nó 
    lista.tamanho++;
    return true;
}

template <typename TIPO>
bool remove_inicio(TListaE<TIPO> &lista) {
    if (lista.Inicio == NULL) {
        return false; // Lista já está vazia
    }
    // Guarda o primeiro nó
    TElementoE<TIPO>* noParaRemover = lista.Inicio;
    
    // O início pula para o segundo nó
    lista.Inicio = lista.Inicio->proximo;
    
    // Deleta o antigo primeiro nó da memória
    delete noParaRemover;
    lista.tamanho--;
    
    return true;
}

template <typename TIPO>
TIPO retorna_topo(TListaE<TIPO> &lista) {
    return lista.Inicio->dado;
}