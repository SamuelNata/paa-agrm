#ifndef GRAPH_H
#define GRAPH_H
#include "Edge.h"

class Graph
{
    public:
        Graph( int _numVertices );
        ~Graph();
        void generateInstance( int _numCores, float _dencidade );
        void readInstance( std::string fileName );
        void printInstanceToFile( std::string fileName );
        void clearEdges();
        void solve();
        void solveAGRM( int * collorCount, bool * verticesInTree, int corMaisPresente, int numUsedCollors, int numUsedEdges );
        void printSolutionReport( std::string instanceName );
        void printToFileSolutionReport( std::string fileName );
        bool validateSolution();
        bool isConexGraph( );
        bool isConexSolution( );
        void print();

    private:
        void solveAGRM();
        void risConexGraph( int vertice, bool ingraph[] );
        void risConexSolution( int vertice, bool ingraph[] );

        int numVertices;
        int numCores;
        int numArestas;
        int bestNumCollors;
        bool * bestSolution;
        float dencidade;
        bool generated;
        double solutionTime;
        Edge ** adjacencyMatrix;
};

#endif // GRAPH_H
