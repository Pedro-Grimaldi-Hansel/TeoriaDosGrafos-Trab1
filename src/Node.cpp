#include "Node.h"
#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 //! Defining the Node's methods
**************************************************************************************************/

//! Constructor
Node::Node(int id)
{
    this->id = id;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->peso = 0;
    this->primeiraAresta = nullptr;
    this->ultimaAresta = nullptr;
    this->proxNo = nullptr;
    this->foiVisitado = false;
};

//! Destructor
Node::~Node()
{

    Edge *proxAresta = this->primeiraAresta;

    while (proxAresta != nullptr)
    {
        Edge *aux_edge = proxAresta->getProxAresta();
        delete proxAresta;
        proxAresta = aux_edge;
    }
};

//! Getters
Edge *Node::getPrimeiraAresta()
{
    return this->primeiraAresta;
}

Edge *Node::getUltimaAresta()
{

    return this->ultimaAresta;
}

int Node::getId()
{

    return this->id;
}
int Node::getPosicaoNo()
{

    return this->posicaoNo;
}
void Node::setPosicaoNo(int posicaoNo)
{
    this->posicaoNo = posicaoNo;
}

int Node::getGrauEntrada()
{
    return this->grauEntrada;
}

int Node::getGrauSaida()
{
    return this->grauSaida;
}

int Node::getGrau()
{
    return this->grau;
}

float Node::getPeso()
{
    return this->peso;
}

Node *Node::getProxNo()
{
    return this->proxNo;
}

bool Node::getFoiVisitado()
{
    return foiVisitado;
}

//! Setters

void Node::setProxNo(Node *proxNo)
{
    this->proxNo = proxNo;
}

void Node::setPeso(float peso)
{
    this->peso = peso;
}

void Node::setFoiVisitado(bool v)
{
    this->foiVisitado = v;
}

//! Other methods

void Node::insereAresta(int idAlvo, int posicaoAlvo, float peso)
{
    // Verifica se há pelo menos uma aresta no nó
    if (this->primeiraAresta != nullptr)
    {
        // Alocando a nova aresta e mantendo a integridade da lista de aresta
        Edge *aresta = new Edge(idAlvo, posicaoAlvo);
        aresta->setPeso(peso);
        this->ultimaAresta->setProxAresta(aresta);
        this->ultimaAresta = aresta;
    }
    else
    {
        // Alocando a nova aresta e mantendo a integridade da lista de aresta
        this->primeiraAresta = new Edge(idAlvo, posicaoAlvo);
        this->primeiraAresta->setPeso(peso);
        this->ultimaAresta = this->primeiraAresta;
    }
}

void Node::removeTodasArestas()
{
    // Verifica se tem pelo menos uma aresta no Nó
    if (this->primeiraAresta != nullptr)
    {
        Edge *prox = nullptr;
        Edge *aux = this->primeiraAresta;
        // Remove todas as arestas
        while (aux != nullptr)
        {
            prox = aux->getProxAresta();
            delete aux;
        }
    }

    this->primeiraAresta = this->ultimaAresta = nullptr;
}

int Node::removeAresta(int id, bool direcionado, Node *noAlvo)
{
    // Verifica se a Aresta existe
    if (this->procuraAresta(id))
    {
        Edge *aux = this->primeiraAresta;
        Edge *previous = nullptr;

        // Procurando a Aresta para remover
        while (aux->getIdAlvo() != id)
        {
            previous = aux;
            aux = aux->getProxAresta();
        }

        //Manter integridade da lista de aresta
        if (previous != nullptr)
            previous->setProxAresta(aux->getProxAresta());

        else
            this->primeiraAresta = aux->getProxAresta();

        if (aux == this->ultimaAresta)
            this->ultimaAresta = previous;

        if (aux->getProxAresta() == this->ultimaAresta)
            this->ultimaAresta = aux->getProxAresta();

        delete aux;
        // Verifica se o grafo é direcionado
        if (direcionado)
            this->decrementaGrauSaida();
        else
        {
            this->decrementaGrauEntrada();
            noAlvo->decrementaGrauEntrada();
        }

        return 1;
    }

    return 0;
}

bool Node::procuraAresta(int idAlvo)
{
    // Verifica se há uma Aresta no Nó.
    if (this->primeiraAresta != nullptr)
    {
        // Procurando por uma Aresta especifica do id informado
        for (Edge *aux = this->primeiraAresta; aux != nullptr; aux = aux->getProxAresta())
            if (aux->getIdAlvo() == idAlvo)
                return true;
    }

    return false;
}

void Node::incrementaGrauEntrada()
{
    this->grauEntrada++;
}

void Node::incrementaGrauSaida()
{
    this->grauSaida++;
}

void Node::decrementaGrauEntrada()
{
    this->grauEntrada--;
}

void Node::decrementaGrauSaida()
{
    this->grauSaida--;
}

Edge *Node::getAresta(int idAlvo)
{
    for (Edge *auxEdge = this->primeiraAresta; auxEdge != nullptr; auxEdge = auxEdge->getProxAresta())
    {
        if (auxEdge->getIdAlvo() == idAlvo)
            return auxEdge;
    }
    return nullptr;
}
