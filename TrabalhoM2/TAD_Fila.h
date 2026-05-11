/* Estruturas de Dados (23272) - Trabalho M2
   Integrantes: Dhavi Grando, Felipe Scremin, Matheus Menozzo */

#ifndef TAD_FILA_H_INCLUDED
#define TAD_FILA_H_INCLUDED
#include <iostream>
#include <cstdlib>
#include "Fila.h"
using namespace std;

// representa uma pessoa que vai ser atendida nos guiches
struct Pessoa {
    int tempo_atendimento; // quantas unidades de tempo essa pessoa leva para ser atendida
    bool eh_socio;         // true = socio-torcedor, false = torcedor normal
};

// representa um guiche de atendimento com sua fila de espera
struct Guiche {
    TFila<Pessoa> fila;             // fila de pessoas esperando para ser atendidas
    int tempo_atendimento_restante; // quanto tempo falta para terminar o atendimento atual
    bool ocupado;                   // indica se o guiche esta atendendo alguem agora
    Pessoa pessoa_em_atendimento;   // dados da pessoa que esta sendo atendida no momento
};

// percorre os guiches e retorna o indice do que tem a menor fila
// assim a proxima pessoa sempre entra no guiche mais rapido
int menor_fila(Guiche* guiches, int qtd) {
    int indice_menor = 0;
    int menor = tamanho_fila(guiches[0].fila); // comeca assumindo que o primeiro e o menor
    for (int j = 1; j < qtd; j++) {
        int tam = tamanho_fila(guiches[j].fila);
        if (tam < menor) { // encontrou fila menor, atualiza o indice
            menor = tam;
            indice_menor = j;
        }
    }
    return indice_menor;
}

// gera uma nova pessoa com tipo (socio/normal) e tempo de atendimento aleatorios
// seguindo as probabilidades definidas no enunciado
Pessoa nova_pessoa() {
    Pessoa p;
    int tipo = rand() % 100;
    p.eh_socio = (tipo < 5); // 5% de chance de ser socio-torcedor

    if (p.eh_socio) {
        // socio: 85% leva 1 unidade de tempo, 15% leva 2
        int t = rand() % 100;
        if (t < 85) {
            p.tempo_atendimento = 1;
        } else {
            p.tempo_atendimento = 2;
        }
    } else {
        // normal: 25% leva 1, 30% leva 2, 45% leva 3 unidades de tempo
        int t = rand() % 100;
        if (t < 25) {
            p.tempo_atendimento = 1;
        } else if (t < 55) {
            p.tempo_atendimento = 2;
        } else {
            p.tempo_atendimento = 3;
        }
    }
    return p;
}

// decide em qual guiche a pessoa vai entrar na fila
// socio vai para guiche socio; normal vai para guiche normal
// se o tipo certo nao existir, redireciona para o outro tipo disponivel
void inserir_pessoa(Pessoa p,
                    Guiche* guiches_socios,  int qtd_socios,
                    Guiche* guiches_normais, int qtd_normais) {
    if (p.eh_socio) {
        if (qtd_socios > 0) {
            // entra na fila do guiche socio com menor espera
            queue(guiches_socios[menor_fila(guiches_socios, qtd_socios)].fila, p);
        } else if (qtd_normais > 0) {
            // nao ha guiche socio, entao vai para o normal
            queue(guiches_normais[menor_fila(guiches_normais, qtd_normais)].fila, p);
        }
        // se nao ha nenhum guiche disponivel, a pessoa nao e inserida
    } else {
        if (qtd_normais > 0) {
            // entra na fila do guiche normal com menor espera
            queue(guiches_normais[menor_fila(guiches_normais, qtd_normais)].fila, p);
        } else if (qtd_socios > 0) {
            // nao ha guiche normal, entao vai para o socio
            queue(guiches_socios[menor_fila(guiches_socios, qtd_socios)].fila, p);
        }
    }
}

// processa um passo de atendimento em todos os guiches do array
// decrementa o tempo de atendimento e libera o guiche quando termina
void processar_atendimento(Guiche* guiches, int qtd) {
    for (int j = 0; j < qtd; j++) {
        if (guiches[j].ocupado) {
            guiches[j].tempo_atendimento_restante--; // passa uma unidade de tempo
            if (guiches[j].tempo_atendimento_restante == 0) {
                guiches[j].ocupado = false; // atendimento concluido, guiche fica livre
            }
        }
        // se o guiche ficou livre e ainda tem gente na fila, chama o proximo
        if (!guiches[j].ocupado && !fila_vazia(guiches[j].fila)) {
            guiches[j].pessoa_em_atendimento = dequeue(guiches[j].fila); // remove da fila
            guiches[j].tempo_atendimento_restante = guiches[j].pessoa_em_atendimento.tempo_atendimento;
            guiches[j].ocupado = true;
        }
    }
}

// exibe o estado atual de todos os guiches de um grupo (socio ou normal)
// tambem acumula o total de pessoas em espera para calcular a media no final
void exibir_filas(Guiche* guiches, int qtd, const char* label, double& soma, int& contagem) {
    cout << "Filas " << label << ":\n";
    for (int j = 0; j < qtd; j++) {
        int tam = tamanho_fila(guiches[j].fila);
        cout << "  Guiche " << j + 1 << " (" << tam << " na fila): ";

        // mostra quem esta sendo atendido agora e quanto tempo falta
        if (guiches[j].ocupado) {
            cout << "[EM ATENDIMENTO: "
                 << (guiches[j].pessoa_em_atendimento.eh_socio ? "socio" : "normal")
                 << ", " << guiches[j].tempo_atendimento_restante << "t restante] ";
        } else {
            cout << "[livre] "; // guiche nao esta atendendo ninguem
        }

        // percorre a fila e mostra cada pessoa que esta esperando
        TItem<Pessoa>* aux = guiches[j].fila.primeiro;
        while (aux != NULL) {
            cout << "["
                 << (aux->dado.eh_socio ? "socio" : "normal")
                 << ", " << aux->dado.tempo_atendimento << "t] ";
            aux = aux->proximo;
        }
        cout << "\n";

        soma += tam; // acumula tamanho da fila para calcular a media depois
        contagem++;  // conta quantas filas foram medidas
    }
}

// imprime um numero com duas casas decimais sem depender de <iomanip>
void imprimir_media(double valor) {
    int inteiro = (int)valor;
    int decimal = (int)((valor - inteiro) * 100 + 0.5);
    if (decimal < 10) {
        cout << inteiro << ".0" << decimal;
    } else {
        cout << inteiro << "." << decimal;
    }
}

#endif // TAD_FILA_H_INCLUDED
