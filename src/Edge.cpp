#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Edge's methods
 **************************************************************************************************/

// Constructor
Edge::Edge(int idAlvo, int posicaoAlvo)
{
    this->idAlvo = idAlvo;
    this->proxAresta = nullptr;
    this->peso = 0;
    this->posicaoAlvo = posicaoAlvo;
}

// Destructor
Edge::~Edge()
{
    if (this->proxAresta != nullptr)
    {
        delete this->proxAresta;
        this->proxAresta = nullptr;
    }
}

// Getters
int Edge::getIdAlvo()
{
    return this->idAlvo;
}
int Edge::getPosicaoAlvo()
{
    return this->posicaoAlvo;
}

Edge *Edge::getProxAresta()
{
    return this->proxAresta;
}

float Edge::getPeso()
{
    return this->peso;
}

// Setters
void Edge::setProxAresta(Edge *aresta)
{
    this->proxAresta = aresta;
}

void Edge::setPeso(float peso)
{
    this->peso = peso;
}
