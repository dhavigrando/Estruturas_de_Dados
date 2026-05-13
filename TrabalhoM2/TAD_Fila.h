#ifndef TAD_FILA_H_INCLUDED
#define TAD_FILA_H_INCLUDED
#include <iostream>
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
    int menor = tamanho_fila(guiches[0].fila) + guiches[0].tempo_atendimento_restante;
    for (int j = 1; j < qtd; j++) {
        int tam = tamanho_fila(guiches[j].fila) + guiches[j].tempo_atendimento_restante;
        if (tam < menor) {
            menor = tam;
            indice_menor = j;
        }
    }
    return indice_menor;
}

// gera uma nova pessoa com tipo (socio/normal) e tempo de atendimento
// usando contadores ciclicos passados por referencia para respeitar as proporcoes do enunciado:
// a cada 20 pessoas: 1 e socio (5%) e 19 sao normais (95%)
Pessoa nova_pessoa(int& contador_pessoas, int& contador_socios, int& contador_normais) {
    contador_pessoas++;

    Pessoa p;

    // a cada 20 pessoas, a de numero 20 e socio (5%), as demais sao normais (95%)
    p.eh_socio = (contador_pessoas % 20 == 0);

    if (p.eh_socio) {
        contador_socios++; // incrementa o contador de socios
        // a cada 20 socios: os 17 primeiros levam 1 unidade (85%), os 3 seguintes levam 2 (15%)
        int ciclo = contador_socios % 20; // valor de 0 a 19
        if (ciclo < 17) {
            p.tempo_atendimento = 1;
        } else {
            p.tempo_atendimento = 2;
        }
    } else {
        contador_normais++; // incrementa o contador de normais
        // a cada 20 normais: 5 levam 1 unidade (25%), 6 levam 2 (30%), 9 levam 3 (45%)
        int ciclo = contador_normais % 20; // valor de 0 a 19
        if (ciclo < 5) {
            p.tempo_atendimento = 1;
        } else if (ciclo < 11) {
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
            entrar_na_fila(guiches_socios[menor_fila(guiches_socios, qtd_socios)].fila, p);
        } else if (qtd_normais > 0) {
            // nao ha guiche socio, entao vai para o normal
            entrar_na_fila(guiches_normais[menor_fila(guiches_normais, qtd_normais)].fila, p);
        }
        // se nao ha nenhum guiche disponivel, a pessoa nao e inserida
    } else {
        if (qtd_normais > 0) {
            // entra na fila do guiche normal com menor espera
            entrar_na_fila(guiches_normais[menor_fila(guiches_normais, qtd_normais)].fila, p);
        } else if (qtd_socios > 0) {
            // nao ha guiche normal, entao vai para o socio
            entrar_na_fila(guiches_socios[menor_fila(guiches_socios, qtd_socios)].fila, p);
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
            guiches[j].pessoa_em_atendimento = atender(guiches[j].fila); // remove da fila
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
            const char* tipo_atendimento;
            if (guiches[j].pessoa_em_atendimento.eh_socio) {
                tipo_atendimento = "socio";
            } else {
                tipo_atendimento = "normal";
            }
            cout << "[EM ATENDIMENTO: " << tipo_atendimento
                 << ", " << guiches[j].tempo_atendimento_restante << "t restante] ";
        } else {
            cout << "[livre] "; // guiche nao esta atendendo ninguem
        }

        // percorre a fila e mostra cada pessoa que esta esperando
        TItem<Pessoa>* aux = guiches[j].fila.primeiro;
        while (aux != NULL) {
            const char* tipo_espera;
            if (aux->dado.eh_socio) {
                tipo_espera = "socio";
            } else {
                tipo_espera = "normal";
            }
            cout << "[" << tipo_espera << ", " << aux->dado.tempo_atendimento << "t] ";
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
