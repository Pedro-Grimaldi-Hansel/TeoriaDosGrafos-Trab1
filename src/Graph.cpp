#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "Edge.h"
#include "Node.h"
#include "Graph.h"

using namespace std;
int INF = 99999999;
int D = 3; // restricao de grau

//**************************************************************************************************
//! * Defining the Graph's methods
//**************************************************************************************************/

//! Constructor
Graph::Graph(int ordem, bool direcionado, bool arestaComPeso, bool noComPeso)
{
    this->ordem = ordem;
    this->direcionado = direcionado;
    this->arestaComPeso = arestaComPeso;
    this->noComPeso = noComPeso;
    this->primeiroNo = this->ultimoNo = nullptr;
    this->numArestas = 0;
    this->posicaoNo = 0;
}

//! Destructor
Graph::~Graph()
{
    Node *proxNo = this->primeiroNo;

    while (proxNo != nullptr)
    {
        proxNo->removeTodasArestas();
        Node *noAux = proxNo->getProxNo();
        delete proxNo;
        proxNo = noAux;
    }
}

//! Getters --------------------------------------------------------------------------------------------

int Graph::getOrdem()
{
    return this->ordem;
}
int Graph::getNumArestas()
{
    return this->numArestas;
}

// Verifica se o grafo é direcionado
bool Graph::getDirecionado()
{
    return this->direcionado;
}

// Verifica se o grafo tem peso na aresta
bool Graph::getArestaComPeso()
{
    return this->arestaComPeso;
}

// Verifica se o grafo tem peso no Nó
bool Graph::getNoComPeso()
{
    return this->noComPeso;
}

Node *Graph::getPrimeiroNo()
{
    return this->primeiroNo;
}

Node *Graph::getUltimoNo()
{
    return this->ultimoNo;
}

Node *Graph::getNo(int id)
{
    // cria ponteiro para percorrer a lista de nós
    Node *no = primeiroNo;

    // encontra o no com o id desejado
    while (no != nullptr)
    {
        if (no->getId() == id)
            return no;
        no = no->getProxNo();
    }

    // retorna null se nao encontrar
    return nullptr;
}

Node *Graph::getPosicaoNo(int posicaoNo)
{
    // cria ponteiro para percorrer a lista de nós
    Node *no = primeiroNo;

    // encontra o no com o id desejado
    while (no != nullptr)
    {
        if (no->getPosicaoNo() == posicaoNo)
            return no;

        no = no->getProxNo();
    }

    // retorna null se nao encontrar
    return nullptr;
}

// Funcao para gerar um Subgrafo Vertice Induzido
Graph *Graph::getVertInduz()
{

    cout << "\nDigite os IDs dos vertices que irao compor esse subgrafo separados por ponto-virgula (Exemplo: 5;6;1;8):" << endl;

    // Lendo os vertices do subgrafo
    string aux;
    cout << "Vertices: ";
    cin >> aux;

    // Vector para armazenar os ids dos vertices do subgrafo
    vector<int> idVertices;
    idVertices.clear();

    // Separando a string
    stringstream ss(aux);
    while (getline(ss, aux, ';'))
    {
        if (this->procuraNo(stoi(aux)))
            idVertices.push_back(stoi(aux));
        else
            cout << "O vertice " << aux << " eh invalido, pois nao esta no Grafo" << endl;
    }

    // Criar o subgrafo vertice induzido
    Graph *subgrafo = new Graph(idVertices.size(), this->getDirecionado(), this->getArestaComPeso(), this->getNoComPeso());

    // Inserindo as arestas correspondentes no subgrafo
    this->limpaVisitado();
    for (int i = 0; i < idVertices.size(); i++)
    {
        for (int j = i + 1; j < idVertices.size(); j++)

            // Verificar se a aresta realmente existe no grafo original
            if ((!this->getNo(idVertices[j])->getFoiVisitado()) && this->getNo(idVertices[i])->procuraAresta(idVertices[j]))
            {
                Edge *aux = this->getNo(idVertices[i])->getAresta(idVertices[j]);
                subgrafo->insereAresta(idVertices[i], idVertices[j], aux->getPeso());
            }
            else
                subgrafo->insereNo(idVertices[j]);

        this->getNo(idVertices[i])->setFoiVisitado(true);
    }

    cout << "\nO Subgrafo X foi gerado com sucesso! ";
    cout << "(Ordem = " << subgrafo->getOrdem() << " e Numero de Arestas = " << subgrafo->getNumArestas() << ")" << endl;

    return subgrafo;
}

//! Other methods --------------------------------------------------------------------------------------------

void Graph::insereNo(int id)
{
    // confere se o grafo tem nós
    if (primeiroNo != nullptr)
    {
        // caso tenha, cria um novo no, aponta o ultimo pro novo e o novo de torna o ultimo
        Node *novoNo = new Node(id);
        novoNo->setPosicaoNo(this->posicaoNo);
        ultimoNo->setProxNo(novoNo);
        ultimoNo = novoNo;
    }
    else
    {
        // caso nao tenha, cria um novo no e ele se torna o ultimo e o primeiro
        Node *novoNo = new Node(id);
        novoNo->setPosicaoNo(this->posicaoNo);
        primeiroNo = novoNo;
        ultimoNo = novoNo;
    }
    this->posicaoNo = this->posicaoNo + 1;
}

void Graph::insereAresta(int id, int idAlvo, float peso)
{
    // cria um ponteiro para o no desejado e um auxiliar para o no alvo da aresta
    Node *no = getNo(id);
    Node *aux = getNo(idAlvo);

    if (no == nullptr)
    {
        this->insereNo(id);
        no = ultimoNo;
    }
    if (aux == nullptr)
    {
        this->insereNo(idAlvo);
        aux = ultimoNo;
    }
    // verifica se os nós existem
    if (no != nullptr && aux != nullptr)
    {

        // verifica se a aresta já existe
        if (!no->procuraAresta(idAlvo))
        {
            // se nao, insere a aresta
            no->insereAresta(idAlvo, aux->getPosicaoNo(), peso);

            this->numArestas++;

            // se o grafo for nao-direcionado e nao houver aresta de volta, insere ela
            if (this->direcionado == 0 && !aux->procuraAresta(id))
            {
                aux->insereAresta(id, no->getPosicaoNo(), peso);
            }
        }
    }
}

void Graph::removeNode(int id)
{
    // cria um ponteiro para o nó
    Node *no = getNo(id);

    // retorna caso nao exista
    if (no == nullptr)
        return;
    else if (no == primeiroNo) // se o nó é o primeiro, apenas faz o proximo ser o primeiro
        primeiroNo = no->getProxNo();
    else
    {
        // caso seja um no do meio ou o ultimo, cria um ponteiro auxiliar
        Node *aux = primeiroNo;

        // encontra o no anterior ao no desejado
        while (aux->getProxNo() != no)
            aux = aux->getProxNo();

        // se o no desejado for o ultimo, atualiza o ultimo para o anterior
        if (no == ultimoNo)
            ultimoNo = aux;

        // seta o proximo de anterior para o proximo do desejado
        aux->setProxNo(no->getProxNo());
    }

    // deleta o no desejado
    delete no;
}

bool Graph::procuraNo(int id)
{
    // cria um ponteiro para o no desejado
    Node *no = getNo(id);

    // retorna falso caso nao encontre e verdadeiro caso encontre
    if (no == nullptr)
        return false;
    else
        return true;
}

// Funcao para definir todos os nós do grafo como nao visitados.
void Graph::limpaVisitado()
{
    Node *no = this->getPrimeiroNo(); // Ponteiro que armazena o endereço de memória do primeiro nó do grafo.

    // Realiza a operacao para todos os nós do grafo.
    while (no != nullptr)
    {
        no->setFoiVisitado(false); // Define o nó como nao visitado.
        no = no->getProxNo();      // Ponteiro passa a apontar para o próximo nó do grafo.
    }
}

//! FUNcÕES AUXILIARES --------------------------------------------------------------------------------------------

// Estrutura e funcões auxiliares para o algoritmo de Kruskal
struct SubArvore
{
    int pai;
    int ordem;
};

// Função para encontrar em qual sub-Arvore está o nó de id n. Usada no Kruskal
int qualSubArvore(SubArvore subArvore[], int n)
{
    if (subArvore[n].pai != n)
        subArvore[n].pai = qualSubArvore(subArvore, subArvore[n].pai);

    return subArvore[n].pai;
}

// Funcao para unir duas subArvores de dois nós u e v. Usada no Kruskal
void unirSubArvores(SubArvore subArvore[], int u, int v)
{
    // Encontrando os indices das subArvores
    int subU = qualSubArvore(subArvore, u);
    int subV = qualSubArvore(subArvore, v);

    // Unindo a menor com a maior
    if (subArvore[subU].ordem < subArvore[subV].ordem)
        subArvore[subU].pai = subV;
    else if (subArvore[subU].ordem > subArvore[subV].ordem)
        subArvore[subV].pai = subU;

    else
    {
        subArvore[subV].pai = subU;
        subArvore[subU].ordem += subArvore[subV].ordem;
    }
}

// Função para auxiliar o algoritmo de Prim. Retorna a posição do nó com menor custo de vizinhança que nao esteja na agm
int posicaoMenor(vector<int> &custoViz, vector<bool> &naAGM)
{
    int min = INF;
    int pos;
    bool tem_pos = false;
    for (int i = 0; i < custoViz.size(); i++)
    {
        if (custoViz[i] < min && naAGM[i] == false)
        {
            min = custoViz[i];
            pos = i;
            tem_pos = true;
        }
    }
    if (tem_pos)
        return pos;
    else
    {
        for (int i = 0; i < custoViz.size(); i++)
        {
            if (custoViz[i] == min && naAGM[i] == false)
            {
                min = custoViz[i];
                pos = i;
                tem_pos = true;
                return pos;
            }
        }
    }
    return pos;
}

// Funcao para imprimir a AGM via Prim
void imprimirPrim(Graph *subgrafo, vector<int> &agm, ofstream &arquivoDeSaida)
{
    int peso = 0;
    cout << "\nARVORE GERADORA MINIMA via Prim\n"
         << endl;
    cout << "graph {" << endl;
    for (int i = 0; i < subgrafo->getOrdem(); i++)
    {
        if (agm[i] != INF)
        {
            int id_destino = subgrafo->getPosicaoNo(i)->getId();
            if (agm[i] == id_destino)
                cout << "  " << agm[i] << endl;
            else
            {
                cout << "  " << agm[i] << " -- " << id_destino;
                cout << " [label = " << subgrafo->getNo(agm[i])->getAresta(id_destino)->getPeso() << "]" << endl;
                peso += subgrafo->getNo(agm[i])->getAresta(id_destino)->getPeso();
            }
        }
    }
    cout << "}" << endl;
    cout << "\nPeso da AGM: " << peso << endl;
    cout << "\nPrim concluido com sucesso!" << endl;

    arquivoDeSaida << "graph {" << endl;
    for (int i = 0; i < subgrafo->getOrdem(); i++)
    {
        if (agm[i] != INF)
        {
            int id_destino = subgrafo->getPosicaoNo(i)->getId();
            if (agm[i] == id_destino)
                arquivoDeSaida << "  " << agm[i] << endl;
            else
            {
                arquivoDeSaida << "  " << agm[i] << " -- " << id_destino;
                arquivoDeSaida << " [label = " << subgrafo->getNo(agm[i])->getAresta(id_destino)->getPeso() << "]" << endl;
            }
        }
    }
    arquivoDeSaida << "}" << endl;
    cout << "\nImpressao concluida!" << endl;
    cout << "\npng criado como output.dot.2.png" << endl;
}

// Funcao para imprimir a AGM via Kruskal
void imprimirKruskal(vector<pair<int, pair<int, int>>> &arestas, vector<int> &agm, ofstream &arquivoDeSaida)
{
    int peso = 0;
    cout << "\nARVORE GERADORA MiNIMA via Kruskal\n"
         << endl;
    cout << "graph {" << endl;
    for (int i = 0; i < agm.size(); i++)
    {
        if (arestas[agm[i]].second.first == arestas[agm[i]].second.second)
            cout << "  " << arestas[agm[i]].second.first << endl;
        else
        {
            cout << "  " << arestas[agm[i]].second.first << " -- " << arestas[agm[i]].second.second;
            cout << " [label = " << arestas[agm[i]].first << "]" << endl;
            peso += arestas[agm[i]].first;
        }
    }
    cout << "}" << endl;
    cout << "\nPeso da AGM: " << peso << endl;
    cout << "\nKruskal concluido com sucesso!" << endl;

    arquivoDeSaida << "graph {" << endl;
    for (int i = 0; i < agm.size(); i++)
    {
        if (arestas[agm[i]].second.first == arestas[agm[i]].second.second)
            arquivoDeSaida << "  " << arestas[agm[i]].second.first << endl;
        else
        {
            arquivoDeSaida << "  " << arestas[agm[i]].second.first << " -- " << arestas[agm[i]].second.second;
            arquivoDeSaida << " [label = " << arestas[agm[i]].first << "]" << endl;
        }
    }
    arquivoDeSaida << "}" << endl;
    cout << "\nImpressao concluida!" << endl;
    cout << "\npng criado como output.dot.3.png" << endl;
}

// FUNCIONALIDADES --------------------------------------------------------------------------------------------

/**
 * @brief          Funcao para identificar e exibir o Fecho Transitivo Direto do grafo direcionado.
 * @param id       Valor que representa o id do nó para o qual será calculado o fecho transitivo direto.
 */
void Graph::fechoTransitivoDireto(int id)
{

    Node *no = this->getNo(id); // Nó para o qual serA calculado o fecho transitivo direto.

    this->limpaVisitado(); // Chama a funcao para setar todos os nós do grafo como nao visitados.

    // Verifica se o nó no existe.
    if (no != nullptr)
    {
        cout << "Fecho transitivo direto: ";

        caminhoProfundidade(no); // Realiza o caminho em profundidade no grafo a partir do nó no.

        // Imprime o id de todos os nós visitados.
        for (no = this->primeiroNo; no != nullptr; no = no->getProxNo())
        {
            Edge *aresta = no->getPrimeiraAresta();

            if (no->getFoiVisitado())
                cout << no->getId() << " | ";
        }
    }
    // Se no nao existe, exibe uma mensagem de erro.
    else
        cout << "Erro! Nao existe um nó com o id proposto.";
}

/**
 * @brief          Funcao para identificar e exibir o Fecho Transitivo Indireto do grafo direcionado.
 * @param id       Valor que representa o id do nó para o qual serA calculado o fecho transitivo indireto.
 */
void Graph::fechoTransitivoIndireto(int id)
{

    Node *noAlvo = this->getNo(id);           // Nó alvo que recebe o id passado como parametro.
    Node *primeiroNo = this->getPrimeiroNo(); // Nó através do qual será feita a verificacao se o alvo é acessivel.

    // Verifica se o nó alvo existe.
    if (noAlvo != nullptr)
    {
        cout << "Fecho transitivo indireto: ";

        // Realiza a busca em profundidade para todos os nós do grafo.
        while (primeiroNo != nullptr)
        {

            this->limpaVisitado(); // Chama a funcao para setar todos os nós do grafo como nao visitados.

            caminhoProfundidade(primeiroNo); // Realiza o caminho em profundidade no grafo a partir do primeiro no.

            // Se o no alvo foi visitado no caminho em profundidade, imprime o id do primeiro no.
            if (noAlvo->getFoiVisitado())
            {

                cout << primeiroNo->getId() << " | ";
            }
            primeiroNo = primeiroNo->getProxNo();
        }
    }
    // Se o no alvo nao existe, imprime uma mensagem de erro.
    else
    {
        cout << "Erro! Nao existe um no com o id proposto.";
    }
}

void Graph ::printMatriz(int **distancia, int tam)
{
    cout << "MATRIZ DE DISTANCIAS" << endl;
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            if (distancia[i][j] == INF)
                cout << "INF ";
            else
                cout << distancia[i][j] << " ";
        }
        cout << endl;
    }
}

float Graph::coefAgrupamentoLocal(int id)
{
    Node *no = this->getNo(id);
    // verifica se o nó existe
    if (no == nullptr)
    {
        cout << "Erro! Nao existe um no com o id proposto."<<endl;
        return -1;
    }

    else
    {
        int tam = this->getOrdem();
        int **distancia = criaMatrizDistancia(distancia, tam); // cria matriz com as distancias de cada no
        int pos = no->getPosicaoNo();
        int pV = 0; // numero de links entre os vizinhos
        float coef;

        // descobre o grau do no
        int grauNo = 0;
        for (int j = 0; j < tam; j++)
            if (distancia[pos][j] != INF && distancia[pos][j] != 0)
                grauNo++;

        int vet[grauNo]; // vetor que guarda os vizinhos do no
        // preenche vetor com os ids dos adjascentes do parametro
        int g = 0;
        for (int j = 0; j < tam; j++)
        {
            if (distancia[pos][j] != INF && distancia[pos][j] != 0)
            {
                vet[g] = j;
                g++;
            }
        }

        // descobre quantos links os nos adjascentes fazem
        for (int i = 0; i < grauNo; i++)
        {
            int idAux = vet[i];
            for (int j = 0; j < grauNo; j++)
            {
                if (i != j)
                {
                    if (distancia[idAux][vet[j]] != INF && distancia[idAux][vet[j]] != 0)
                    {
                        pV++;
                    }
                }
            }
        }
        // como a matriz de distância é simetrica, divide pV por dois
        pV = pV / 2;

        // calcula o coeficiente
        float grau = grauNo * (grauNo - 1);
        coef = pV / grau;
        coef = coef / 2;
        if (isnan(coef))
            return 0;
        
        // desaloca a matriz distancia
        for (int k = 0; k < tam; k++)
            delete[] distancia[k];
        delete[] distancia;

        return coef; //retorna o coeficiente local
    }
}
float Graph::coefAgrupamentoMedio()
{
    // percorre o grafo e soma o coeficiente de cada no, retorna a media do coeficiente geral
    Node *aux = this->getPrimeiroNo();
    int tam = this->getOrdem();
    float coef = 0.0;
    
    for (int i = 0; i < tam; i++)
    {
        coef += coefAgrupamentoLocal(aux->getId());
        aux = aux->getProxNo();
    }
    return coef / tam;
}

/**
 * @brief    Funcao para buscar o caminho minimo usando o algoritmo de Dijkstra
 */
float Graph::camMinDijkstra()
{

    int idNoFonte, idNoAlvo;
    Node *noFonte, *noAlvo;
    string idP, idS;
    try
    { // Try para verificar se os parametros passados sao IDs inteiros
        cout << "Digite o primeiro no" << endl;
        cin >> idP;
        cout << "Digite o segundo no" << endl;
        cin >> idS;
        idNoFonte = stoi(idP);
        idNoAlvo = stoi(idS);
    }
    catch (const exception &e)
    {
        cout << "Parametros invalidos." << endl;
        return 0;
    }

    if (idNoFonte == idNoAlvo)
    {
        cout << "\n\nA distancia eh: " << 0 << endl;
        return 0;
    } // Encerra caso seja o mesmo no

    noFonte = getNo(idNoFonte); // Busca o no
    noAlvo = getNo(idNoAlvo);   // Busca o no

    if (noFonte != nullptr && noAlvo != nullptr)
    {

        int posFont = noFonte->getPosicaoNo(), posAlvo = noAlvo->getPosicaoNo(), distancia = INF, V = getOrdem();
        int ver = 0, c_edge = 0, u;

        int *distance = new int[V];      // Vetor para os distancias entre a posicao do noFonte e os demais
        int *antec = new int[V];         // Vetor para os antecessores
        bool *foiVisitado = new bool[V]; // Vetor para as posicões ja visitadas
        for (int i = 0; i < V; i++)
        {
            distance[i] = INF;
            foiVisitado[i] = false;
        }                      // Inicializador dos vetores visitados e distancia
        distance[posFont] = 0; // Distancia do vertice para ele mesmo

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fp; // Fila de prioridade para os pares distancia e vertice

        fp.push(make_pair(distance[posFont], posFont)); // Adiciona o par vetor distancia e

        pair<int, int> p = fp.top(); // Adiciona o p na fila de prioridade

        Node *no = nullptr;
        Edge *aresta = nullptr;

        while (!fp.empty())
        {

            pair<int, int> p = fp.top(); // Pega o do topo
            u = p.second;                // Obtem o vehtice
            fp.pop();                    // Remove da lista de prioridade
            if (foiVisitado[u] == false)
            {
                foiVisitado[u] = true; // Marca o vertice como visitado
                no = getPosicaoNo(u);
                if (no != nullptr) // Busca o no pela posicao
                    aresta = no->getPrimeiraAresta();
                else
                    aresta = nullptr; // Pega a primeira aresta do no

                while (aresta != nullptr)
                { // Passa por todas as arestas do vertice u

                    if (!getArestaComPeso())
                        c_edge = 1; // Para caso nao haja peso a distancia sera 1 por salto
                    else
                        c_edge = aresta->getPeso();

                    ver = aresta->getPosicaoAlvo(); // Pega a posicao do no Alvo dessa aresta

                    if (distance[ver] > (distance[u] + c_edge))
                    {                                           // Verifica se a distancia eh menor
                        antec[ver] = u;                         // Atualiza o antecessor
                        distance[ver] = (distance[u] + c_edge); // Atualiza a distancia
                        fp.push(make_pair(distance[ver], ver)); // Adiciona o vertice na fila de prioridade
                    }
                    aresta = aresta->getProxAresta(); // Avanca para o a proxima aresta do vertice
                }
            }
        }

        distancia = distance[posAlvo];

        delete[] distance;    // Desalocando o vetore usado
        delete[] foiVisitado; // Desalocando o vetore usado

        if (distancia < INF)
            saidaDijkstra(antec, posFont, posAlvo); // Imprime todo a lista na ordem de acesso

        delete[] antec;
        cout << "\n\nA distancia eh: " << distancia << endl;
        return distancia;
    }
    else
    {

        if (noFonte == nullptr)
            cout << "No fonte nao existe nesse grafo" << endl;
        if (noAlvo == nullptr)
            cout << "No alvo nao existe nesse grafo" << endl;
        return -1;
    }
}

/**
 * @brief Funcao de busca de caminho minimo usando o algoritmo de Floyd-Warshall
 *
 */
float Graph::camMinFloyd()
{
    int idNoFonte, idNoAlvo;
    Node *noFonte, *noAlvo;
    string idP, idS;
    try
    { // Try para verificar se os parametros passados sao IDs inteiros
        cout << "Digite o primeiro no" << endl;
        cin >> idP;
        cout << "Digite o segundo no" << endl;
        cin >> idS;
        idNoFonte = stoi(idP);
        idNoAlvo = stoi(idS);
    }
    catch (const exception &e)
    {
        cout << "Parametros invalidos" << endl;
        return 0;
    }

    if (idNoFonte == idNoAlvo)
    {
        cout << "\n\nA distancia eh: " << 0 << endl;
        return 0;
    }

    noFonte = getNo(idNoFonte); // Busca o no fonte
    noAlvo = getNo(idNoAlvo);   // Busca o no alvo

    if (noFonte != nullptr && noAlvo != nullptr)
    {

        int V = getOrdem(), i, j, k, distancia;
        int **dist, **pred;

        dist = criaMatrizDistancia(dist, V);   // Inicia a matriz de distancia
        pred = iniciaAnterioresFloyd(pred, V); // Inicia a matriz de anteriores

        for (k = 0; k < V; k++) // Calculando a distancia de todas as posicões
            for (i = 0; i < V; i++)
                for (j = 0; j < V; j++)
                    if (dist[i][j] > (dist[i][k] + dist[k][j]))
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        pred[i][j] = pred[k][j]; // Atualiza a posicao do no antercessor dos nos na posicao i e j
                    }
        distancia = dist[noFonte->getPosicaoNo()][noAlvo->getPosicaoNo()]; // Armazena a distancia minima entre os dois nós

        for (i = 0; i < V; i++)
        {
            delete[] dist[i];
        }
        delete[] dist; // Desalocando a matriz distancia usada

        if (distancia < INF)
            saidaFloyd(pred, noFonte, noAlvo);

        for (i = 0; i < V; i++)
        {
            delete[] pred[i];
        } // Desalocando a matriz de antecessores
        delete[] pred;

        cout << "\n\nDistancia eh: " << distancia << endl;
        return distancia;
    }
    else
    {

        if (noFonte == nullptr)
            cout << "No fonte nao existe nesse grafo" << endl;
        if (noAlvo == nullptr)
            cout << "No alvo nao existe nesse grafo" << endl;
        return -1;
    }
}

/**
 * @brief ALGORITMO DE PRIM para encontrar a Arvore Geradora Minima
 *
 * @param subgrafo
 * @param arquivoDeSaida
 */
void Graph::agmPrim(Graph *subgrafo, ofstream &arquivoDeSaida)
{
    cout << "\nIniciando a execucao do algoritmo de Prim..." << endl;

    // 1 PASSO: Organizar os custos das vizinhancas dos nós em um vector

    // Vector para armazenar os custoViz dos nós do subgrafo. O indice no vector eh compativel com a posicao do nó no subgrafo
    vector<int> custoViz;
    custoViz.clear();

    // Vector para checar se o nó ja foi inserido na agm
    vector<bool> naAGM(subgrafo->getOrdem(), false);

    // O primeiro nó do vector sera inicializado com custoViz = 0
    custoViz.push_back(0);

    // Os demais nós serao inicializados com custoViz = INFINITO
    for (int i = 1; i < subgrafo->getOrdem(); i++)
        custoViz.push_back(INF);

    // 2 PASSO: Criar Arvore Geradora Minima -> vetor com os pais de cada nó da agm ou INF caso nao tenha pai

    // Os indices da agm corresponderao à posicao do nó no subgrafo
    // A raiz da agm, indice 0, serA o primeiro nó do subgrafo, portanto nao terA pai
    vector<int> agm(subgrafo->getOrdem(), INF);

    // 3 PASSO: Iterar pelos vertices verificando o custoViz e inserindo na agm

    int cont = 0;
    while (cont < subgrafo->getOrdem())
    {
        // Pega o nó com menor custoViz que ainda nao esta na agm
        int pos_menor = posicaoMenor(custoViz, naAGM);      // Posicao do nó
        int u = subgrafo->getPosicaoNo(pos_menor)->getId(); // ID do nó
        // Atualiza naAGM, pois, nessa iteracao, u serA colocado na agm
        naAGM[pos_menor] = true;

        // Iterar pelos nós v adjacentes a u e verificar se o peso da aresta entre eles eh menor que o seu custoViz
        Edge *aux = subgrafo->getNo(u)->getPrimeiraAresta();
        if (aux == nullptr) // nó nao tem arestas
            agm[pos_menor] = u;
        else
        {
            while (aux != nullptr)
            {
                int v = aux->getIdAlvo();                       // ID de v
                int pos_v = subgrafo->getNo(v)->getPosicaoNo(); // posicao de v
                if (!naAGM[pos_v])                              // executa caso o nó v ainda nao esteja na agm
                {
                    // Se o peso da aresta (u, v) for menor que o custoViz de v, atualiza o custoViz com o valor do peso
                    if (aux->getPeso() < custoViz[pos_v])
                    {
                        custoViz[pos_v] = aux->getPeso();
                        // Atualiza o pai de v na agm
                        agm[pos_v] = u;
                    }
                }
                aux = aux->getProxAresta();
            }
        }
        cont++;
    }

    // 4 PASSO: Imprimir a Arvore Geradora Minima e seu peso

    imprimirPrim(subgrafo, agm, arquivoDeSaida);

    return;
}

// ALGORITMO DE KRUSKAL
// para encontrar a Arvore Geradora Minima
void Graph::agmKruskal(Graph *subgrafo, ofstream &arquivoDeSaida)
{
    cout << "\nIniciando a execucao do algoritmo de Kruskal..." << endl;

    // 1 PASSO: Vector para armazenar as arestas do grafo

    vector<pair<int, pair<int, int>>> arestas; // vector<peso, noOrigem, noDestino>
    arestas.clear();

    subgrafo->limpaVisitado();
    Node *noAux = subgrafo->getPrimeiroNo();
    Edge *arestaAux = noAux->getPrimeiraAresta();

    int u = noAux->getId(); // id do nó de origem
    int v;

    if (arestaAux != nullptr)
        v = arestaAux->getIdAlvo(); // id do nó destino

    // Percorrer todas as arestas do Grafo
    for (int i = 1; i < subgrafo->getOrdem(); i++)
    {
        if (arestaAux == nullptr)
            arestas.push_back({INF, {u, u}});

        while (arestaAux != nullptr)
        {
            // Coloca a aresta no vetor de arestas
            if (!subgrafo->getNo(v)->getFoiVisitado())
                arestas.push_back({arestaAux->getPeso(), {u, v}});

            // Atualiza os auxiliares se a aresta nao for null
            arestaAux = arestaAux->getProxAresta();
            if (arestaAux != nullptr)
            {
                v = arestaAux->getIdAlvo();
            }
        }

        noAux->setFoiVisitado(true);
        noAux = subgrafo->getPosicaoNo(i);
        arestaAux = noAux->getPrimeiraAresta();
        u = noAux->getId();
        if (arestaAux != nullptr)
            v = arestaAux->getIdAlvo();
    }

    cout << "1 passo concluido com sucesso" << endl;

    // 2 PASSO: Ordenar as arestas por peso do menor para o maior

    sort(arestas.begin(), arestas.end());
    cout << "2 passo concluido com sucesso" << endl;

    // 3 PASSO: Criar subAvores cada uma contendo um nó isolado

    int V = subgrafo->getOrdem();
    SubArvore *subArvore = new SubArvore[(V * sizeof(SubArvore))]; // vetor para armazenar todas as subArvores

    for (int i = 0; i < V; i++)
    {
        subArvore[i].pai = i;
        subArvore[i].ordem = 1;
    }
    cout << "3 passo concluido com sucesso" << endl;

    // 4 PASSO: Montar a Arvore Geradora Minima

    vector<int> agm; // vetor com o indice associado à posicao de cada aresta da Arvore geradora minima no vector 'arestas' do subgrafo
    agm.clear();

    // Iterar ate atingir condicao de parada
    int cont = 0;
    while (agm.size() < V - 1 && cont < arestas.size())
    {
        pair<int, int> proxAresta = arestas[cont].second;
        int u = proxAresta.first;
        int v = proxAresta.second;

        if (u == v)
            agm.push_back(cont);

        // Se u e v nao estao na mesma subArvore
        if (qualSubArvore(subArvore, subgrafo->getNo(u)->getPosicaoNo()) != qualSubArvore(subArvore, subgrafo->getNo(v)->getPosicaoNo()))
        {
            agm.push_back(cont);
            unirSubArvores(subArvore, subgrafo->getNo(u)->getPosicaoNo(), subgrafo->getNo(v)->getPosicaoNo());
        }
        cont++;
    }
    cout << "4 passo concluido com sucesso" << endl;

    // 5 PASSO: Imprimir a Arvore Geradora Minima e seu peso

    imprimirKruskal(arestas, agm, arquivoDeSaida);

    delete[] subArvore;
    return;
}

/**
 * @brief Caminhamento Profundidade destacando as Arestas de retorno
 *
 * @param arquivoDeSaida Arquivo para geração de saida gráfica
 */
void Graph::buscaProfundidade(ofstream &arquivoDeSaida)
{
    // variaveis
    vector<int> retorno;
    vector<int> findG;
    vector<string> graph;
    int id;

    cout << "\nCaminhamento Profundidade destacando as Arestas de retorno\n\n";
    do
    {
        cout << "Informe o numero do no: ";
        cin >> id;
    } while (!this->procuraNo(id));

    Node *no = this->getNo(id);
    auxBuscaProfundidade(no, &findG, &retorno, arquivoDeSaida, &graph); // chama a funcao auxiliar

    cout << "\n -- Arvore em Profundidade -- \n";
    for (int i = 0; i < findG.size(); i++)
        cout << findG[i] << " | ";

    cout << "\n\n -- Arestas de Retorno -- \n";
    for (int i = 0; i < retorno.size(); i++)
        cout << retorno[i] << " | ";
    cout << endl;

    printBuscaProfundidade(&graph, arquivoDeSaida);
    retorno.clear();
    findG.clear();
    graph.clear();
}

void Graph::printBuscaProfundidade(vector<string> *graph, ofstream &arquivoDeSaida)
{
    arquivoDeSaida << "graph{\n";
    for (int i = 0; i < graph->size(); i++)
        arquivoDeSaida << graph->at(i) << endl;
    arquivoDeSaida << "}";
    cout << "\npng criado como output.dot.png" << endl;
}

//! AUXILIAR METHODS --------------------------------------------------------------------------------------------

/**
 *  @brief         Funcao generica para realizar o caminho em profundidade a partir de um nó.
 *  @param no    Nó atraves do qual sera feito o caminho em profundidade.
 *
 */
void Graph::caminhoProfundidade(Node *no)
{

    no->setFoiVisitado(true); // Define o nó no como visitado.

    // Operacao recursiva para percorrer todos os nós acessiveis a partir de no.
    // Percorre todas as arestas do nó.
    for (Edge *aresta = no->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta())
    {

        // Verifica se o nó para o qual a aresta aponta foi visitado.
        // Caso nao tenha sido, caminhoProfundidade eh chamado novamente passando como parametro o nó alvo da aresta.
        if (!getNo(aresta->getIdAlvo())->getFoiVisitado())
        {

            caminhoProfundidade(getNo(aresta->getIdAlvo()));
        }
    }
}

/**
 * @brief Auxiliar da busca em profundidade de um Nó dado
 *
 * @param no      Ponteiro para o nó
 * @param findG     vetor para os vehrtices do caminho
 * @param retorno   vetor para as arestas de retorno
 * @param arquivoDeSaida   arquivo texto de saida
 * @param graf      vetor para gravacao dos dados impressos no arquivo de saida
 */
void Graph::auxBuscaProfundidade(Node *no, vector<int> *findG, vector<int> *retorno, ofstream &arquivoDeSaida, vector<string> *graf)
{

    findG->push_back(no->getId());
    no->setFoiVisitado(true);
    for (Edge *aresta = no->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta())
    {
        if (!getNo(aresta->getIdAlvo())->getFoiVisitado())
        {
            graf->push_back(to_string(no->getId()) + "--" + to_string(aresta->getIdAlvo()));
            auxBuscaProfundidade(getNo(aresta->getIdAlvo()), findG, retorno, arquivoDeSaida, graf);
        }
    }
    retorno->push_back(no->getId());
}

/**
 * @brief               Funcao auxiliar para as saidas dos algoritmos de caminho minimo, imprimindo e salvando no arquivo
 * @param   antecessor  Lista contendo os antecessores do caminho minimo, representa a ordem de acesso
 * @param   arquivoDeSaida     Arquivo de saida
 */
void Graph::caminhoMinimo(list<int> &antecessor)
{
    string arco;                                     // Para a escrita no arquivoDeSaida, se for arco '->' se for aresta '--'
    int primeiro = antecessor.front(), tNode, sNode; // Usado para armazenar o primeiro vertice, e auxiliar na escrita no arquivo dot

    if (getDirecionado())
    {
        arco = " -> ";
    }
    else
    {
        arco = " -- ";
    }
    Node *no = getPosicaoNo(primeiro);
    sNode = no->getId();
    Node *noAux = nullptr;  // No auxiliar
    Edge *aresta = nullptr; // Edge auxiliar para pegar o peso
    cout << "\nCAMINHO MINIMO\n"
         << endl;
    while (!antecessor.empty())
    {                                // Passa por toda a lista de ordem de acesso em ndo o ID
        no = getPosicaoNo(primeiro); // no recebe o no que eh o primeiro no caminho minimo
        tNode = no->getId();

        sNode = tNode; // Atualiza o valor do sNode

        cout << no->getId() << arco;   // 'Imprime' para o usuArio
        antecessor.pop_front();        // Remove o primeiro elemento da lista dos antecessor
        primeiro = antecessor.front(); // Atualiza o valor do primeiro
        noAux = no;                    // Atualiza o valor do noAux
    }
}

/**
 * @brief               Funcao auxiliar de Floy, para iniciar a matriz dos anteriores
 * @param anteriores    Matriz dos anteriores que serA inicializada
 * @param tam           Tamanho da matriz quadratica, recebe um valor i para cada posicao
 *
 */
int **Graph::iniciaAnterioresFloyd(int **anteriores, int tam)
{
    anteriores = new int *[tam]; // Aloca a matriz dos anteriores
    for (int i = 0; i < tam; i++)
        anteriores[i] = new int[tam]; // Aloca cada posicao da matriz anteriores
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            anteriores[i][j] = i; // Coloca um valor em cada posicao da matriz anteriores

    return anteriores; // Retorna a matriz anteriores
}

/**
 * @brief               Funcao auxiliar de Floyd, para iniciar a posicao da matriz de distancia contendo os pesos
 * @param   distancia   Vetor de distancia a ser inicializado
 * @param   tam         Tamanho da matriz quadratica, recebe a ordem do grafo
 *
 */
int **Graph::criaMatrizDistancia(int **distancia, int tam)
{
    distancia = new int *[tam]; // Alocando a matriz distancia
    for (int i = 0; i < tam; i++)
        distancia[i] = new int[tam]; // Aloca cada posicao da matriz distancia
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            distancia[i][j] = INF; // Atribuindo a distancia de infinito para todas a posicões
    for (int i = 0; i < tam; i++)
        distancia[i][i] = 0; // Atribuindo a distancia de 0 do vertice para ele mesmo

    Node *no = nullptr;
    Edge *aresta = nullptr;

    for (int i = 0; i < tam; i++)
    { // Colocando o peso das arestas na matriz, caso o grafo nao seja ponderado o valor 1 sera atribuido para cada salto
        no = getPosicaoNo(i);
        aresta = no->getPrimeiraAresta();

        while (aresta != nullptr)
        {
            if (!getArestaComPeso())
                distancia[i][aresta->getPosicaoAlvo()] = 1; // Adiciona 1 para cada salto caso o grafo nao seja ponderado
            else
                distancia[i][aresta->getPosicaoAlvo()] = aresta->getPeso(); // Adiciona o peso nessa posicao da matriz
            aresta = aresta->getProxAresta();                               // Avanca a aresta
        }
    }
    return distancia; // Retorna a matriz distancia
}

/**
 * @brief               Funcao auxiliar de floyd para imprimir e salvar em arquivo dot o caminho minimo
 * @param   antecessor  Lista contendo os antecessores do caminho minimo, representa a ordem de acesso
 */
void Graph::saidaFloyd(int **pred, Node *noFonte, Node *noAlvo)
{

    list<int> ordemAcesso; // Lista para conteer a ordem de acesso dos vertices, de tras para frente
    int ant;

    ordemAcesso.push_front(noAlvo->getPosicaoNo());              // Adiciona a posicao do no Target na filha
    ant = pred[noFonte->getPosicaoNo()][noAlvo->getPosicaoNo()]; // Pega a possicao do antercessor ao primeiro no e Target

    while (ant != noFonte->getPosicaoNo())
    { // Loop que adciona na lista todas as posicões dos nos do menor caminho
        ordemAcesso.push_front(ant);
        ant = pred[noFonte->getPosicaoNo()][ant];
    }
    ordemAcesso.push_front(noFonte->getPosicaoNo());

    caminhoMinimo(ordemAcesso);
}

/**
 * @brief               Funcao auxiliar de disjkstra para imprimir o caminho
 * @param   antecessor  Vetor contendo o antecessor de cada posicao
 * @param   idNoFonte    ID do nó de origem
 * @param   idNoAlvo    ID do nó de destino
 */
void Graph::saidaDijkstra(int antecessor[], int idNoFonte, int idNoAlvo)
{

    string arco;
    int noAnterior, primeiro, tNode, sNode; // Usado para armazenar o vertice anterior, e auxiliar na escrita no arquivo dot

    list<int> ordemAcesso; // Lista contendo a ordem de acesso dos vertices

    ordemAcesso.push_front(idNoAlvo);  // Armazena na lista na ordem de acesso dos vertices,
    noAnterior = antecessor[idNoAlvo]; // apartir de seus anteriores, comecando pelo nó target

    while (noAnterior != idNoFonte)
    {
        ordemAcesso.push_front(noAnterior);
        noAnterior = antecessor[noAnterior];
    }
    ordemAcesso.push_front(idNoFonte); // Insere o nó Source como o primeiro a ser acessado
    primeiro = ordemAcesso.front();

    caminhoMinimo(ordemAcesso);
}