#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;


typedef struct item {
    int lado; // -1 direita, 1 esquerda
    struct item* pai;
    string valor;
    int fb;
    int contador;
    vector<int> linhas;
    struct item* filho_direita;
    struct item* filho_esquerda;
} no;

// ======================================================
// CRIAÇÃO DA ÁRVORE
// ======================================================

no* cria_no() {

    no* novo = new no;

    novo->lado = 0;
    novo->pai = nullptr;

    novo->valor = "";

    novo->fb = 0;
    novo->contador = 1;

    novo->filho_esquerda = nullptr;
    novo->filho_direita = nullptr;

    return novo;
}

no* cria_arv_vazia() {
    return nullptr;
}

int arvore_vazia(no* n) {
    return n == nullptr;
}

int e_folha(no* n) {

    return n->filho_direita == nullptr &&
           n->filho_esquerda == nullptr;
}

// ======================================================
// BALANCEAMENTO AVL
// ======================================================

void balancear(no* n, no*& raiz) {
    no* aux = n;

    // ==================================================
    // PESOU PARA ESQUERDA
    // ==================================================

    if (aux->fb == 2) {

        // ---------------- LL ----------------

        if (aux->filho_esquerda->fb == 1) {

            no* P = aux->pai;
            no* A = aux;
            no* B = A->filho_esquerda;
            no* C = B->filho_direita;

            int ladoAntigo = 0;

            if (P != nullptr) {
                ladoAntigo = A->lado;
            }

            B->filho_direita = A;
            B->pai = P;

            A->pai = B;
            A->filho_esquerda = C;

            A->lado = -1;

            if (C != nullptr) {

                C->pai = A;
                C->lado = 1;
            }

            if (P == nullptr) {

                raiz = B;

                B->pai = nullptr;
                B->lado = 0;
            }

            else if (ladoAntigo == 1) {

                P->filho_esquerda = B;
                B->lado = 1;
            }

            else if (ladoAntigo == -1) {

                P->filho_direita = B;
                B->lado = -1;
            }

            A->fb = 0;
            B->fb = 0;
        }

        // ---------------- LR ----------------

        else if (aux->filho_esquerda->fb == -1) {

            no* P = aux->pai;
            no* A = aux;
            no* B = A->filho_esquerda;
            no* C = B->filho_direita;

            no* D = C->filho_esquerda;
            no* E = C->filho_direita;


            int fbAntigo = C->fb;
            int ladoAntigo = 0;

            if (P != nullptr) {

                ladoAntigo = A->lado;
            }

            C->filho_esquerda = B;
            C->filho_direita = A;

            C->pai = P;

            B->pai = C;
            A->pai = C;


            B->filho_direita = D;
            A->filho_esquerda = E;


            B->lado = 1;
            A->lado = -1;


            if (D != nullptr) {

                D->pai = B;
                D->lado = -1;
            }


            if (E != nullptr) {

                E->pai = A;
                E->lado = 1;
            }

            if (P == nullptr) {

                raiz = C;

                C->pai = nullptr;
                C->lado = 0;
            }

            else if (ladoAntigo == -1) {

                P->filho_direita = C;
                C->lado = -1;
            }

            else if (ladoAntigo == 1) {

                P->filho_esquerda = C;
                C->lado = 1;
            }


            C->fb = 0;


            if (fbAntigo == 0) {

                A->fb = 0;
                B->fb = 0;
            }

            else if (fbAntigo == 1) {

                A->fb = -1;
                B->fb = 0;
            }

            else if (fbAntigo == -1) {

                A->fb = 0;
                B->fb = 1;
            }
        }
    }


    // ==================================================
    // PESOU PARA DIREITA
    // ==================================================

    else if (aux->fb == -2) {

        // ---------------- RR ----------------

        if (aux->filho_direita->fb == -1) {

            no* P = aux->pai;
            no* A = aux;
            no* B = A->filho_direita;
            no* C = B->filho_esquerda;

            int ladoAntigo = 0;

            if (P != nullptr) {

                ladoAntigo = A->lado;
            }

            B->filho_esquerda = A;
            B->pai = P;

            A->pai = B;
            A->filho_direita = C;
            A->lado = 1;

            if (C != nullptr) {

                C->pai = A;
                C->lado = -1;
            }

            if (P == nullptr) {

                raiz = B;

                B->pai = nullptr;
                B->lado = 0;
            }

            else if (ladoAntigo == -1) {

                P->filho_direita = B;
                B->lado = -1;
            }

            else if (ladoAntigo == 1) {

                P->filho_esquerda = B;
                B->lado = 1;
            }

            A->fb = 0;
            B->fb = 0;
        }

        // ---------------- RL ----------------

        else if (aux->filho_direita->fb == 1) {

            no* P = aux->pai;

            no* A = aux;
            no* B = A->filho_direita;
            no* C = B->filho_esquerda;

            no* D = C->filho_direita;
            no* E = C->filho_esquerda;


            int ladoAntigo = 0;

            int fbAntigo = C->fb;


            if (P != nullptr) {

                ladoAntigo = A->lado;
            }

            C->filho_esquerda = A;
            C->filho_direita = B;

            A->pai = C;
            B->pai = C;

            C->pai = P;

            A->filho_direita = E;
            B->filho_esquerda = D;

            A->lado = 1;
            B->lado = -1;

            if (D != nullptr) {

                D->pai = B;
                D->lado = 1;
            }


            if (E != nullptr) {

                E->pai = A;
                E->lado = -1;
            }


            if (P == nullptr) {

                raiz = C;

                C->pai = nullptr;
                C->lado = 0;
            }

            else if (ladoAntigo == -1) {

                P->filho_direita = C;
                C->lado = -1;
            }

            else if (ladoAntigo == 1) {

                P->filho_esquerda = C;
                C->lado = 1;
            }


            C->fb = 0;


            if (fbAntigo == 0) {

                A->fb = 0;
                B->fb = 0;
            }

            else if (fbAntigo == -1) {

                A->fb = 1;
                B->fb = 0;
            }

            else if (fbAntigo == 1) {

                A->fb = 0;
                B->fb = -1;
            }
        }
    }
}



// ======================================================
// ALTERA OS FATORES DE BALANCEAMENTO
// ======================================================

void altera_fatores(no* n, int lado, no*& raiz) {

    if (lado == -1) {
        n->fb--;
    }

    else {
        n->fb++;
    }


    if (n->fb == 0) {
        return;
    }


    if (n->fb == 2 || n->fb == -2) {
        balancear(n, raiz);
        return;
    }


    if (n->pai != nullptr) {
        altera_fatores(
            n->pai,
            n->lado,
            raiz
        );
    }
}

// ======================================================
// INSERÇÃO
// ======================================================

void insere(
    string valor,
    int linha,
    no*& n,
    no*& raiz
) {

    if (arvore_vazia(n)) {

        n = cria_no();

        n->pai = nullptr;

        n->valor = valor;

        n->linhas.push_back(linha);

        return;
    }

    // Palavra já existe
    if (valor == n->valor) {

        n->contador++;

        // Não repete a mesma linha
        if (
            n->linhas.empty() ||
            n->linhas.back() != linha
        ) {

            n->linhas.push_back(linha);
        }

        return;
    }

    // Vai para direita
    else if (valor > n->valor) {

        if (n->filho_direita == nullptr) {

            n->filho_direita = cria_no();

            n->filho_direita->pai = n;

            n->filho_direita->valor = valor;

            n->filho_direita->lado = -1;

            n->filho_direita->linhas.push_back(linha);

            altera_fatores(
                n,
                n->filho_direita->lado,
                raiz
            );

            return;
        }

        insere(valor, linha, n->filho_direita, raiz);
    }

    // Vai para esquerda
    else {

        if (n->filho_esquerda == nullptr) {

            n->filho_esquerda = cria_no();

            n->filho_esquerda->pai = n;

            n->filho_esquerda->valor = valor;

            n->filho_esquerda->lado = 1;

            n->filho_esquerda->linhas.push_back(linha);


            altera_fatores(n, n->filho_esquerda->lado, raiz);
            return;
        }

        insere(valor, linha, n->filho_esquerda, raiz);
    }
}

// ======================================================
// LIMPA A PALAVRA
// ======================================================

string limpaPalavra(string palavra) {

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

// ======================================================
// CARREGA PALAVRAS DE EXCLUSÃO
// ======================================================

vector<string> carregaExclusao(string nomeArquivo) {

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

// ======================================================
// VERIFICA SE A PALAVRA ESTÁ NA LISTA DE EXCLUSÃO
// ======================================================

bool palavraExcluida(const vector<string>& exclusao, const string& palavra) {

    return binary_search(exclusao.begin(), exclusao.end(), palavra);
}

// ======================================================
// LEITURA DO ARQUIVO
// ======================================================

void leituraArquivo(
    string nomeArquivo,
    no*& raiz,
    const vector<string>& exclusao
) {

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

        for (
            size_t i = 0;
            i <= linhaTexto.size();
            i++
        ) {

            if (
                i < linhaTexto.size() &&
                isalnum(
                    static_cast<unsigned char>(
                        linhaTexto[i]
                    )
                )
            ) {

                palavra += tolower(
                    static_cast<unsigned char>(
                        linhaTexto[i]
                    )
                );
            }

            else {

                if (!palavra.empty()) {


                    // Verifica se NÃO é palavra de exclusão
                    if (
                        !palavraExcluida(
                            exclusao,
                            palavra
                        )
                    ) {

                        /*
                            Aqui entra SUA AVL.

                            A palavra é inserida
                            e automaticamente balanceada.
                        */

                        insere(
                            palavra,
                            numeroLinha,
                            raiz,
                            raiz
                        );
                    }


                    palavra = "";
                }
            }
        }


        numeroLinha++;
    }


    arquivo.close();
}



// ======================================================
// BUSCA
// ======================================================

no* busca(no* n, const string& valor) {

    if (n == nullptr) {

        return nullptr;
    }


    if (valor == n->valor) {

        return n;
    }


    if (valor < n->valor) {

        return busca(
            n->filho_esquerda,
            valor
        );
    }


    return busca(
        n->filho_direita,
        valor
    );
}



// ======================================================
// MOSTRA A ÁRVORE
// ======================================================

void mostra(no* n) {

    if (n == nullptr) {
        return;
    }

    mostra(n->filho_esquerda);

    cout << "-------------------------" << endl;

    cout << "Palavra: "
         << n->valor
         << endl;

    cout << "FB: "
         << n->fb
         << endl;

    cout << "Quantidade: "
         << n->contador
         << endl;

    if (n->pai != nullptr) {
        cout << "Pai: " << n->pai->valor << endl;
    }

    else {
        cout << "Pai: nenhum (RAIZ)" << endl;
    }

    mostra(n->filho_direita);
}

// ======================================================
// MAIN
// ======================================================

int main() {

    no* n = cria_arv_vazia();
    // ----------------------------------
    // CARREGA LISTA DE EXCLUSÃO
    // ----------------------------------

    vector<string> exclusao =
        carregaExclusao("exclusao.txt");
    // ----------------------------------
    // LÊ O TEXTO E MONTA A AVL
    // ----------------------------------

    leituraArquivo(
        "texto.txt",
        n,
        exclusao
    );
   
    //mostra(n);
    cout << "Arquivo processado!"<< endl
        << "Arvore AVL criada e balanceada."<< endl;

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