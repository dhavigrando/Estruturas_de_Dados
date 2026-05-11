/* Estruturas de Dados  - Trabalho M2 - GRUPO A
   Integrantes: Dhavi Grando, Felipe Scremin, Matheus Menozzo */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Fila.h"
#include "TAD_Fila.h"
using namespace std;

int main() {
    srand(time(NULL)); // inicializa o gerador de numeros aleatorios com a hora atual

    int qtd_socios, qtd_normais, carga_inicial, qtd_por_tempo, tempo_simulacao;

    cout << "Simulacao de Atendimento no Estadio \n\n";
    cout << "Quantidade de guiches socio-torcedor: ";
    cin >> qtd_socios;
    cout << "Quantidade de guiches normal: ";
    cin >> qtd_normais;
    cout << "Quantidade de pessoas aguardando antes da simulacao (carga inicial): ";
    cin >> carga_inicial;
    cout << "Quantidade de pessoas que procuram os guiches a cada unidade de tempo: ";
    cin >> qtd_por_tempo;
    cout << "Quantidade de tempo em que o modelo sera simulado: ";
    cin >> tempo_simulacao;
    cin.ignore(); // limpa o '\n' que ficou no buffer apos a ultima leitura

    // aloca os arrays de guiches dinamicamente de acordo com as quantidades informadas
    // se a quantidade for zero, nao aloca (NULL indica que esse tipo nao existe)
    Guiche* guiches_socios  = (qtd_socios  > 0) ? new Guiche[qtd_socios]  : NULL;
    Guiche* guiches_normais = (qtd_normais > 0) ? new Guiche[qtd_normais] : NULL;

    // inicializa cada guiche socio com fila vazia e sem ninguem em atendimento
    for (int i = 0; i < qtd_socios; i++) {
        inicializa_fila(guiches_socios[i].fila);
        guiches_socios[i].ocupado = false;
        guiches_socios[i].tempo_atendimento_restante = 0;
    }
    // inicializa cada guiche normal da mesma forma
    for (int i = 0; i < qtd_normais; i++) {
        inicializa_fila(guiches_normais[i].fila);
        guiches_normais[i].ocupado = false;
        guiches_normais[i].tempo_atendimento_restante = 0;
    }

    // insere a carga inicial de pessoas nas filas antes de comecar a simulacao
    for (int i = 0; i < carga_inicial; i++) {
        Pessoa p = nova_pessoa();
        inserir_pessoa(p, guiches_socios, qtd_socios, guiches_normais, qtd_normais);
    }

    // acumuladores para calcular a media de torcedores por fila ao final
    double soma_socios  = 0, soma_normais  = 0;
    int contagem_socios = 0, contagem_normais = 0;

    // loop principal da simulacao: executa uma unidade de tempo por iteracao
    for (int tempo = 1; tempo <= tempo_simulacao; tempo++) {
        cout << "\n Unidade de tempo: " << tempo << " ---\n";

        // 1. processa o atendimento: avanca o tempo de quem esta sendo atendido
        //    e libera o guiche quando o atendimento termina
        processar_atendimento(guiches_socios,  qtd_socios);
        processar_atendimento(guiches_normais, qtd_normais);

        // 2. novas pessoas chegam e entram na menor fila disponivel do seu tipo
        for (int i = 0; i < qtd_por_tempo; i++) {
            Pessoa p = nova_pessoa();
            inserir_pessoa(p, guiches_socios, qtd_socios, guiches_normais, qtd_normais);
        }

        // 3. exibe o estado de todas as filas e acumula dados para a media final
        if (qtd_socios > 0)
            exibir_filas(guiches_socios,  qtd_socios,  "socio-torcedor", soma_socios,  contagem_socios);
        if (qtd_normais > 0)
            exibir_filas(guiches_normais, qtd_normais, "normal",         soma_normais, contagem_normais);

        // aguarda o usuario pressionar Enter para avancar para a proxima unidade de tempo
        cout << "Pressione Enter para continuar...";
        cin.get();
    }

    // exibe o resultado final com a media de torcedores esperando por fila
    cout << "\n Resultado Final \n";
    if (contagem_socios > 0) {
        cout << "Media de torcedores esperando por fila (socio): ";
        imprimir_media(soma_socios / contagem_socios);
        cout << "\n";
    }
    if (contagem_normais > 0) {
        cout << "Media de torcedores esperando por fila (normal): ";
        imprimir_media(soma_normais / contagem_normais);
        cout << "\n";
    }

    // libera toda a memoria alocada dinamicamente antes de encerrar
    delete[] guiches_socios;
    delete[] guiches_normais;

    return 0;
}
