#define LINUX
//#define WINDOWS

#include <iostream>
#include <sstream>
#include <string>
#include "Graph.h"
#include <ctime>


float alfa = 0.8;
float beta = 0.5;
int numVerticesAndCollors1 = 10;
int numVerticesAndCollors2 = 15;
int numVerticesAndCollors3 = 20;


std::string itoa( int number ){ //numberToString
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void generateTestBanch(){
    // Graph( int numVertices)
    // Graph::generateInstance(int numCores, float dencidade)
    // Graph::printInstanceToFile(std::string fileName)

    //GERAR BANCO DE TESTES;

    Graph g1(numVerticesAndCollors1);
    Graph g2(numVerticesAndCollors2);
    Graph g3(numVerticesAndCollors3);

    int numberOfCases = 20;

    #ifdef WINDOWS
        std::string fileName = "casosDeTesteGrupo1\\AGRM";
    #endif

    #ifdef LINUX
        std::string fileName = "casosDeTesteGrupo1/AGRM";
    #endif

    for( int i(0) ; i<numberOfCases ; i++ ){
        std::cout << " >>>> Gerando " << i << " de " << numberOfCases << "...\n";

        std::cout << "g(" << numVerticesAndCollors1 << ", " << numVerticesAndCollors1 << ", 0.2)\n";
        g1.generateInstance(numVerticesAndCollors1, 0.2);
        g1.printInstanceToFile( fileName+itoa(i) );

        std::cout << "g(" << numVerticesAndCollors1 << ", " << numVerticesAndCollors1 << ", 0.5)\n";
        g1.generateInstance(numVerticesAndCollors1, 0.5);
        g1.printInstanceToFile( fileName+itoa(i+20) );

        std::cout << "g(" << numVerticesAndCollors1 << ", " << numVerticesAndCollors1 << ", 0.8)\n";
        g1.generateInstance(numVerticesAndCollors1, 0.8);
        g1.printInstanceToFile( fileName+itoa(i+40) );



        std::cout << "g(" << numVerticesAndCollors2 << ", " << numVerticesAndCollors2 << ", 0.2)\n";
        g2.generateInstance(numVerticesAndCollors2, 0.2);
        g2.printInstanceToFile( fileName+itoa(i+60) );

        std::cout << "g(" << numVerticesAndCollors2 << ", " << numVerticesAndCollors2 << ", 0.5)\n";
        g2.generateInstance(numVerticesAndCollors2, 0.5);
        g2.printInstanceToFile( fileName+itoa(i+80) );

        std::cout << "g(" << numVerticesAndCollors2 << ", " << numVerticesAndCollors2 << ", 0.8)\n";
        g2.generateInstance(numVerticesAndCollors2, 0.8);
        g2.printInstanceToFile( fileName+itoa(i+100) );



        std::cout << "g(" << numVerticesAndCollors3 << ", " << numVerticesAndCollors3 << ", 0.2)\n";
        g3.generateInstance(numVerticesAndCollors3, 0.2);
        g3.printInstanceToFile( fileName+itoa(i+120) );

        std::cout << "g(" << numVerticesAndCollors3 << ", " << numVerticesAndCollors3 << ", 0.5)\n";
        g3.generateInstance(numVerticesAndCollors3, 0.5);
        g3.printInstanceToFile( fileName+itoa(i+140) );

        std::cout << "g(" << numVerticesAndCollors3 << ", " << numVerticesAndCollors3 << ", 0.8)\n";
        g3.generateInstance(numVerticesAndCollors3, 0.8);
        g3.printInstanceToFile( fileName+itoa(i+160) );
    }
}

void solveInstances(){
    Graph g100(numVerticesAndCollors1);
    Graph g150(numVerticesAndCollors2);
    Graph g200(numVerticesAndCollors3);

    #ifdef WINDOWS
        std::string filename = "casosDeTesteGrupo1\\AGRM";
    #endif

    #ifdef LINUX
        std::string filename = "casosDeTesteGrupo1/AGRM";
    #endif

    for( int i(0) ; i<180 ; i++ ){ //RESOLVENDO GRUPO 1
        if( i<60 ){
            g100.readInstance( filename+itoa(i)+".txt");
            g100.exactSolve();
            if( g100.validateSolution() ){
                std::cout << "OK\n";
            }
            g100.printSolutionReport(filename+itoa(i));
            g100.printToFileSolutionReport(filename+itoa(i));
            g100.heurisctSolve(alfa, beta);
            g100.completFileSolutionReportLine(filename+itoa(i));
        }
        if( i>=60 && i<120 ){
            g150.readInstance( filename+itoa(i)+".txt");
            g150.exactSolve();
            if( g150.validateSolution() ){
                std::cout << "OK\n";
            }
            g150.printSolutionReport(filename+itoa(i));
            g150.printToFileSolutionReport(filename+itoa(i));
            g150.heurisctSolve(alfa, beta);
            g150.completFileSolutionReportLine(filename+itoa(i));
        }
        if( i>=120 && i<180 ){
            g200.readInstance( filename+itoa(i)+".txt");
            g200.exactSolve();
            if( g200.validateSolution() ){
                std::cout << "OK\n";
            }
            g200.printSolutionReport(filename+itoa(i));
            g200.printToFileSolutionReport(filename+itoa(i));
            g200.heurisctSolve(alfa, beta);
            g200.completFileSolutionReportLine(filename+itoa(i));
        }
    }
    return;
}

int main()
{
    std::cout << "Gerando casos\n";
    generateTestBanch(); // DESCOMENTE CASO QUEIRA GERAR CASOS DE TESTE;
    std::cout << "Resolvendo casos\n";
    solveInstances(); //SOLUCIONA TODO O BRANCO DE TESTES QUE FOI GERADO E ESTÁ EM ARQUIVO

    /**
    Graph g10(30);
    g10.findCase();
    g10.readInstance("findErrorCase.txt");
    g10.exactSolve();
    g10.printToFileSolutionReport("findErrorWithExact");
    g10.heurisctSolve(0.8, 0.5);
    g10.printToFileSolutionReport("findErrorWithHeuristic");
    **/

    /*Graph g20(200);
    g20.generateInstance(200, 0.2);
    g20.printInstanceToFile("g20Instance");
    std::cout << "EXATO\n";
    g20.exactSolve();
    g20.printSolutionReport("SOLUCAO EXATA");
    if( !g20.validateSolution() ){
        std::cout << "Houve erros.\n";
    }
    std::cout << "\n\n\n";

    std::cout << "HEURISTICO\n";
    g20.heurisctSolve(0.8, 0.5);
    g20.printSolutionReport("SOLUCAO HEURISTICA");
    if( !g20.validateSolution() ){
        std::cout << "Houve erros.\n";
    }
    */

    return 0;
}
