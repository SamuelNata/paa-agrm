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
        void printSolutionReport( std::string instanceName );
        void printToFileSolutionReport( std::string fileName );
        void completFileSolutionReportLine( std::string fileName );
        void print();

        void exactSolve();
        void exactSolveAGRM( int * collorCount, bool * verticesInTree, int corMaisPresente, int numUsedCollors, int numUsedEdges );
        void heurisctSolve( float alfa, float beta );

        bool validateSolution();
        void findCase();

    private:
        int projection( int numUsedEdges, int * collorCount );
        void clearEdges();
        void initialSolution(float a);
        int generateCollor( int alfa, int corMaisPresente );
        bool localSearch( );
        bool isConexGraph( );
        bool isConexSolution( );
        void risConexGraph( int vertice, bool ingraph[] );
        void risConexSolution( int vertice, bool ingraph[] );
        bool colorir( bool * verticesInTree, int * collorCount, const int numUsedArrestas, int & localNumUsedArestas );
        bool persistCollorsInTree();
        void unpaint();
        void mountSolution();
        void disturb( float alfa);

        int numVertices;
        int numCores;
        int numArestas;
        int bestNumCollors;
        bool * bestSolution;
        float dencidade;
        bool generated;
        double solutionTime;
        Edge ** adjacencyMatrix;
        bool * candidateSolution;
};

#endif // GRAPH_H
