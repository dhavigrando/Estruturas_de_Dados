#include <iostream>
#include "lista_encadeada.h"
using namespace std;

bool verifica_string(string a){
    
    TListaE<char> pilha;
    inicializa_lista(pilha);
    
    for(int i=0; i<a.size(); i++){
        char c=a[i];
        if(c=='('||c=='{'||c=='['){
            insere_inicio(pilha, c);
        }
        if(c==')' || c=='}' || c==']'){
            if(pilha.Inicio == NULL){
                return false;
            }
            char topo = retorna_topo(pilha);
            if((c==')' && topo!='(') ||
               (c=='}' && topo!='{') ||
               (c==']' && topo!='[')){
                return false;
            }
            remove_inicio(pilha);
        }
    }
    if(pilha.Inicio != NULL){
        return false;
    }
    return true;
};

int main(){
    string codigo;
    
    cout<<"Digite a expressâo: ";
    cin>>codigo;
    
    
    if(verifica_string(codigo)){
        cout<<"Correto";
    }else{
        cout<<"Errado";
    }
}