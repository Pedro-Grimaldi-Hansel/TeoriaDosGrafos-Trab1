/**************************************************************************************************
 * Implementation of the TAD Edge
**************************************************************************************************/

#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

// Definition of the Edge class
class Edge{

    // Attributes
    private:
        int idAlvo;
        int posicaoAlvo;            //posição do nó target
        Edge* proxAresta;
        float peso;

    public:
        // Constructor
        Edge(int idAlvo, int posicaoAlvo);
        // Destructor
        ~Edge();
        // Getters
        int getIdAlvo();
        int getPosicaoAlvo();
        Edge* getProxAresta();
        float getPeso();
        // Setters
        void setProxAresta(Edge* edge);
        void setPeso(float peso); 

}; 

#endif // EDGE_H_INCLUDED