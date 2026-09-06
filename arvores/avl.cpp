#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <regex>

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

void mostra_detalhado(no* n) {
    if (n == nullptr) return;

    // Exibe as informações do nó atual
    cout << "------------------------------------" << endl;
    cout << "Nó (Valor): " << n->valor << endl;
    cout << "FB: " << n->fb << endl;
    
    // Verifica se o nó possui pai (para não acessar memória inválida na Raiz)
    if (n->pai != nullptr) {
        cout << "Pai: " << n->pai->valor << " (FB do pai: " << n->pai->fb << ")" << endl;
    } else {
        cout << "Pai: Nenhum (Este nó é a RAIZ)" << endl;
    }
    cout << "Quantidade: " << n->contador << endl;

    // Chama recursivamente para percorrer toda a árvore
    mostra_detalhado(n->filho_esquerda);
    mostra_detalhado(n->filho_direita);
}

no* cria_no(){//pra quando inicializo uma árvore vazia
    no* novo = new no;
    novo->fb = 0;
    novo->contador = 1;
    novo->filho_esquerda = nullptr;
    novo->filho_direita = nullptr;
    return novo;
}

no* cria_arv_vazia(){
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

int altura(no* n){ //calcular a altura da árvore
    int e, d;
    if(n == nullptr)
        return 0;
    e = altura(n->filho_esquerda);
    d = altura(n->filho_direita);
    if(e > d)
        return e + 1; //altura sempre altura->filho + 1 
    else
        return d + 1; //altura sempre altura->filho + 1
}

void recalcula_fb(no* n){
    if (n == nullptr)
        return;
    n->fb = altura(n->filho_direita) - altura(n->filho_esquerda); //calcula da diferença de altura da arvore em relação ao N
    recalcula_fb(n->filho_esquerda);       //  recursivamente para percorrer toda arvore
    recalcula_fb(n->filho_direita);        //     ||      ||     ||      ||    ||
}


void reconecta(no* n, no* novo, no*& raiz){     //void para atualizar ponteiros, "novo" sendo o literal novo "centro e/ou raiz"
    novo->pai = n->pai;                         //"n" virando filho do "novo"
    if(n->pai != nullptr)   // se "n" não for raiz
        if(n->pai->filho_esquerda == n){
            n->pai->filho_esquerda = novo;
            novo->lado = 1;
        }
        else {
            n->pai->filho_direita = novo;
            novo->lado = -1;
        }
    else{  //caso "n" for a raiz
        raiz = novo;
        novo->lado = 0;
    }
}



void rotaciona_direita(no* n, no*& raiz){       
    no* aux = n->filho_esquerda;
    
    n->filho_esquerda = aux->filho_direita;
    if(aux->filho_direita != nullptr){
        aux->filho_direita->pai = n;
        aux->filho_direita->lado = 1;
    }
    
    aux->filho_direita = n;
    reconecta(n, aux, raiz);    //procedimento acima criado para despoluir o codigo da rotação, mudar dps (ou nao)
    n->pai = aux;
    n->lado = -1;
    

    recalcula_fb(n);    // mantendo o fator de balanceamento sempre atualizado
    recalcula_fb(aux);  //    ||       ||     ||     ||     ||    ||       ||
}


void rotaciona_esquerda(no* n, no*& raiz){  
    no* aux = n->filho_direita;
    
    n->filho_direita = aux->filho_esquerda;
    if(aux->filho_esquerda != nullptr){
        aux->filho_esquerda->pai = n;
        aux->filho_esquerda->lado = -1;
    }
    
    aux->filho_esquerda = n;
    reconecta(n, aux, raiz);        //procedimento acima criado para despoluir o codigo da rotação, mudar dps (ou nao)
    n->pai = aux;
    n->lado = 1;
    
    recalcula_fb(n);    // mantendo o fator de balanceamento sempre atualizado
    recalcula_fb(aux);  //    ||       ||     ||     ||     ||    ||       ||
}


void balancear(no* n, no*& raiz){   
    if (n == nullptr)
        return;
        
    recalcula_fb(raiz);          //calculando fator de balanceamento recursivamente para evitar valores legados
    
    if (n->fb == -2){                                                      //caso a arvore esteja desbalanceada para a ESQUERDA
        if(n->filho_esquerda !=nullptr && n->filho_esquerda->fb > 0){      //Caso ED (Centro desbalanceado para esquerda
            rotaciona_esquerda(n->filho_esquerda, raiz);                   //         filho desbalanceado para direita)
        }                                                                  //         necessidade de dupla-rotação
        
        rotaciona_direita(n, raiz);                                        //Caso E (árvore totalmente desbalanceada para esquerda)                                   
       
    }
    else if (n->fb == 2){                                                  //caso a árvore esteja desbalanceada para a DIREITA
        if(n->filho_direita !=nullptr && n->filho_direita->fb < 0){        //Caso DE (Centro desbalanceado para esquerda
            rotaciona_direita(n->filho_direita, raiz);                     //         filho desbalanceado para direita)
        }                                                                  //         necessidade de dupla-rotação
        
        rotaciona_esquerda(n, raiz);                                       //Caso D (árvore totalmente desbalanceada para direita) 
       
    }
    
    if(n->pai != nullptr)
        balancear(n->pai, raiz); // verificação para saber se o balanceamento anterior não modificou nós anteriores
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


void insere(int valor, no* &n, no*& raiz){ // n é o primeiro nó da árvore em qualquer situação
    if (arvore_vazia(n)){
        n = cria_no();
        n->pai = nullptr;
        n->valor = valor;
        raiz = n;
    }else {
        if (valor == n->valor) {
            (n->contador)++;
        }
        else if (valor > n->valor) {
            if (n->filho_direita == nullptr) {
                n->filho_direita = cria_no();
                n->filho_direita->pai = n;
                n->filho_direita->valor = valor;
                n->filho_direita->lado = -1;
                altera_fatores(n, n->filho_direita->lado);
                balancear(n, raiz);
                return;
            }
            insere(valor, n->filho_direita, raiz);            
        }else {
            if (n->filho_esquerda == nullptr) {
                n->filho_esquerda = cria_no();
                n->filho_esquerda->pai = n;
                n->filho_esquerda->valor = valor;
                n->filho_esquerda->lado = 1;
                altera_fatores(n, n->filho_esquerda->lado);
                balancear(n, raiz);
                return;
            }
            insere(valor, n->filho_esquerda, raiz);
        }

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
        << "FB: " << aux->fb << endl
        << "Qts vezes: \n" << aux->contador << endl;

        if (aux->pai != nullptr) {
            cout << "Pai: " << aux->pai->valor << endl
            << "FB do pai: \n" << aux->pai->fb << endl << endl;
        }
        aux = aux->filho_esquerda;
    }
}

int main(){
    no *raiz = cria_arv_vazia();
    int v;
    cout << "Insira um valor: ";
    cin >> v;
    while (v != -1){
        insere(v, raiz, raiz);
        cout << "\n=== ESTADO ATUAL DA ÁRVORE ===" << endl;
        mostra_detalhado(raiz);
        cout << "==============================\n" << endl;
        
        cout << "Insira um valor (-1 para sair): ";
        cin >> v;
     //   mostra(raiz);
     //   cout << "Insira um valor: " << endl;
     //   cin >> v;        
    }
}