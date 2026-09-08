#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct dados{
    string palavra;
    int contador;
    vector<int> linhas;
}dado;

typedef struct No{
    /* O vetor 'ponteiros' segue a seguinte regra:
    1- o primeiro ponteiro é o que aponta para o pai
    2- o segundo ponteiro aponta para o próximo nó da árvore B+
    3- os ponteiros restantes são os intermediários dos valores no nó
    Assim, sempre teremos uma árvore com grau + 1 ponteiros! */
    vector<No*> ponteiros;

    vector<dados> palavras; // aqui teremos grau - 1 palavras
}no;

no* cria_arv_vazia(){
    return nullptr;
}

bool arvore_vazia(no* raiz){
    return raiz == nullptr;
}

no* cria_no(){
    no* novo = new no;
    return novo;
}

string limpaPalavra(string palavra){
    return 0; // mudar aqui só quando a função estiver consertada
}

no* busca(no* n, const string& valor){
    return 0; // definir pra b+!!
}

vector<string> carregaExclusao(string nomeArquivo){

    vector<string> exclusao;

    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {

        cout << "Erro ao abrir o arquivo de exclusao!"
             << endl;

        return exclusao;
    }

    string palavra_exc;

    while (arquivo >> palavra_exc) {

        palavra_exc = limpaPalavra(palavra_exc);

        if (!palavra_exc.empty()) {
            exclusao.push_back(palavra_exc);
        }
    }

    sort(exclusao.begin(), exclusao.end());
    arquivo.close();
    return exclusao;
}

bool palavraExcluida(const vector<string>& exclusao, const string& palavra) {

    return binary_search(exclusao.begin(), exclusao.end(), palavra);
}

void insere(string valor, int linha, no*& n, no*& raiz){
    if (arvore_vazia(n)) {
        n = cria_no();

        n->palavras[0].palavra = valor;
        n->palavras[0].contador = 1;
        n->palavras[0].linhas.push_back(linha);

        return;
    }
    else {
        for (int i = 0; i < 3; i++) {
        
        }
    }

}


// ======================================================
// LEITURA DO ARQUIVO
// ======================================================

void leituraArquivo(string nomeArquivo, no*& raiz, const vector<string>& exclusao){
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {

        cout << "Erro ao abrir o arquivo de texto!"
             << endl;

        return;
    }

    string linhaTexto;

    int numeroLinha = 1;

    // Lê linha por linha
    while (getline(arquivo, linhaTexto)) {

        string palavra = "";

        /*
            Percorre cada caractere da linha.

            Quando encontra espaço ou pontuação,
            termina a palavra atual.
        */

        for (size_t i = 0; i <= linhaTexto.size(); i++) {

            if (i < linhaTexto.size() && isalnum(static_cast<unsigned char>(linhaTexto[i]))) {

                palavra += tolower(static_cast<unsigned char>(linhaTexto[i]));
            }

            else {

                if (!palavra.empty()) {
                    // Verifica se NÃO é palavra de exclusão
                    if (!palavraExcluida(exclusao, palavra)) {
                        // já insere as palavras na B+
                        insere(palavra, numeroLinha, raiz, raiz);
                    }
                    palavra = "";
                }
            }
        }
        numeroLinha++;
    }

    arquivo.close();
}

int main(){
    no* n = cria_arv_vazia();
    // ----------------------------------
    // CARREGA LISTA DE EXCLUSÃO
    // ----------------------------------

    vector<string> exclusao = carregaExclusao("exclusao.txt");
    // ----------------------------------
    // LÊ O TEXTO E MONTA A B+
    // ----------------------------------

    leituraArquivo("texto.txt", n, exclusao);
   
    //mostra(n);
    cout << "Arquivo processado!"<< endl
        << "Arvore B+ criada."<< endl;

    // ----------------------------------
    // CONSULTAS
    // ----------------------------------

    string palavraConsulta;

    char continuar = 's';

    while (
        continuar == 's' ||
        continuar == 'S'
    ) {

        cout << "\n--- Busca de Palavras na Arvore ---"<< endl;
        cout << "Digite uma palavra "<< "(ou -1 para sair): "; 
        cin >> palavraConsulta;

        if (palavraConsulta == "-1") {
            cout << "Saindo do programa..." << endl;

            break;
        }

        palavraConsulta = limpaPalavra(palavraConsulta);

        // ----------------------------------
        // BUSCA NA AVL
        // ----------------------------------

        auto inicio = steady_clock::now(); //steady_clock::now marca o momento de agora no cronômetro

        no* resultado = busca(n, palavraConsulta);

        auto fim = steady_clock::now();

        auto duracao = fim - inicio;

        int duracao_nanoseg = duration_cast<nanoseconds>(duracao).count();
        // conta a duração de tempo da execução em microssegundos

        if (resultado == nullptr) {
            cout << "Palavra nao encontrada."<< endl;
        }

        else {

            cout << "\nPalavra encontrada!"<< endl;

            cout << "Ocorrencias: "<< resultado->contador << endl;

            cout << "Linha(s): ";

            for (size_t i = 0; i < resultado->linhas.size(); i++) {

                if (i > 0) {
                    cout << ", ";
                }

                cout << resultado->linhas[i];
            }
            cout << endl;
        }
        cout << "Duração da busca: " << duracao_nanoseg << " nanossegundos." << endl << endl
            << "\nDeseja procurar outra palavra? (s/n): ";
        cin >> continuar;
    }



    return 0;
}