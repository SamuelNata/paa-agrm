#include <iostream>
#include <sstream>
#include <string>
#include "Graph.h"
#include <ctime>

float alfa = 0.8;
float beta = 0.5;

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
    Graph g100(100);
    Graph g150(150);
    Graph g200(200);

    int numberOfCases = 20;
    std::string fileName = "casosDeTesteGrupo1\\AGRM";
    for( int i(0) ; i<numberOfCases ; i++ ){
        std::cout << " >>>> Gerando " << i << " de " << numberOfCases << "...\n";

        std::cout << "g(100, 100, 0.2)\n";
        g100.generateInstance(100, 0.2);
        g100.printInstanceToFile( fileName+itoa(i) );

        std::cout << "g(100, 100, 0.5)\n";
        g100.generateInstance(100, 0.5);
        g100.printInstanceToFile( fileName+itoa(i+20) );

        std::cout << "g(100, 100, 0.8)\n";
        g100.generateInstance(100, 0.8);
        g100.printInstanceToFile( fileName+itoa(i+40) );



        std::cout << "g(150, 150, 0.2)\n";
        g150.generateInstance(150, 0.2);
        g150.printInstanceToFile( fileName+itoa(i+60) );

        std::cout << "g(150, 150, 0.5)\n";
        g150.generateInstance(150, 0.5);
        g150.printInstanceToFile( fileName+itoa(i+80) );

        std::cout << "g(150, 150, 0.8)\n";
        g150.generateInstance(150, 0.8);
        g150.printInstanceToFile( fileName+itoa(i+100) );



        std::cout << "g(200, 200, 0.2)\n";
        g200.generateInstance(200, 0.2);
        g200.printInstanceToFile( fileName+itoa(i+120) );

        std::cout << "g(200, 200, 0.5)\n";
        g200.generateInstance(200, 0.5);
        g200.printInstanceToFile( fileName+itoa(i+140) );

        std::cout << "g(200, 200, 0.8)\n";
        g200.generateInstance(200, 0.8);
        g200.printInstanceToFile( fileName+itoa(i+160) );
    }
}

void solveInstances(){
    Graph g100(100);
    Graph g150(150);
    Graph g200(200);
    std::string filename = "casosDeTesteGrupo1\\AGRM";
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
    //generateTestBanch(); // DESCOMENTE CASO QUEIRA GERAR CASOS DE TESTE;
    //solveInstances(); //SOLUCIONA TODO O BRANCO DE TESTES QUE FOI GERADO E ESTÁ EM ARQUIVO
    Graph g10(7);
    g10.findCase();
    g10.readInstance("findErrorCase.txt");
    std::cout << "1\n";
    g10.exactSolve();
    std::cout << "2\n";
    g10.printToFileSolutionReport("findErrorWithExact");
    std::cout << "3\n";
    g10.heurisctSolve(0.8, 0.5);
    std::cout << "4\n";
    g10.printToFileSolutionReport("findErrorWithHeuristic");

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
    /**
    Graph g20(20);
    g20.generateInstance(20, 0.2);
    g20.printInstanceToFile("g20Test.txt");


    std::cout << "Solucao Inicial:\n";
    g20.initialSolution(0.5);
    if( g20.validateSolution() ){
                std::cout << "OK\n";
    }
    else{
        std::cout << "Houve erros.\n";
    }
    std::cout << "Solucao Inicial:\n";
    g20.printSolutionReport("G20 - Solucao Inicial");

    std::cout << "Busca Local:\n";
    g20.localSearch();
    if( g20.validateSolution() ){
                std::cout << "OK\n";
    }
    else{
        std::cout << "Houve erros.\n";
    }
    g20.printSolutionReport("G20 com Busca local");
    g20.printToFileSolutionReport("g20TestSolution.txt");
    **/

    return 0;
}
