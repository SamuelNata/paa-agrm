#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include "Edge.h"
#include "Graph.h"

/**
Metodo Creator
**/
Graph::Graph(int _numVertices){
    numArestas = 0;
    numCores = 0;
    dencidade = 0;
    generated = false;
    numVertices = _numVertices;
    bestSolution = NULL;
    adjacencyMatrix = new Edge *[_numVertices];
    for( int i(0) ; i<numVertices ; i++ ){
        adjacencyMatrix[i] = new Edge [numVertices];
    }
}

/**
Metodo Destructor
**/
Graph::~Graph(){
    for( int i(0) ; i<numVertices ; i++ ){
        delete [] (adjacencyMatrix[i]);
    }
    delete [] adjacencyMatrix;
    if( bestSolution!=NULL ){
        delete [] bestSolution;
    }
}

/**
Metodo utilizado para gerar uma instancia com "_numCores" cores e "_dencidade" dencidade de arestas
Garante que o grafo é conexo pois gera uma arvore geradora para então gerar o grafo final.
**/
void Graph::generateInstance( int _numCores, float _dencidade ){
    this->clearEdges();
    numCores = _numCores;
    dencidade = _dencidade;

    //INICIALIZA AS ARESTAS
    for( int i(0) ; i<numVertices ; i++ ){
        for( int j(0) ; j<numVertices ; j++ ){
            adjacencyMatrix[i][j].setNumCollors(numCores);
        }
    }

    numArestas = dencidade*(numVertices-1)*numVertices/2; // CALCULO DO NUMERO DE ARESTAS DE ACORDO COM A AS ESPECIFICAÇÕES DE [Cerulli 2005]
    int linha, coluna, collorQuantity;
    srand(time(NULL)+rand());

    std::vector<int> verticesInTree;    //REPRESENTA OS VERTICES JÁ PRESENTES NA ARVORE GERADORA
    std::vector<int> verticesOutTree;   //REPRESENTA OS VERTICES NÃO PRESENTES NA ARVORE GERADORA
    for( int i(0) ; i<numVertices ; i++ ){ //INICIALIZA O CONJUNTO DE VERTICES NÃO PRESENTES NA ARVORE GERADORA
        verticesOutTree.push_back(i);
    }

    //VERTICES QUE SERÃO ADJACENTES NO GRAFO
    int vertice1 = verticesOutTree[rand()%numVertices];
    int vertice2 = verticesOutTree[rand()%numVertices];

    while( vertice2==vertice1 ){    vertice2 = verticesOutTree[rand()%numVertices];    }
    collorQuantity = 1 + rand()%(1+numCores/10);
    for( int k(0) ; k<collorQuantity ; k++ ){ // PREENCHER A PRIMEIRA ARESTA COM UM NÚMERO ARBITRARIO DE CORES (NO MAXIMO numCores/10 CORES)
        adjacencyMatrix[vertice1][vertice2].addCollor(rand()%numCores);
    }

    //COLOCA A ARESTA QUE LIGA vertice1 A vertice2 NO GRAFO
    adjacencyMatrix[vertice2][vertice1] = adjacencyMatrix[vertice1][vertice2];

    //ATUALIZA OS CONJUNTOS DE VERTICES
    std::vector<int>::iterator it;
    for( it = verticesOutTree.begin() ; *it!=vertice1 ; it++ ){}
    verticesOutTree.erase(it);
    for( it = verticesOutTree.begin() ; *it!=vertice2 ; it++ ){}
    verticesOutTree.erase(it);
    verticesInTree.push_back(vertice1);
    verticesInTree.push_back(vertice2);

    for( int i(1) ; i<numArestas ; ){ // PREENCHER O GRAFO COM AS numArestas ARESTAS RESTANTES

        if( i<numVertices-1 ){
            vertice1 = verticesInTree[rand()%verticesInTree.size()];
            vertice2 = verticesOutTree[rand()%verticesOutTree.size()];
            collorQuantity = 1 + rand()%(1+numCores/10);
            for( int k(0) ; k<collorQuantity ; k++ ){ // PREENCHER A ARESTA COM UM NÚMERO ARBITRARIO DE CORES (NO MAXIMO numCores/10 CORES)
                adjacencyMatrix[vertice1][vertice2].addCollor(rand()%numCores);
            }
            //COLOCA A ARESTA QUE LIGA vertice1 A vertice2 NO GRAFO
            adjacencyMatrix[vertice2][vertice1] = adjacencyMatrix[vertice1][vertice2];
            //ATUALIZA OS CONJUNTOS DE VERTICES
            for( it = verticesOutTree.begin() ; *it!=vertice2 ; it++ ){}
            verticesOutTree.erase(it);
            verticesInTree.push_back(vertice2);
            i++;
        }
        else{ // APOR TER UMA ARVORE GERADORA, TERMINA DE ADICIONAS AS ARESTAS RESTANTES PARA FORMAR O GRAFO
            linha=rand()%numVertices;
            coluna=rand()%numVertices;
            while( adjacencyMatrix[linha][coluna].isActive && linha!=coluna ){
                linha=rand()%numVertices;
                coluna=rand()%numVertices;
            }

            collorQuantity = 1 + rand() % (1+ numCores/10);
            for( int k(0) ; k<collorQuantity ; k++ ){ // PREENCHER CADA ARESTA COM UM NÚMERO ARBITRARIO DE CORES (NO MAXIMO numCores CORES)
                adjacencyMatrix[linha][coluna].addCollor(rand()%numCores);
            }
            adjacencyMatrix[coluna][linha] = adjacencyMatrix[linha][coluna];
            i++;
        }
    }
    generated = true;
}

/**
Metodo utilizado para salvar uma instancia gerada em arquivo de texto (que pode ser liga mais tarde)
**/
void Graph::printInstanceToFile( std::string fileName ){
    std::ofstream file;
    file.open( (fileName + ".txt").c_str() );

    file << numVertices << " " << numCores << " " << numArestas <<"\n"; // FORMATO: n. de vertices, n. de cores, n. de arestas

    //IMPRIME A LISTA DE ARESTAS ( aresta do vertice A ao vertice B, com as cores 2 e 0, dentre 5 cores é printado no arquivo como: A B 1 0 1 0 0 )
    for( int i(0) ; i<numVertices ; i++ ){
        for( int j(0) ; j<i ; j++ ){
            if( adjacencyMatrix[i][j].isActive ){
                file << i << " " << j << " ";
                for( int k(0) ; k<numCores ; k++ ){
                    file << adjacencyMatrix[i][j].hasCollor(k) << " ";
                }
                file << "\n";
            }
        }
    }
    file.close();
}


/**
Printa a instancia do Grafo no terminal
**/
void Graph::print(){
    std::cout << numVertices << " " << numCores << " " << numArestas <<"\n"; // FORMATO: n. de vertices, n. de cores, n. de arestas

    //PRINTA A LISTA DE ARESTAS ( aresta do vertice A ao vertice B, com as cores 2 e 0, dentre 5 cores é printado como: A B 1 0 1 0 0 )
    for( int i(0) ; i<numVertices ; i++ ){
        for( int j(0) ; j<i ; j++ ){
            if( adjacencyMatrix[i][j].isActive ){
                std::cout << i << " " << j << " ";
                for( int k(0) ; k<numCores ; k++ ){
                    std::cout << adjacencyMatrix[i][j].hasCollor(k) << " ";
                }
                std::cout << "\n";
            }
        }
    }
}

/**
Lê a instancia de um grafo a partir de um arquivo (O arquivo deve estar no modelo utilizado na função printInstanceToFile() )
**/
void Graph::readInstance( std::string fileName ){
    std::string line;
    std::ifstream file( fileName.c_str() );
    int begin, end;
    if( file.is_open() && getline(file,line) ){

        begin = 0;
        end = line.find(" ");
        if( numVertices!=atoi( line.substr(begin, end).c_str() ) ){
            std::cout << "Você ta inserindo um grafo com um número diferente de vertices\nO numero deve ser igual ao do grafo criado.";
            return;
        }

        begin = end+1;
        end = line.find(" ", begin);
        numCores = atoi( line.substr(begin, end).c_str() );

        begin = end+1;
        end = line.find(" ", begin);
        numArestas = atoi( line.substr(begin, end).c_str() );

        for( int i(0) ; i<numVertices ; i++ ){
            for( int j(0) ; j<numVertices ; j++ ){
                adjacencyMatrix[i][j].setNumCollors(numCores);
            }
        }

        int linha, coluna;
        for( int i(0) ; i<numArestas && getline(file,line) ; i++ ){
            begin = 0;
            end = line.find(" ");
            linha = atoi( line.substr(begin, end).c_str() );

            begin = end+1;
            end = line.find(" ", begin);
            coluna = atoi( line.substr(begin, end).c_str() );

            for( int k(0) ; k<numCores ; k++ ){
                begin = end+1;
                end = line.find(" ", begin);
                if( atoi(line.substr(begin, end).c_str())==1 ){
                    adjacencyMatrix[linha][coluna].addCollor(k);
                }
            }
            adjacencyMatrix[coluna][linha] = adjacencyMatrix[linha][coluna];
        }
        file.close();
    }
    else{
        std::cout << "(" << fileName << ") Esse arquivo existe mesmo? Escreva esse negocio direito homi.\n";
    }
}

/**
Limpa toda as arestas do grafo (remove as corer associadas a aresta, e as cores que foram pintadas na aresta)
**/
void Graph::clearEdges(){
    for( int i(0) ; i<numVertices ; i++ ){
        for( int j(0) ; j<numVertices ; j++ ){
            for( int k(0) ; k<numCores ; k++ ){
                adjacencyMatrix[i][j].clearCollor(k);
            }
        }
    }
}


/**
1 - Realiza os preparativos para a resolução do problema da Arvore Geradora de Rotulação Minima
2 - Chama o algoritmo de Branch-And-Bound para a resolução da Arvore Geradora de Rotulação Minima
3 - Constroi a Arvore Geradora de Rotulação Minima com base no conjunto de cores encontrado

C = numero de cores
N = numero de vertices
M = numero de arestas
**/
void Graph::solve(){
    // 1 - REALIZANDO PREPARATIVOS
    clock_t end, begin = clock();
    if( bestSolution==NULL ){
        bestSolution = new bool[numCores];
    }
    int * collorCount = new int [numCores];
    for( int i(0) ; i<numCores ; i++ ){     //INICIALIZANDO VETOR DE CONTAGEM DAS CORES PRESENTES NAS ARESTAS DA ARVORE, E O VETOR QUE CONTEM AS CORES DA SOLUÇÃO
        collorCount[i] = 0;                 // O(C)
        bestSolution[i] = true;
    }
    bestNumCollors = numCores;

    int corMaisPresente = 0;
    bool * verticesInTree = new bool [numVertices];
    for( int i(1) ; i<numVertices ; i++ ){          // INICIALIZANDO VETOR QUE MARCA A PRESENÇA DOS VERTICES NA ARVORE E
        for( int j(0) ; j<i ; j++ ){                // CONTA AS ARESTAS POR COR, SEPARA A COR MAIS FREQUENTE
            for( int k(0) ; k<numCores ; k++ ){         //O(N^2*C)
                if( adjacencyMatrix[i][j].hasCollor(k) && ++collorCount[k]>collorCount[corMaisPresente] ){
                    corMaisPresente = k;
                }
            }
        }
        verticesInTree[i] = false;
    }
    verticesInTree[0] = false;

    // 2 - ALGORITMO QUE REALIZA O BRANCH-AND-BOUND
    solveAGRM(collorCount, verticesInTree, corMaisPresente, 0, 0); //O(C!*N^2)

    // 3 - MONTANDO ARVORE SOLUÇÃO
    verticesInTree = new bool[numVertices];
    for( int i(0) ; i<numVertices ; i++ ){
        verticesInTree[i] = false;
    }

    //CONJUNTO DE CORES UTILIZADAS NA SOLUÇÃO
    std::vector<int> usedCollors;
    for( int i(0) ; i<numCores ; i++ ){ //INICIALIZA O CONJUNTO DE CORES UTILIZADAS NA SOLUÇÃO
        if(bestSolution[i]){            //O(C)
            usedCollors.push_back(i);
        }
    }

    bool firstInsert = true;
    int arestas = 0;
    bool finish = false;
    for( int i(1) ; i<numVertices && !finish ; i++ ){   //PINTA, SEM FORMAR CICLOS, AS ARESTAS QUE POSSUEM ALGUMA DAS CORES UTILIZADA NA SOLUÇÃO
        for( int j(0) ; j<i && !finish ; j++ ){         //O(N^2*C)
            if( !adjacencyMatrix[i][j].isActive ){
                continue;
            }
            for( int k(0) ; k<usedCollors.size() ; k++ ){
                if( adjacencyMatrix[i][j].hasCollor(usedCollors[k]) &&
                (verticesInTree[i]!=verticesInTree[j] || firstInsert) ){
                    firstInsert = false;
                    verticesInTree[i] = true;
                    verticesInTree[j] = true;
                    arestas++;
                    adjacencyMatrix[i][j].paint(usedCollors[k]);
                    adjacencyMatrix[j][i].paint(usedCollors[k]);
                    i=1;
                    j=0;
                }
                if( arestas == numVertices-1 ){
                    finish = true;
                    break;
                }
            }
        }
    }
    if( !finish ){
        std::cout << "vem k\n";
    }
    delete [] collorCount;
    delete [] verticesInTree;
    end = clock();
    solutionTime = ((end-begin)*1000.0)/CLOCKS_PER_SEC;
    return;
}


/**
Algoritmo que realiza o Branch-And-Bound nas cores buscando otimizar o número de cores utilizadas.
**/
void Graph::solveAGRM( int * collorCount, bool * verticesInTree, int corMaisPresente, int numUsedCollors, int numUsedEdges ){
    //NESTE PASSO SERÁ INSERIDA UMA COR NA SOLUÇÃO
    numUsedCollors++;

    //REALIZA UMA PROJEÇÃO DA QUANTIDADE MINIMA DE CORES NECESSARIAS PARA ENCONTAR UMA SOLUÇÃO
    //bigger = indice da cor mais utilizada nas arestas restantes da arvore da projeção
    //numEdgesProjection = projeção do numero de arestas que serão inseridas
    //numCollorProjeciton = projeção do numero de cores que serão inseridas
    int bigger = 0 , numEdgesProjection = 0, numCollorProjection = 0;
    int * tempProjection = new int[numCores];
    for( int i(0) ; i<numCores ; i++ ){ // INICIA O VETOR DE PROJEÇÃO DA CONTAGEM DAS CORES
        tempProjection[i] = collorCount[i];
    }
    while( numUsedEdges+numEdgesProjection<numVertices-1 ){ // ADICIONA A COR QUE POSSUI O MAIOR NUMERO DE ARESTAS ATÉ COMPLETAR N-1 ARESTAS NA ARVORE
        for( int i(0) ; i<numCores ; i++ ){ //RECONHECE A COR QUE MAIS APARECE NA ARVORE
            bigger = (tempProjection[i]>=0 && tempProjection[i]>tempProjection[bigger])? i : bigger;
        }
        numEdgesProjection += tempProjection[bigger];       //ATUALIZA PROJEÇÃO DA QUANTIDADE DE ARESTAS
        tempProjection[bigger] = -1;                        //ATUALIZA PROJEÇÃO DA CONTAGEM DAS CORES
        numCollorProjection++;                              //ATUALIZA PROJEÇÃO DO NUMERO DE CORES NA SOLUÇÃO
    }

    if(  numUsedCollors+numCollorProjection>=bestNumCollors || numUsedEdges>=numVertices ){    return; } //VERIFICA SE É POSSIVEL ENCONTRAR UMA SOLUÇÃO MELHOR POR ESSE CAMINHO

    int * nextCount;
    bool * nextVerticesInTree;
    int c = corMaisPresente;
    int localNumArestas;


    for( int k(0) ; k<numCores ; k++, c=(c+1)%numCores ){ //TENTA INSERIR CADA UMA DAS CORES, COMEÇANDO PELA QUE MAIS APARECE NAS ARESTAS
        if( collorCount[c]<1 ){   continue; } // SE A COR JÁ FAZ PARTE DA SOLUÇÃO, PULA PARA A PROXIMA COR
        localNumArestas = 0;
        nextCount = new int[numCores];
        for( int t(0) ; t<numCores ; t++ ){ //INICIALIZA A CONTAGEM DE CORES PARA O PROXIMO PASSO DO BRANCH-AND-BOUND
            nextCount[t] = collorCount[t];
        }
        nextCount[c] = -1;

        nextVerticesInTree = new bool[numVertices]; // INICIALIZA A PRESENÇA DOS VERTICES PARA O PROXIMO PASSO DO BRANCH-AND-BOUND
        for(int i(0) ; i<numVertices ; i++ ){
            nextVerticesInTree[i] = verticesInTree[i];
        }

        for( int i(1) ; i<numVertices ; i++ ){  //PARA A COR ATUAL (c)
            for( int j(0) ; j<i ; j++ ){    //INSERI AS ARESTAS QUE ALGUMA COR DA SOLUÇÃO NA ARVORE, SEM FORMAR CICLOS
                if( (nextVerticesInTree[i]!=nextVerticesInTree[j] || numUsedEdges+localNumArestas==0) &&   // ADICIONA UM VERTICE QUE NÃO ESTAVA NA ARVORE (NAO FORMA CICLO)
                   adjacencyMatrix[i][j].hasCollors(nextCount) )
                {
                    nextVerticesInTree[i] = true;
                    nextVerticesInTree[j] = true;
                    localNumArestas++;
                    for( int t(0) ; t<numCores ; t++ ){
                        if( adjacencyMatrix[i][j].hasCollor(t) ){
                            nextCount[t]--;
                        }
                    }
                    i=1;
                    j=0;
                }
            }
        }

        //SE O NUMERO DE ARESTAS ATUAL É N-1 TEMOS UMA ARVORE GERADORA (SOLUÇÃO)
        if( numUsedEdges+localNumArestas==numVertices-1 ){
            if( numUsedCollors<bestNumCollors ){ //SE A SOLUÇÃO É MELHOR DO QUE A QUE JÁ TEMOS
                for( int i(0) ; i<numCores  ; i++ ){ //GUARDA A SOLUÇÃO. O(C)
                    bestSolution[i] = (nextCount[i]<0);
                }
                bestNumCollors = numUsedCollors; //ATUALIZA O NUMERO DE CORES DA SOLUÇÃO
                return;
            }
            return;
        }

        int nextCorMaisPresente = 0;
        for( int i(0) ; i<numCores ; i++ ){ //RECALCULA A COR MAIS PRESENTE NAS ARESTAS DO GRAFO
            if( nextCount[i]>nextCount[nextCorMaisPresente] ){
                nextCorMaisPresente = i;
            }
        }

        if( numUsedCollors<bestNumCollors-1 && numUsedCollors<numCores ){ //SE AINDA É POSSIVEL CONSEGUIR UMA SOLUÇÃO MELHOR, REALIZA O PROXIMO PASSO DO BRANCH-AND-BOUND
            solveAGRM( nextCount, nextVerticesInTree, nextCorMaisPresente, numUsedCollors, numUsedEdges+localNumArestas );
        }
        else{
            delete [] nextCount;
            delete [] nextVerticesInTree;
        }
    }

    delete [] collorCount;
    delete [] verticesInTree;
    return;
}

/**
Imprime o relatorio da solução no terminal.
[Use a funcao Graph::printToFileSolutionReport() para imprimir a arvore gerada em em arquivo.]
**/
void Graph::printSolutionReport( std::string instanceName ){
    std::cout << "Nome da instancia: " << instanceName << std::endl;
    std::cout << "Numero Minimo de Cores: " << bestNumCollors << " cores de " << numCores << ".\n";
    std::cout << "Conjunto de cores: ";
    for( int i(0) ; i<numCores ; i++ ){
        if( bestSolution!=NULL && bestSolution[i] ){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "tempo de solucao: " << solutionTime << " segundos.\n\n";
}

/**
Imprime o relatorio da solução em arquivo de texto.
**/
void Graph::printToFileSolutionReport( std::string fileName ){
    std::ofstream file;
    file.open( (fileName + "Solution.txt").c_str() );
    std::stringstream tree;
    int arestas = 0;

    //LISTA DE ARESTAS PINTADAS ( aresta do vertice A ao vertice B, com as cores 2 e 0 pintadas, dentre 5 cores: A B 1 0 1 0 0 )
    for( int i(1) ; i<numVertices ; i++ ){
        for( int j(0) ; j<i ; j++ ){
            if( adjacencyMatrix[i][j].isPainted() ){
                arestas++;
                tree << i << " " << j << " ";
                for( int k(0) ; k<numCores ; k++ ){
                    tree << adjacencyMatrix[i][j].isPaintedCollor(k) << " ";
                }
                tree << "\n";
            }
        }
    }
    if( arestas!=numVertices-1 ){   std::cout << "DEU MERDA, vertices " << numVertices << ", arestas " << arestas << std::endl; }
    file << numVertices << " " << numCores << " " << arestas <<"\n"; // FORMATO: n. de vertices, n. de cores, n. de arestas
    file << tree.str();
    file << "\n";
    file << "Nome da instancia: " << fileName << std::endl;
    file << "Numero Minimo de Cores: " << bestNumCollors << " cores de " << numCores << ".\n";
    file << "Conjunto de cores: ";
    for( int i(0) ; i<numCores ; i++ ){
        if( bestSolution!=NULL && bestSolution[i] ){
            file << i << " ";
        }
    }
    file << std::endl;
    file << "tempo de solucao: " << solutionTime << " segundos.\n";

    file.close();

    // IMPRIMINDO SOLUÇÃO NO RELATORIO GERAL PARA A GERAÇÃO DE TABELAS

    file.open("testBanchSolutions.txt.", std::ios::app);
    file << numVertices << "\t" << numCores << "\t" << numArestas << "\t" << bestNumCollors << "\t" << solutionTime << "\n";

    file.close();
}

/**
Vetifica se:
1 - O grafo inicial era conexo
2 - A solução encontrada é conexa
3 - Se todas as arestas no grafo podiam ser pintadas da cor que foram pintadas
4 - Se asolução é livre de ciclos
**/
bool Graph::validateSolution(){
    if( !this->isConexGraph() ){
        std::cout << "O grafo gerado nao e conexo.\n";
        return false;
    }
    if( !this->isConexSolution() ){
        std::cout << "A solucao gerada nao e conexa.\n  ";
        return false;
    }
    int arestas = 0;
    for( int k(0) ; k<numCores ; k++ ){
        for( int i(0) ; i<numVertices ; i++ ){
            for( int j(0) ; j<i ; j++ ){
                if( adjacencyMatrix[i][j].isPaintedCollor(k) ){
                    arestas++;
                    if( !adjacencyMatrix[i][j].hasCollor(k) ){
                        std::cout << "como ta pintado se nem ta na arvore?\n";
                        return false;
                    }
                    if( arestas>numVertices-1 ){
                        std::cout << "arestas D+, n é arvore.\n";
                        return false;
                    }
                }
            }
        }
    }
}

/**
vetifica se a instancia do grafo é conexa
**/
bool Graph::isConexGraph( ){
    bool ingraph[numVertices];
    for(int i(0) ; i<numVertices ; i++ ){
        ingraph[i] = false;
    }
    ingraph[0] = true;
    risConexGraph(0, ingraph);
    for(int i(0) ; i<numVertices ; i++ ){
        if( !ingraph[i] ){  return false;   }
    }
    return true;
}

/**
Verifica se a solução encontrada é conexa
**/
bool Graph::isConexSolution( ){
    bool ingraph[numVertices];
    for(int i(0) ; i<numVertices ; i++ ){
        ingraph[i] = false;
    }
    ingraph[0] = true;
    risConexSolution(0, ingraph);
    for(int i(0) ; i<numVertices ; i++ ){
        if( !ingraph[i] ){
            std::cout << i << "-esimo vertice nao foi marcado\n";
            return false;
        }
    }
    return true;
}

/**
Algoritmo recursivo utilizado por Graph::isConexGraph( )
**/

void Graph::risConexGraph( int vertice, bool ingraph[] ){
    for(int i(0) ; i<numVertices ; i++ ){
        if( (adjacencyMatrix[vertice][i].isActive || adjacencyMatrix[i][vertice].isActive) && !ingraph[i] ){
            ingraph[i] = true;
            this->risConexGraph(i, ingraph);
        }
    }
    return;
}

/**
Algoritmo recursivo utilizado por Graph::isConexSolution( )
**/
void Graph::risConexSolution( int vertice, bool ingraph[] ){
    for(int i(0) ; i<numVertices ; i++ ){
        if( (adjacencyMatrix[vertice][i].isPainted() || adjacencyMatrix[i][vertice].isPainted()) && !ingraph[i] ){
            ingraph[i] = true;
            this->risConexGraph(i, ingraph);
        }
    }
    return;
}




