#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <vector>
#include <stack>
#include <list>


using namespace std;

class Graph
{

    //Atributes
private:
    int ordem;
    int numArestas;
    int posicaoNo; //posição de inserção do node
    bool direcionado;
    bool arestaComPeso;
    bool noComPeso;

    Node *primeiroNo;
    Node *ultimoNo;

public:
    //Constructor & Destructor
    Graph(int ordem, bool direcionado, bool arestaComPeso, bool noComPeso);
    ~Graph();

    //Getters
    int getOrdem();
    int getNumArestas();
    bool getDirecionado();
    bool getArestaComPeso();
    bool getNoComPeso();
    Node *getPrimeiroNo();
    Node *getUltimoNo();
    Node *getNo(int id);
    Node *getPosicaoNo(int posicaoNo); //Pega um no a partir de sua posição de inserção
    Graph *getVertInduz();

    // Other methods
    void insereNo(int id);
    void insereAresta(int id, int idAlvo, float peso);
    void removeNode(int id);
    bool procuraNo(int id);
    void limpaVisitado(); // Define todos os nós como não visitados.

    // Funcionalidades
    void fechoTransitivoDireto(int id);
    void fechoTransitivoIndireto(int id);
    float coefAgrupamentoLocal(int id);
    float coefAgrupamentoMedio();
    float camMinDijkstra();
    float camMinFloyd();
    void agmPrim(Graph *subgrafo, ofstream &outFile);
    void agmKruskal(Graph *subgrafo, ofstream &outFile);
    void buscaProfundidade(ofstream &outFile);
    void printBuscaProfundidade(vector<string> *corpo, ofstream &outFile);

private:
    //Auxiliar methods
    void caminhoProfundidade(Node *node);
    void auxBuscaProfundidade(Node *node, vector<int> *finG, vector<int> *retorno, ofstream &outFile,  vector<string> *corpo);
    void caminhoMinimo(list<int> &antecessor);
    int **iniciaAnterioresFloyd(int **anteriores, int tam);
    int **criaMatrizDistancia(int **distancia, int tam);
    void saidaFloyd(int **pred, Node *noSource, Node *noTarget);
    void saidaDijkstra(int antecessor[], int idSource, int idTarget);

    void printMatriz(int **matriz, int tam);

};



#endif // GRAPH_H_INCLUDED
