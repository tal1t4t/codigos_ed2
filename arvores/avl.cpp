#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <regex>
#include <fstream>
#include <sstream>
#include<cctype>

using namespace std;
typedef struct item{
    int lado; //-1 para direita e 1 para esquerda
    struct item* pai;
    string palavra;
    vector<int> linhas; //funciona tipo um deque. é um "tipo" que já cria sozinho e permite inserir ou excluir no inicio ou no fim
    int fb;
    int contador;
    struct item* filho_direita;
    struct item* filho_esquerda;
}no;

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


void insere(string palavra, int linha, no* &n){ // n é o primeiro nó da árvore em qualquer situação
    if (arvore_vazia(n)){
        n = cria_no();
        n->pai = nullptr;
        n->palavra = palavra;
        n->linhas.push_back(linha); //push_back insere no final (cria o vetor 1, 2, 3...) mostrando em quais linhas a palavra aparece
    }else {
        if (palavra == n->palavra) {
            (n->contador)++;
            if (n->linhas.back() != linha) { //confere se a ultima linha que a palavra apareceu é a mesma ou não, para não repetir (1, 1) indicando que está na mesma linha
                n->linhas.push_back(linha);
             }
        }
        else if (palavra > n->palavra) {
            if (n->filho_direita == nullptr) {
                n->filho_direita = cria_no();
                n->filho_direita->pai = n;
                n->filho_direita->palavra = palavra;
                n->filho_direita->linhas.push_back(linha);
                n->filho_direita->lado = -1;
                altera_fatores(n, n->filho_direita->lado);
                return;
            }
            insere(palavra, linha, n->filho_direita);            
        }else {
            if (n->filho_esquerda == nullptr) {
                n->filho_esquerda = cria_no();
                n->filho_esquerda->pai = n;
                n->filho_esquerda->palavra = palavra;
                n->filho_esquerda->linhas.push_back(linha);
                n->filho_esquerda->lado = 1;
                altera_fatores(n, n->filho_esquerda->lado);
                return;
            }
            insere(palavra, linha, n->filho_esquerda);
        }

    }

}

//busca a palavra na árvore
no* busca(no* n, string palavra)
{
    if (n == nullptr) {
        return nullptr;
    }

    if (palavra == n->palavra) {
        return n;
    }

    if (palavra < n->palavra) {
        return busca(n->filho_esquerda, palavra);
    }

    return busca(n->filho_direita, palavra);
}


string limpaPalavra(string palavra) {
    for (char &c : palavra) { //percorre a palavra e transforma em maiusculo
        c = toupper(c);
    }

    for (int i = 0; i < palavra.size(); i++) { 
        if (ispunct(palavra[i])) { //verifica se há caractetes especiais
            palavra.erase(i, 1); //apaga os caracteres especiais 
            i--; //corrige a posição que "sobra" após apagar um caractere especial
        }
    }

    return palavra;
}


bool e_excluida (string palavra_exc, const vector<string>& exclusao) {
    return binary_search(exclusao.begin(), exclusao.end(), palavra_exc); //binary_serch faz a busca binaria do começo de exclusao até o fim
}



//lê o arquivo e transforma a formatação em maíscula
void leituraArquivo(string nomeArquivo, no* &raiz, const vector<string>& exclusao) {
    ifstream arquivo(nomeArquivo); //ifstream passa para a variável arquivo o valor de nomeArquivo (que representa o txt)

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    string linhaTexto;
    string palavra;
    int numeroLinha = 1;

    while (getline(arquivo, linhaTexto)) {  // stringstream transforma a linha em um fluxo de dados. Isso permite extrair cada palavra separadamente
        stringstream ss(linhaTexto);

        while (ss >> palavra) { //usando o operador >>

            palavra = limpaPalavra(palavra);

            if (!e_excluida(palavra, exclusao)) {
                insere(palavra, numeroLinha, raiz);
            }
        }

        numeroLinha++;
    }

    arquivo.close();
    
}

vector<string> carregaExclusao(string nomeArquivo) {
    vector <string> exclusao;
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
     cout << "Erro ao abrir o arquivo!" << endl;
     return exclusao;
    }
    string palavra_exc;
    while (arquivo >> palavra_exc)
    {
        palavra_exc = limpaPalavra(palavra_exc);
        exclusao.push_back(palavra_exc);
    }

    sort(exclusao.begin(), exclusao.end());

    arquivo.close();

    return exclusao;
    
}


int main() {

    no* n = cria_arv_vazia();
    string palavraConsulta;
    no* resultado = nullptr;
    char continuar = 's';
     vector<string> exclusao = carregaExclusao("exclusao.txt");
    
     // Processa o arquivo e monta a árvore
    leituraArquivo("texto.txt", n, exclusao);

    // Parte responsável pelas consultas
    while (continuar == 's' || continuar == 'S')
    {
        cout << "\n--- Busca de Palavras na Arvore ---" << endl;
        cout << "Digite uma palavra (ou -1 para sair): ";
        cin >> palavraConsulta;

        if (palavraConsulta == "-1") {
            cout << "Saindo do programa..." << endl;
            break;
        }

        // Limpa a palavra digitada pelo usuário (tira os pontos, vírgulas, hífens, etc)
        palavraConsulta = limpaPalavra(palavraConsulta);

        // Procura na árvore
        resultado = busca(n, palavraConsulta);

        if (resultado == nullptr) {
            cout << "Palavra nao encontrada. Tente novamente." << endl;
        }
        else {
            cout << "Palavra encontrada!" << endl;
            cout << "Ocorrencias: " << resultado->contador << endl;
            cout << "Linha(s): ";

            for (size_t i = 0; i < resultado->linhas.size(); i++) {
                if (i == 0) {
                    cout << resultado->linhas[i];
                }
                else {
                    cout << ", " << resultado->linhas[i];
                }
            }

            cout << endl;

            cout << "\nDeseja procurar outra palavra? (s/n): ";
            cin >> continuar;
        }
    }

    return 0;
}