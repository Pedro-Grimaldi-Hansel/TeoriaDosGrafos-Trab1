/**************************************************************************************************
 * Implementation of the TAD Node
**************************************************************************************************/

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
    private:
        Edge* primeiraAresta;
        Edge* ultimaAresta;
        int id;
        int posicaoNo;                   //para guarda a posição de inserção do no
        unsigned int grauEntrada;
        unsigned int grauSaida;
        int grau;
        float peso;
        Node* proxNo;
        bool foiVisitado; // Atributo para definir se o nó foi visitado ou não.

    public:
        // Constructor
        Node(int id);
        // Destructor
        ~Node();
        // Getters
        Edge* getPrimeiraAresta();
        Edge* getUltimaAresta();
        int getId();
        int getGrauEntrada();
        int getGrauSaida();
        int getGrau();
        float getPeso();
        Node* getProxNo();
        bool getFoiVisitado(); // Retorna o atributo foiVisitado;
        // Setters
        void setProxNo(Node* node);
        void setPeso(float peso);
        void setFoiVisitado(bool foiVisitado); // Define se o nó foi visitado ou não.
        // Other methods
        bool procuraAresta(int idAlvo);
        void insereAresta(int idAlvo, int posicaoAlvo,float peso);
        void removeTodasArestas();
        int removeAresta(int id, bool direcionado, Node* target_node);
        void incrementaGrauEntrada();
        void decrementaGrauEntrada();
        void incrementaGrauSaida();
        void decrementaGrauSaida();
        Edge* getAresta(int idAlvo);
        // Auxiliar methods
        int getPosicaoNo();
        void setPosicaoNo(int posicaoNo);
};

#endif // NODE_H_INCLUDED
