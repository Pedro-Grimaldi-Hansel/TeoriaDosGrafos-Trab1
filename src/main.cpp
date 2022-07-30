#include "Edge.h"
#include "Node.h"
#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Graph *leitura(ifstream &arquivoDeEntrada, int direcionado, int arestaComPeso, int noComPeso)
{

    // Variáveis para criação dos nós do grafo
    int idNoFonte;
    int idNoAlvo;
    int ordem;
    int numArestas;
    float pesoAresta;

    // Pega a ordem do grafo
    arquivoDeEntrada >> ordem;

    cout << "\nLendo o arquivo input.txt..." << endl;

    // Cria objeto grafo
    Graph *grafo = new Graph(ordem, direcionado, arestaComPeso, noComPeso);

    // Leitura do arquivo

    // Sem peso nas arestas e sem peso nos nos
    if (!grafo->getArestaComPeso() && !grafo->getNoComPeso()) 
        while (arquivoDeEntrada >> idNoFonte >> idNoAlvo)
            grafo->insereAresta(idNoFonte, idNoAlvo, 0);
        
    // Com peso nas arestas e sem peso nos nós
    else if (grafo->getArestaComPeso() && !grafo->getNoComPeso()) 
        while (arquivoDeEntrada >> idNoFonte >> idNoAlvo >> pesoAresta)
            grafo->insereAresta(idNoFonte, idNoAlvo, pesoAresta);
    
    // Com peso no nó e sem peso na aresta
    else if (grafo->getNoComPeso() && !grafo->getArestaComPeso()) 
    {
        float pesoNoFonte, pesoNoAlvo;
        while (arquivoDeEntrada >> idNoFonte >> pesoNoFonte >> idNoAlvo >> pesoNoAlvo)
        {
            grafo->insereAresta(idNoFonte, idNoAlvo, 0);
            grafo->getNo(idNoFonte)->setPeso(pesoNoFonte);
            grafo->getNo(idNoAlvo)->setPeso(pesoNoAlvo);
        }
    }

    // Com peso no nó e com peso na aresta  
    else if (grafo->getNoComPeso() && grafo->getArestaComPeso()) 
    {
        float pesoNoFonte, pesoNoAlvo, pesoAresta;
        while (arquivoDeEntrada >> idNoFonte >> pesoNoFonte >> idNoAlvo >> pesoNoAlvo)
        {
            grafo->insereAresta(idNoFonte, idNoAlvo, pesoAresta);
            grafo->getNo(idNoFonte)->setPeso(pesoNoFonte);
            grafo->getNo(idNoAlvo)->setPeso(pesoNoAlvo);
        }
    }
    return grafo;
}

int main(int argc, char const *argv[])
{
    // Verificando os parâmetros do programa
    if (argc != 6) 
    {
        cout << "ERRO: Esperado: ./<nome_Programa> <arquivoDeEntrada> <arquivoDeSaida> <direcionado> <arestaComPeso> <noComPeso> " << endl;
        return 1;
    }

    string nomeDoPrograma(argv[0]);
    string entradaNomeDoArquivo(argv[1]);

    // Abrindo arquivo de entrada
    ifstream arquivoDeEntrada;
    ofstream arquivoDeSaida;
    arquivoDeEntrada.open(argv[1], ios::in);
    arquivoDeSaida.open(argv[2], ios::out | ios::trunc);

    Graph *grafo;

    if (arquivoDeEntrada.is_open())
        grafo = leitura(arquivoDeEntrada, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    else
        cout << "Nao foi possível abrir o arquivo! " << argv[1];

    string direcionado, arestaPeso, verticePeso;
    direcionado = arestaPeso = verticePeso = "Sim";

    if (!grafo->getDirecionado())
        direcionado = "Nao";
    if (!grafo->getArestaComPeso())
        arestaPeso = "Nao";
    if (!grafo->getNoComPeso())
        verticePeso = "Nao";

    cout << "\nCriando grafo..." << endl;
    cout << "Ordem: " << grafo->getOrdem() << endl;
    cout << "N Arestas: " << grafo->getNumArestas() << endl;
    cout << "Direcionado? " << direcionado << endl;
    cout << "Arestas com peso? " << arestaPeso << endl;
    cout << "Vertices com peso? " << verticePeso << endl;

    int entrada;
    cout << "\nFUNCIONALIDADES" << endl;

     if(grafo->getDirecionado()){
      cout << "1) Fecho Transitivo Direto" << endl;
      cout << "Digite o id do no: ";
      cin >> entrada;
      grafo->fechoTransitivoDireto(entrada);
      cout << endl;

      cout << "2) Fecho Transitivo Indireto" << endl;
      cout << "Digite o id do no: ";
      cin >> entrada;
      grafo->fechoTransitivoIndireto(entrada);
      cout << endl;
    }else
        cout<<"Funcionalidades 1 e 2 precisam que o grafo seja direcionado."<<endl; 

    cout << "3) Coeficiente de Agrupamento Local do Vertice" << endl;
    cout << "Digite o id do no: ";
    cin >> entrada;
    cout << "Coeficiente de agrupamento local do vertice: " << grafo->coefAgrupamentoLocal(entrada) <<  endl;

    cout << endl;  
 
    cout << "4) Coeficiente de Agrupamento Medio do Grafo" << endl;
    cout << "Coeficiente de agrupamento Medio do grafo: " <<   grafo->coefAgrupamentoMedio()  << endl;
    cout << endl; 

     cout << "5) Caminho Minimo entre dois vertices - Dijkstra" << endl;
    grafo->camMinDijkstra();
    cout << endl;

    cout << "6) Caminho Minimo entre dois vertices - Floyd" << endl;
    grafo->camMinFloyd();
    cout << endl;

    cout << "7) Caminhamento em Profundidade" << endl;
    grafo->buscaProfundidade(arquivoDeSaida);
    cout << endl;

    cout << "8) Arvore Geradora Minima - Prim" << endl;
    grafo->agmPrim(grafo->getVertInduz(), arquivoDeSaida);
    cout << endl;

    cout << "9) Arvore Geradora Minima - Kruskal" << endl;
    grafo->agmKruskal(grafo->getVertInduz(), arquivoDeSaida);
    cout << endl;

    system("dot -Tpng -O output.dot");  

    // Fechando arquivo de arquivoDeEntrada e saida
    arquivoDeEntrada.close();
    arquivoDeSaida.close();

    return 0;
}
