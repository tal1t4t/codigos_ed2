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
    2- o segundo ponteiro aponta para o próximo nó da árvore B+ (ou para o filho se não for folha)
    3- os ponteiros restantes são os intermediários dos valores no nó
    Assim, sempre teremos uma árvore com grau + 1 ponteiros! */
    vector<No*> ponteiros;

    vector<dados> chaves; // aqui teremos grau - 1 chaves
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

bool e_folha(no* n, int GRAU){
    for (int i = 2; i < GRAU; i++) {
        if (n->ponteiros[i] !=nullptr) {
            return false;
        }
    }
    return true;
}

bool no_cheio(no* n, no* raiz, const int GRAU){
    for (int i = 0; i < GRAU - 1; i++) {
        if (n->chaves[i].palavra == ""){
            return false;
        }
    } 
    return true;   
}

void balancear(no* &n, no* &raiz, const int GRAU, string valor, vector<int> linha){
    if (e_folha(n, GRAU)) {
        no* aux1 = new no;
        no* aux2 = new no;
        int i;
        bool entrada_foi = false;
        // distribuindo os valores nas folhas novas
        for (i = 0; i < GRAU - 1; i++) {
            if (i < GRAU/2) {
                if (valor < n->chaves[i].palavra) {
                    aux1->chaves[i].palavra = valor;
                    aux1->chaves[i].linhas = linha;
                    aux1->chaves[i].contador = 1;

                    entrada_foi = true;
                    i++;
                }
                aux1->chaves[i].palavra = n->chaves[i].palavra;
                aux1->chaves[i].linhas = n->chaves[i].linhas;
                aux1->chaves[i].contador = n->chaves[i].contador;
            }
            else {
                if (valor < n->chaves[i].palavra) {
                    aux2->chaves[abs(GRAU - i - GRAU/2)].palavra = valor;
                    aux2->chaves[GRAU - i - GRAU/2].linhas = linha;
                    aux2->chaves[GRAU - i - GRAU/2].contador = 1;

                    entrada_foi = true;
                    i++;
                }
                aux2->chaves[abs(GRAU - i - GRAU/2)].palavra = n->chaves[GRAU - i - GRAU/2].palavra;
                aux2->chaves[abs(GRAU - i - GRAU/2)].linhas = n->chaves[GRAU - i - GRAU/2].linhas;
                aux2->chaves[abs(GRAU - i - GRAU/2)].contador =  n->chaves[GRAU - i - GRAU/2].contador;
            }

            if (i == GRAU - 2 && !entrada_foi) {
                aux2->chaves[abs(GRAU - i - GRAU/2)].palavra = valor;
                aux2->chaves[abs(GRAU - i - GRAU/2)].linhas = linha;
                aux2->chaves[GRAU - i - GRAU/2].contador = 1;
            }
        }

    }
    else {
        if (GRAU % 2 == 0) {
            balancear(n->ponteiros[0], raiz, GRAU, n->chaves[GRAU - GRAU/2].palavra, n->chaves[GRAU - GRAU/2].linhas);
        
        }
        else {
            balancear(n->ponteiros[0], raiz, GRAU, n->chaves[GRAU - GRAU/2 + 1].palavra, n->chaves[GRAU - GRAU/2 + 1].linhas);
        }
        /* ordenar os nós intermediários (usar função sort da biblioteca algorithm)

            se o nó estiver cheio, sobe com o menor valor da segunda metade para balancear em cima
            e apaga do nó atual(só repete na folha)

            ajustar os ponteiros ao ordenar
        */
    }
}

string limpaPalavra(string palavra){
    string limpa = "";

    for (char c : palavra) {

        if (isalnum(static_cast<unsigned char>(c))) {

            limpa += tolower(
                static_cast<unsigned char>(c)
            );
        }
    }

    return limpa;
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

void insere(string valor, int linha, no*& n, no*& raiz, const int GRAU){
    if (arvore_vazia(n)) {
        n = cria_no();

        n->chaves[0].palavra = valor;
        n->chaves[0].contador = 1;
        n->chaves[0].linhas.push_back(linha);

        return;
    }
    else if(no_cheio(n, raiz, GRAU)){
        vector<int> linhas;
        linhas.push_back(linha);
        balancear(n, raiz, GRAU, valor, linhas);
    }
    else {//ainda tem espaço pra inserir no nó
        /*
            ordenar nó ao inserir 
        */
        return;
    }

}


// ======================================================
// LEITURA DO ARQUIVO
// ======================================================

void leituraArquivo(string nomeArquivo, no*& raiz, const vector<string>& exclusao, const int GRAU){
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
                        insere(palavra, numeroLinha, raiz, raiz, GRAU);
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
    int num;
    cout << "Digite o grau da árvore(3 a 6): " << endl;
    cin >> num;

    while (num != 3 && num != 4 && num != 5 && num != 6) {
        cout << "grau inválido." << endl <<
                "Escolha um grau de 3 a 6 para sua arvore: ";
        cin >> num;
    }

    int const GRAU = num;

    no* n = cria_arv_vazia();
    n->ponteiros.resize(GRAU + 1);
    n->chaves.resize(GRAU - 1);

    // ----------------------------------
    // CARREGA LISTA DE EXCLUSÃO
    // ----------------------------------

    vector<string> exclusao = carregaExclusao("exclusao.txt");
    // ----------------------------------
    // LÊ O TEXTO E MONTA A B+
    // ----------------------------------

    leituraArquivo("texto.txt", n, exclusao, GRAU);
   
    //mostra(n);
    cout << "Arquivo processado!"<< endl
        << "Arvore B+ com grau " << GRAU << "criada."<< endl;

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