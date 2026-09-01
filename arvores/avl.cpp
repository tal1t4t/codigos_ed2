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
    struct item* filho_direita;
    struct item* filho_esquerda;
}no;

no* cria_no(){//pra quando inicializo uma árvore vazia
    no* novo = new no;
    novo->fb = 0;
    novo->pai = NULL;
    novo->filho_esquerda = NULL;
    novo->filho_direita = NULL;
    return novo;
}

no* cria_arv_vazia(){
    return NULL;
}

int arvore_vazia(no* n){
    return n == NULL;
}

int e_folha(no* n){
    return n->filho_direita == NULL && n->filho_esquerda == NULL;
}

void altera_fatores(no* n, int lado){
 if (n->pai == NULL) {
    if (lado == -1) {
        (n->fb)--;
    }
    else {
        (n->fb)++;
    }
 }
 else {
    altera_fatores(n->pai, int n->lado);
 }
}

void procura_pai(no* n, no* novo){
    no* atual = n;
    while (true) {
        if (novo->valor >= atual->valor) {
            if (atual->filho_direita != NULL) {
                atual = atual->filho_direita;
            }
            else {
                atual->filho_direita = novo;
                novo->lado = -1;
                (atual->fb)--;
                break;
            }
        }
        else {
            if (atual->filho_esquerda != NULL) {
                atual = atual->filho_esquerda;
            }
            else {
                atual->filho_esquerda = novo;   
                novo->lado = 1;
                (atual->fb)++;
                break;        
            }
        }
    }
    novo->pai = atual;

    altera_fatores(novo->pai, novo->lado);
}

// void balancear(no* n){

// }

// int fb(no* item){
//     if (e_folha(item)) {
//         item->fb = 0;
//         return 0;
//     }
//     return 1 + max(fb(item->filho_direita), fb(item->filho_esquerda));
// }

void insere(int valor, no* &n){ // n é o primeiro nó da árvore em qualquer situação
    if (arvore_vazia(n)){
        n = cria_no();
        n->valor = valor;
        cout << "valor " << valor << " inserido" << endl
        << "Altura = " << n->fb << endl;
    }else {
        no* novo = new no;
        novo->valor = valor;
        novo->filho_direita = NULL;
        novo->filho_esquerda = NULL;
        novo->fb = 0;
        procura_pai(n, novo);
        // novo->fb = fb(novo);

        cout << "valor " << valor << " inserido" << endl
        << "Pai: " << novo->pai->valor << endl
        << "FB: " << novo->fb << endl
        << "FB do pai: " << novo->pai->fb << endl;


        // cout << "valor " << valor << " inserido" << endl
        // << "Altura = " << novo->altura << endl
        // << "Pai: " << novo->pai->valor << endl;
        // balancear(n);
    }


}

// void mostra(no* n){
//     if (e_folha(n)) {
//         cout << "valor " << n->valor << endl
//         << "Altura = " << n->altura << endl
//         << "Pai: " << (n->pai)->valor << endl;
//     }
//     if (n->filho_esquerda != NULL) {
//         mostra(n->filho_esquerda);
//     }else {
//         mostra(n->filho_direita);
//     }

// }

int main(){
    no* n = cria_arv_vazia();
    int v;
    cout << "Insira um valor: ";
    cin >> v;
    while (v != -1){
        insere(v, n);
        cout << "Insira um valor: " << endl;
        cin >> v;        
        // mostra(n);
    }
}