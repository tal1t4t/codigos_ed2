#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>

using namespace std;
typedef struct item{
    int lado; //-1 para direita e 1 para esquerda
    struct item* pai;
    int valor;
    int fb;
    int contador;
    struct item* filho_direita;
    struct item* filho_esquerda;
}no;

typedef struct Arvore{
    no* raiz;
}arvore;

no* cria_no(){//pra quando inicializo uma árvore vazia
    no* novo = new no;
    novo->fb = 0;
    novo->pai = nullptr;
    novo->filho_esquerda = nullptr;
    novo->filho_direita = nullptr;
    return novo;
}

no* cria_arv_vazia(){
    return nullptr;
}

arvore* cria_raiz_vazia(){
    return nullptr;
}

int arvore_vazia(no* n){
    return n == nullptr;
}

int e_folha(no* n){
    return n->filho_direita == nullptr && n->filho_esquerda == nullptr;
}

void altera_fatores(no* n, int lado){
    if (lado == -1) {
        (n->fb)--;
    }
    else {
        (n->fb)++;
    }
    if (n->pai != nullptr) {
        altera_fatores(n->pai, n->lado);
    }
}

void procura_pai(no* n, no* novo){
    no* atual = n;
    while (true) {
        if (novo->valor >= atual->valor) {
            if (atual->filho_direita != nullptr) {
                atual = atual->filho_direita;
            }
            else {
                atual->filho_direita = novo;
                novo->lado = -1;
                break;
            }
        }
        else {
            if (atual->filho_esquerda != nullptr) {
                atual = atual->filho_esquerda;
            }
            else {
                atual->filho_esquerda = novo;   
                novo->lado = 1;
                break;        
            }
        }
    }
    novo->pai = atual;

    altera_fatores(novo->pai, novo->lado);
}

// void balancear(no* n){

// }


void insere(int valor, no* &n){ // n é o primeiro nó da árvore em qualquer situação
    if (arvore_vazia(n)){
        n = cria_no();
        n->valor = valor;
    }else {
        no* aux = n;
        



        no* novo = new no;
        novo->valor = valor;
        novo->filho_direita = nullptr;
        novo->filho_esquerda = nullptr;
        novo->fb = 0;
        procura_pai(n, novo);


        // novo->fb = fb(novo);

        // cout << "valor " << valor << " inserido" << endl
        // << "Pai: " << novo->pai->valor << endl
        // << "FB: " << novo->fb << endl
        // << "FB do pai: " << novo->pai->fb << endl;

        // balancear(n);
    }


}

void mostra(no* n){
    no* aux = n;
    while (aux != nullptr) {
        cout << "valor " << aux->valor << endl 
        << "FB: " << aux->fb << endl;

        if (aux->pai != nullptr) {
            cout << "Pai: " << aux->pai->valor << endl
            << "FB do pai: " << aux->pai->fb << endl << endl;
        }
        aux = aux->filho_esquerda;
    }
}

int main(){
    no* n = cria_arv_vazia();
    int v;
    cout << "Insira um valor: ";
    cin >> v;
    while (v != -1){
        insere(v, n);
        mostra(n);
        cout << "Insira um valor: " << endl;
        cin >> v;        
    }
}