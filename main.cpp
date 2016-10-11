#include <iostream>
#include <sstream>
#include <string>
#include "Graph.h"
#include <ctime>



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
    Graph g20(20);
    Graph g30(30);
    Graph g40(40);
    Graph g50(50);

    std::string fileName = "casosDeTesteGrupo1\\AGRM";
    for( int i(0) ; i<5 ; i++ ){
        std::cout << " >>>> Gerando " << i << " de 5...\n";

        std::cout << "g(20, 20, 0.2)\n";
        g20.generateInstance(20, 0.2);
        g20.printInstanceToFile( fileName+itoa(i) );

        std::cout << "g(20, 20, 0.5)\n";
        g20.generateInstance(20, 0.5);
        g20.printInstanceToFile( fileName+itoa(i+5) );

        std::cout << "g(20, 20, 0.8)\n";
        g20.generateInstance(20, 0.8);
        g20.printInstanceToFile( fileName+itoa(i+10) );



        std::cout << "g(30, 30, 0.2)\n";
        g30.generateInstance(30, 0.2);
        g30.printInstanceToFile( fileName+itoa(i+15) );

        std::cout << "g(30, 30, 0.5)\n";
        g30.generateInstance(30, 0.5);
        g30.printInstanceToFile( fileName+itoa(i+20) );

        std::cout << "g(30, 30, 0.8)\n";
        g30.generateInstance(30, 0.8);
        g30.printInstanceToFile( fileName+itoa(i+25) );



        std::cout << "g(40, 40, 0.2)\n";
        g40.generateInstance(40, 0.2);
        g40.printInstanceToFile( fileName+itoa(i+30) );

        std::cout << "g(40, 40, 0.5)\n";
        g40.generateInstance(40, 0.5);
        g40.printInstanceToFile( fileName+itoa(i+35) );

        std::cout << "g(40, 40, 0.8)\n";
        g40.generateInstance(40, 0.8);
        g40.printInstanceToFile( fileName+itoa(i+40) );



        std::cout << "g(50, 50, 0.2)\n";
        g50.generateInstance(50, 0.2);
        g50.printInstanceToFile( fileName+itoa(i+45) );

        std::cout << "g(50, 50, 0.5)\n";
        g50.generateInstance(50, 0.5);
        g50.printInstanceToFile( fileName+itoa(i+50) );

        std::cout << "g(50, 50, 0.8)\n";
        g50.generateInstance(50, 0.8);
        g50.printInstanceToFile( fileName+itoa(i+55) );
    }
}

void solveInstances(){
    Graph g20(20);
    Graph g30(30);
    Graph g40(40);
    Graph g50(50);
    std::string filename = "casosDeTesteGrupo1\\AGRM";
    for( int i(0) ; i<60 ; i++ ){ //RESOLVENDO GRUPO 1
        if( i<15 ){
            g20.readInstance( filename+itoa(i)+".txt");
            g20.solve();
            if( g20.validateSolution() ){
                std::cout << "OK\n";
            }
            g20.printSolutionReport(filename+itoa(i));
            g20.printToFileSolutionReport(filename+itoa(i));
        }
        if( i>=15 && i<30 ){
            g30.readInstance( filename+itoa(i)+".txt");
            g30.solve();
            if( g30.validateSolution() ){
                std::cout << "OK\n";
            }
            g30.printSolutionReport(filename+itoa(i));
            g30.printToFileSolutionReport(filename+itoa(i));

        }
        if( i>=30 && i<45 ){
            g40.readInstance( filename+itoa(i)+".txt");
            g40.solve();
            if( g40.validateSolution() ){
                std::cout << "OK\n";
            }
            g40.printSolutionReport(filename+itoa(i));
            g40.printToFileSolutionReport(filename+itoa(i));
        }
        if( i>=45 && i<60 ){
            g50.readInstance( filename+itoa(i)+".txt");
            g50.solve();
            if( g50.validateSolution() ){
                std::cout << "OK\n";
            }
            g50.printSolutionReport(filename+itoa(i));
            g50.printToFileSolutionReport(filename+itoa(i));
        }
    }
    return;
}

int main()
{
    generateTestBanch(); // DESCOMENTE CASO QUEIRA GERAR CASOS DE TESTE;
    solveInstances(); //SOLUCIONA TODO O BRANCO DE TESTES QUE FOI GERADO E ESTÁ EM ARQUIVO
    return 0;
}
