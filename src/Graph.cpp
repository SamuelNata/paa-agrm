#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <math.h>
#include "Edge.h"
#include "Graph.h"

int level=0;

/**
DEGUB: serve para imprimir as mensagens de debug em formato de arvore
**/
void deslocLevel(){
    for( int i(0) ; i<level ; i++ ){
        std::cout << "\t";
    }
}

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
    adjacencyMatrix[vertice1][vertice2].addCollor(rand()%numCores);

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

    int cor=0;
    for( int i(1) ; i<numArestas ; i++ ){ // PREENCHER O GRAFO COM AS numArestas ARESTAS RESTANTES
        if( i<numVertices-1 ){ // SE AINDA NAO FOI FORMADA UMA ARVORE (PARA QUE O GRAFO SEJA CONEXO)
            vertice1 = verticesInTree[rand()%verticesInTree.size()];
            vertice2 = verticesOutTree[rand()%verticesOutTree.size()];
            adjacencyMatrix[vertice1][vertice2].addCollor(rand()%numCores);

            //COLOCA A ARESTA QUE LIGA vertice1 A vertice2 NO GRAFO
            adjacencyMatrix[vertice2][vertice1] = adjacencyMatrix[vertice1][vertice2];

            //ATUALIZA OS CONJUNTOS DE VERTICES
            for( it = verticesOutTree.begin() ; *it!=vertice2 ; it++ ){}
            verticesOutTree.erase(it);
            verticesInTree.push_back(vertice2);
        }
        else{ // APOS TER UMA ARVORE GERADORA, TERMINA DE ADICIONAS AS ARESTAS RESTANTES PARA FORMAR O GRAFO
            linha=rand()%numVertices;
            coluna=rand()%numVertices;
            while( adjacencyMatrix[linha][coluna].isActive || linha==coluna ){
                linha=rand()%numVertices;
                coluna=rand()%numVertices;
            }
            adjacencyMatrix[linha][coluna].addCollor(rand()%numCores);
            adjacencyMatrix[coluna][linha] = adjacencyMatrix[linha][coluna];
        }
    }

    for( int i(1) ; i<numVertices ; i++ ){
        for( int j(0) ; j<i ; j++ ){
            if( adjacencyMatrix[i][j].isActive ){
                collorQuantity = 1+(rand() % (1+ numCores/10));
                for( int k(0) ; k>collorQuantity ; k++ ){
                    adjacencyMatrix[i][j].addCollor(rand()%numCores);
                }
            }
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
            std::cout << "Você esta inserindo um grafo com um número diferente de vertices\nO numero deve ser igual ao do grafo criado.";
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
Limpa toda as arestas do grafo (remove as cores associadas a aresta, e as cores que foram pintadas na aresta)
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
Despinta todo o grafo
**/
void Graph::unpaint(){
    for( int i(0) ; i<numVertices ; i++ ){
        for( int j(0) ; j<numVertices ; j++ ){
            adjacencyMatrix[i][j].unpaintAllCollors();
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
void Graph::exactSolve(){
    // 1 - REALIZANDO PREPARATIVOS
    clock_t begin = clock();
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
                if( adjacencyMatrix[i][j].isActive && adjacencyMatrix[i][j].hasCollor(k) && ++collorCount[k]>collorCount[corMaisPresente] ){
                    corMaisPresente = k;
                }
            }
        }
        verticesInTree[i] = false;
    }
    verticesInTree[0] = false;

    // 2 - ALGORITMO QUE REALIZA O BRANCH-AND-BOUND
    exactSolveAGRM(collorCount, verticesInTree, corMaisPresente, 0, 0); //O(C!*N^2)

    // 3 - MONTANDO ARVORE SOLUÇÃO
    unpaint();
    mountSolution();

    delete [] collorCount;
    delete [] verticesInTree;
    solutionTime = (float)(clock()-begin)/CLOCKS_PER_SEC;
    return;
}

/**
Algoritmo que realiza o Branch-And-Bound nas cores buscando otimizar o número de cores utilizadas.
**/
void Graph::exactSolveAGRM( int * collorCount, bool * verticesInTree, int corMaisPresente, int numUsedCollors, int numUsedEdges ){
    //NESTE PASSO SERÁ INSERIDA UMA COR NA SOLUÇÃO
    //level++;
    if(  numUsedCollors+projection(numUsedEdges, collorCount)>=bestNumCollors ){    return; } //VERIFICA SE É POSSIVEL ENCONTRAR UMA SOLUÇÃO MELHOR POR ESSE CAMINHO
    numUsedCollors++;

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

        /**
        deslocLevel();
        std::cout << "Try to solve with: ";
        for( int i(0) ; i<numCores ; i++ ){
            if( nextCount[i]<0 ){
                std::cout << i << ", ";
            }
        }
        **/
        colorir(nextVerticesInTree, nextCount, numUsedEdges, localNumArestas);

        //SE O NUMERO DE ARESTAS ATUAL É N-1 TEMOS UMA ARVORE GERADORA (SOLUÇÃO)
        if( numUsedEdges+localNumArestas==numVertices-1 ){
            if( numUsedCollors<bestNumCollors ){ //SE A SOLUÇÃO É MELHOR DO QUE A QUE JÁ TEMOS
                for( int i(0) ; i<numCores  ; i++ ){ //GUARDA A SOLUÇÃO. O(C)
                    bestSolution[i] = (nextCount[i]<0);
                }
                bestNumCollors = numUsedCollors; //ATUALIZA O NUMERO DE CORES DA SOLUÇÃO
                delete [] nextCount;
                delete [] nextVerticesInTree;
                return;
            }
            delete [] nextCount;
            delete [] nextVerticesInTree;
            return;
        }

        int nextCorMaisPresente = 0;
        for( int i(0) ; i<numCores ; i++ ){ //RECALCULA A COR MAIS PRESENTE NAS ARESTAS DO GRAFO
            if( nextCount[i]>nextCount[nextCorMaisPresente] ){
                nextCorMaisPresente = i;
            }
        }

        if( numUsedCollors<bestNumCollors-1 ){ //SE AINDA É POSSIVEL CONSEGUIR UMA SOLUÇÃO MELHOR, REALIZA O PROXIMO PASSO DO BRANCH-AND-BOUND
            exactSolveAGRM( nextCount, nextVerticesInTree, nextCorMaisPresente, numUsedCollors, numUsedEdges+localNumArestas );
        }
        delete [] nextCount;
        delete [] nextVerticesInTree;
    }
    //level--;
    return;
}

/**
Realiza uma projeção, retornando o minimo de cores que devem ser adicionadas para completar a arvore
**/
int Graph::projection( int numUsedEdges, int * collorCount ){
    //mostUsedCollor = indice da cor mais utilizada nas arestas restantes da arvore da projeção
    //numCollorProjeciton = projeção do numero de cores que serão inseridas
    int mostUsedCollor= 0, numCollorProjection = 0;
    int * collorCountProjection = new int[numCores];
    for( int i(0) ; i<numCores ; i++ ){ // INICIA O VETOR DE PROJEÇÃO DA CONTAGEM DAS CORES
        collorCountProjection[i] = collorCount[i];
    }
    while( numUsedEdges<numVertices-1 ){ // ADICIONA A COR QUE POSSUI O MAIOR NUMERO DE ARESTAS ATÉ COMPLETAR N-1 ARESTAS NA ARVORE
        for( int i(0) ; i<numCores ; i++ ){ //RECONHECE A COR QUE MAIS APARECE NA ARVORE
            mostUsedCollor = (collorCountProjection[i]>=0 && collorCountProjection[i]>collorCountProjection[mostUsedCollor])? i : mostUsedCollor;
        }
        numUsedEdges += collorCountProjection[mostUsedCollor];      //ATUALIZA PROJEÇÃO DA QUANTIDADE DE ARESTAS
        collorCountProjection[mostUsedCollor] = -1;                 //ATUALIZA PROJEÇÃO DA CONTAGEM DAS CORES
        numCollorProjection++;                                      //ATUALIZA PROJEÇÃO DO NUMERO DE CORES NA SOLUÇÃO
    }
    return numCollorProjection;
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
    if( arestas!=numVertices-1 ){   std::cout << "printToFileSolutionReport >> DEU MERDA, vertices " << numVertices << ", arestas " << arestas << std::endl; }
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

    file.open("testBanchSolutions1.txt", std::ios::app);
    file << numVertices << "\t" << numCores << "\t" << numArestas << "\t" << bestNumCollors << "\t" << solutionTime;

    file.close();
}

/**
Imprime no arquivo o tempo e quantidade de cores encontradas pelo algoritmo exato
**/
void Graph::completFileSolutionReportLine( std::string fileName ){
    std::ofstream file;
    file.open("testBanchSolutions1.txt", std::ios::app);
    file << "\t" << bestNumCollors << "\t" << solutionTime << "\n";
    file.close();
}

/**
Vetifica se:
1 - O grafo inicial era conexo
2 - A solução encontrada é conexa
3 - todas as arestas no grafo podiam ser pintadas da cor que foram pintadas
4 - a solução é livre de ciclos
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
                        std::cout << "como ta pintado se nem ta no grafo?\n";
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
    return true;
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

/**
Monta uma arvore com o conjunto de cores solução
**/
void Graph::mountSolution(){
    bool * verticesInTree = new bool[numVertices];
    for( int i(0) ; i<numVertices ; i++ ){
        verticesInTree[i] = false;
    }

    //CONJUNTO DE CORES UTILIZADAS NA SOLUÇÃO
    int * collorCount = new int [numCores];
    for( int i(0) ; i<numCores ; i++ ){
        collorCount[i] = bestSolution[i]? -1:numVertices*numVertices;
    }

    if( !persistCollorsInTree() ){
        std::cout << "Graph::mountSolution() >> vem k\n";
    }

    delete [] collorCount;
    delete [] verticesInTree;
}

/**
Resolve o problema atravez de uma busca local iterativa
**/
void Graph::heurisctSolve( float alfa, float beta ){
    clock_t begin = clock();
    bool * backupSolution = new bool [numCores];

    initialSolution(alfa);

    int backupNumCollors = bestNumCollors;
    for( int i(0) ; i<numCores ; i++ ){
        backupSolution[i] = bestSolution[i];
    }

    localSearch();

    //int coresUsadas;
    int iterations = 0;

    do{
        while( localSearch() ){}
        if( bestNumCollors<backupNumCollors ){
            iterations = 0;
            for( int i(0) ; i<numCores ; i++ ){
                backupSolution[i] = bestSolution[i];
            }
            backupNumCollors = bestNumCollors;
        }
        iterations++;
        disturb(alfa);
    }while( iterations < 5 );

    for( int i(0) ; i<numCores ; i++ ){
        bestSolution[i] = backupSolution[i];
    }
    bestNumCollors = backupNumCollors;
    unpaint();
    mountSolution();

    delete [] backupSolution;
    solutionTime = (float)(clock()-begin)/CLOCKS_PER_SEC;
    return;
}

/*
1 SOLUÇÃO INICIAL                   // void initialSolution(float a);
2 BUSCA LOCAL                       // bool localSearch();
3 ENQUANTO(CRITERIO)                // ???
    3.1 PERTURBAÇÃO                 // ???  (pode mudar totalmente?)
    3.2 BUSCA LOCAL                 // bool localSearch();
    3.3 CRITERIO DE ACEITAÇÃO       // ???

----------------------------------------------------------------
1 SOLUÇÃO INICIAL:
    Dada uma probabilidade alfa, escolhe uma cor, com a probabilidade alfa
    de escolher de forma guloza, e 1-alfa de escolhar aleatoriamente.

    A cada cor escolhida, insere a cor no conjunto de cores da solução, e
    tenta gerar uma arvore com estas cores, se não for possivel, adiciona
    outra cor e continua o processo.

    A escolha guloza seleciona a cor que possui maior grau nas arestas
    do grafo não utilizadas na arvore.

----------------------------------------------------------------
2 BUSCA LOCAL:
    Vizinhança:
    Para cada cor não utilizada na solução, colore com ela
    e depois tenta colorir com as cores pertencentes a solução
    atual.

    Formal: Sendo S o conjunto de cores na solução, K o conjunto de
    todas as cores, e L o conjunto das cores que não estão na solução,
    onde S U L = K, a vizinhança V é dada por:
    V = U {Vi C (forAll c in L, {c} U S)}

----------------------------------------------------------------
3.1 PERTURBAÇÃO:
    Escolhe cores com probabilidade alfa de escolher cores de fora da
    solução atual, caso não haja  cores fora da solução atual, é sempre
    escolhidaum  cor da solução. A cada cor escolhida, ela é adicionada
    ao proximo conjunto de cores, e tentamos colorir a arvore com o
    proximo conjunto de cores, se for possivel temos uma nova solução, se
    não, continuamos adicionando cores.

----------------------------------------------------------------
3.3 CRITERIO DE ACEITAÇÃO: Enquanto diminuir o numero de cores.

*/

/**
Retorna uma cor de forma aleatoria/gulosa de acordo com o parametro alfa (1 para 100% guloso, 0 para 100% aleatorio)
**/
int Graph::generateCollor( int alfa, int corMaisPresente ){
    srand(time(NULL));
    if( (float)rand()/(float)RAND_MAX >alfa ){
        return rand()%numCores;
    }
    return corMaisPresente;
}

/**
Gera uma solução inicial para o algoritmo
**/
void Graph::initialSolution(float alfa){
    //std::cout << "InitialSolution\n";
    unpaint();
    if( bestSolution==NULL ){
        bestSolution = new bool[numCores];
    }
    int * collorCount = new int [numCores];
    for( int i(0) ; i<numCores ; i++ ){     //INICIALIZANDO VETOR DE CONTAGEM DAS CORES PRESENTES NAS ARESTAS DA ARVORE, E O VETOR QUE CONTEM AS CORES DA SOLUÇÃO
        collorCount[i] = numVertices*numVertices;                 // O(C)
        bestSolution[i] = false;
    }

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
    int arestas = 0;
    bestNumCollors = 0;
    int c;
    while( arestas<numCores-1 ){
        c = generateCollor(alfa, corMaisPresente);
        while( collorCount[c]<1 ){  c=(c+1)%numCores;    }
        collorCount[c] = -1;
        bestNumCollors++;

        colorir(verticesInTree, collorCount, 0, arestas);

        bestSolution[c] = true;
        for( int k(0) ; k<numCores ; k++ ){         //O(C)
            if( collorCount[k]>collorCount[corMaisPresente] ){
                corMaisPresente = k;
            }
        }
    }
    unpaint();
    mountSolution();

    delete [] collorCount;
    delete [] verticesInTree;
}

/**
Pinta as arestas com as cores de collorCount, ligando vertices de fora da arvore a vertices da arvore
**/
bool Graph::colorir( bool * verticesInTree, int * collorCount, const int numUsedArrestas, int & localNumUsedArestas ){
    bool returnValue = false;
    bool coloriu;
    int cor;
    do{
        coloriu = false;
        for( int i(1) ; i<numVertices ; i++ ){  //PARA A COR ATUAL (cor)
            for( int j(0) ; j<i ; j++ ){    //INSERI AS ARESTAS QUE TEM ALGUMA COR DA SOLUÇÃO NA ARVORE, SEM FORMAR CICLOS
                if( adjacencyMatrix[i][j].isActive && (verticesInTree[i]!=verticesInTree[j] || numUsedArrestas+localNumUsedArestas==0) ){   // ADICIONA UM VERTICE QUE NÃO ESTAVA NA ARVORE (NAO FORMA CICLO)
                    cor = adjacencyMatrix[i][j].hasCollors(collorCount);
                    if( cor!=-1 ){
                        //std::cout << "Colorindo (" << i << ", " << j << ") com cor " << cor << std::endl;
                        returnValue = true;
                        verticesInTree[i] = true;
                        verticesInTree[j] = true;
                        localNumUsedArestas++;
                        collorCount[cor]--;
                        coloriu = true;
                    }
                }
            }
        }
    }while( coloriu );
    return returnValue;
}

/**
Persiste as cores pintadas nas arestas do grafo
**/
bool Graph::persistCollorsInTree(){
    bool * verticesInTree = new bool[numVertices];
    for( int i(0) ; i<numVertices ; i++ ){  verticesInTree[i] = false;  }

    int numPaintedEdges = 0;

    int * collorCount = new int[numCores];
    for( int i(0) ; i<numVertices ; i++ ){  collorCount[i] = bestSolution[i]? -1:1;  }

    bool coloriu;
    int cor;
    do{
        coloriu = false;
        for( int i(1) ; i<numVertices ; i++ ){  //PARA A COR ATUAL (cor)
            for( int j(0) ; j<i ; j++ ){    //INSERI AS ARESTAS QUE TEM ALGUMA COR DA SOLUÇÃO NA ARVORE, SEM FORMAR CICLOS
                if( adjacencyMatrix[i][j].isActive && (verticesInTree[i]!=verticesInTree[j] || numPaintedEdges==0) ){   // ADICIONA UM VERTICE QUE NÃO ESTAVA NA ARVORE (NAO FORMA CICLO)
                    cor = adjacencyMatrix[i][j].hasCollors(collorCount);
                    if( cor!=-1 ){
                        adjacencyMatrix[i][j].paint(cor);
                        numPaintedEdges++;
                        verticesInTree[i] = true;
                        verticesInTree[j] = true;
                        coloriu = true;
                    }
                }
            }
        }
    }while( coloriu );
    delete [] collorCount;
    delete [] verticesInTree;
    return numPaintedEdges==numVertices-1;
}

/**
Busca uma solução melhor dentre as soluções proximas a atual
**/
bool Graph::localSearch(){
    //std::cout << "LocalSearch\n";
    unpaint();
    std::vector<int> unusedCollors;
    int * collorCount = new int [numCores];
    bool * verticesInTree = new bool [numVertices];
    for( int i(0) ; i<numCores ; i++ ){
        if( !bestSolution[i] ){
            unusedCollors.push_back(i);
        }
    }

    srand(time(NULL));
    std::vector<int>::iterator it;
    int cor;
    int localNumArestas;
    int numUsedCollors;
    while( !unusedCollors.empty() ){
        localNumArestas = 0;
        numUsedCollors = 0;
        for( int i(0) ; i<numCores ; i++ ){
            collorCount[i] = numVertices*numVertices;
        }
        for( int i(0) ; i<numVertices ; i++ ){
            verticesInTree[i] = false;
        }
        cor = unusedCollors[rand()%unusedCollors.size()]; //cor a ser pintada
        for( it = unusedCollors.begin() ; *it!=cor ; it++ ){}
        unusedCollors.erase(it);
        while( localNumArestas<numVertices-1 && numUsedCollors<bestNumCollors ){
            collorCount[cor] = -1;
            colorir(verticesInTree, collorCount, 0, localNumArestas);
            numUsedCollors++;
            cor = rand()%numCores;
            while( numUsedCollors<bestNumCollors && (!bestSolution[cor] || collorCount[cor]<0) ){
                cor = (cor+1)%numCores;
            }
        }
        if( localNumArestas==numVertices-1 && numUsedCollors<bestNumCollors ){
            //std::cout << "Busca local encontrou solucao melhor!\n";
            for( int i(0) ; i<numCores ; i++ ){
                bestSolution[i] = collorCount[i]<0;
            }
            bestNumCollors = numUsedCollors;
            unpaint();
            mountSolution();
            delete [] collorCount;
            delete [] verticesInTree;
            return true;
        }
        //else{ std::cout << "Nao achou solucao\n"; }
    }
    delete [] collorCount;
    delete [] verticesInTree;
    return false;
}

/**
Perturba a solução atual, com probabilidade alfa de escolher cores de fora da solução
**/
void Graph::disturb( float alfa){
    //std::cout << "Disturb\n";
    unpaint();
    int * collorCount = new int [numCores];
    for( int i(0) ; i<numCores ; i++ ){     //INICIALIZANDO VETOR DE CONTAGEM DAS CORES PRESENTES NAS ARESTAS DA ARVORE, E O VETOR QUE CONTEM AS CORES DA SOLUÇÃO
        collorCount[i] = numVertices*numVertices;                 // O(C)
        bestSolution[i] = false;
    }

    std::vector<int> collorsInTree;
    std::vector<int> collorsOutTree;
    for( int i(0) ; i<numCores ; i++ ){ //INICIALIZA O CONJUNTO DE VERTICES NÃO PRESENTES NA ARVORE GERADORA
        if(bestSolution[i]){
            collorsInTree.push_back(i);
        }
        else{
            collorsOutTree.push_back(i);
        }
        bestSolution[i] = false;
    }

    bool * verticesInTree = new bool [numVertices];
    for( int i(1) ; i<numVertices ; i++ ){          // INICIALIZANDO VETOR QUE MARCA A PRESENÇA DOS VERTICES NA ARVORE E
        verticesInTree[i] = false;
    }
    verticesInTree[0] = false;
    int arestas = 0;
    bestNumCollors = 0;
    int c;
    srand(time(NULL));
    std::vector<int>::iterator it;
    while( arestas<numCores-1 ){
        if( (((float)rand()/(float)RAND_MAX) <= alfa && !collorsOutTree.empty()) || collorsInTree.empty() ){ // ESCOLHE UMA COR QUE NÃO ESTAVA NA SOLUÇÃO
            c = collorsOutTree[rand()%collorsOutTree.size()];
            for( it = collorsOutTree.begin() ; *it!=c ; it++ ){}
            collorsOutTree.erase(it);
        }
        else{           // ESCOLHE UMA COR QUE ESTAVA NA SOLUÇÃO
            c = collorsInTree[rand()%collorsInTree.size()];
            for( it = collorsInTree.begin() ; *it!=c ; it++ ){}
            collorsInTree.erase(it);
        }
        while( collorCount[c]<1 ){  c=(c+1)%numCores;    }
        collorCount[c] = -1;
        bestNumCollors++;
        colorir(verticesInTree, collorCount, 0, arestas);
        bestSolution[c] = true;
    }
    if(arestas!=numVertices-1){ std::cout << "Graph::disturb >> " << numVertices << " vertices e " << arestas << " arestas\n";  }
    unpaint();
    mountSolution();
    if( !validateSolution() ){
        std::cout << "Graph::disturb >> Solução invalida\n";
    }

    delete [] collorCount;
    delete [] verticesInTree;
}

/**
Usado para debug, encontra um caso de erro por tentativa e erro
**/
void Graph::findCase(){
    int exactNumCores = 0;
    int heuristicNumCores = 0;
    while( exactNumCores<=heuristicNumCores ){
        generateInstance(numVertices, 0.5);
        exactSolve();
        exactNumCores = bestNumCollors;
        heurisctSolve(0.8, 0.5);
        heuristicNumCores = bestNumCollors;
    }
    printInstanceToFile("findErrorCase");
    return;
}

