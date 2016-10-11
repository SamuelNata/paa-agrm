#include "Edge.h"
#include <iostream>


Edge::Edge(){
    isActive = false;
    possibleCollors = NULL;
    activeCollors = NULL;
}

Edge::~Edge(){
    if( possibleCollors!=NULL )
        delete [] possibleCollors;
    if( activeCollors!=NULL )
        delete [] activeCollors;
}

/**
Inicializa as cores associadas a aresta e pintadas na aresta
**/
void Edge::setNumCollors(int num){
    maxCollorsNum = num;
    if(possibleCollors!=NULL){
        delete [] possibleCollors;
    }
    if(activeCollors!=NULL){
        delete [] activeCollors;
    }
    possibleCollors = new bool [num];
    activeCollors = new bool [num];
    for( int index(0) ; index<num ; index++ ){
        possibleCollors[index] = false;
        activeCollors[index] = false;
    }
    isActive = false;
}

/**
Pinta uma cor na aresta
**/
void Edge::paint(int collor){
    if(activeCollors!=NULL && collor<maxCollorsNum && isActive && possibleCollors!=NULL && possibleCollors[collor] ){
        activeCollors[collor] = true;
    }
    else{
        std::cout << "Edge::paint > Vai da merda, vai da merda (8)...\n";
        if(possibleCollors==NULL){  std::cout << "possibleCollors==NULL\n"; }
        if(activeCollors==NULL){  std::cout << "activeCollors==NULL\n"; }
        if(!possibleCollors[collor]){ std::cout << "possibleCollors[collor]";}
        if(collor>=maxCollorsNum){  std::cout << "collor>=maxCollorsNum\n"; }
        if(collor<0){  std::cout << "collor<0\n"; }
        if(!isActive){  std::cout << "!active\n"; }
    }
}

/**
Associa uma cor a aresta
**/
void Edge::addCollor(int collor){
    if(possibleCollors!=NULL && collor<maxCollorsNum){
        possibleCollors[collor] = true;
        this->isActive = true;
    }
    else{
        std::cout << "ERRO EM Edge::addCollor\n";
        if(possibleCollors==NULL){  std::cout << "possibleCollors==NULL\n"; }
        if(collor>=maxCollorsNum){  std::cout << "collor>=maxCollorsNum\n"; }
        if(collor<0){  std::cout << "collor<0\n"; }
    }
}

/**
Limpa todas as cores pintadas e associadas na aresta
**/
void Edge::clearCollor(int collor){
    if(activeCollors!=NULL && collor<maxCollorsNum){
        activeCollors[collor] = false;
        possibleCollors[collor] = false;
        isActive = false;
        for( int t(0) ; t<maxCollorsNum ; t++ ){
            if( activeCollors[t] ){
                isActive = true;
                return;
            }
        }
    }
    else{
        std::cout << "ERRO EM Edge::clearCollor\n";
        std::cout << "\nactiveCollors, collor, maxCollorsNum: " << activeCollors << ", " << collor << ", " << maxCollorsNum << std::endl;
    }
}

/**
retorna true se uma das cores do vetor collor está associada a aresta, false caso contrario
**/
bool Edge::hasCollors(int * collor){
    if( isActive && activeCollors!=NULL && possibleCollors!=NULL ){
        for( int i(0) ; i<maxCollorsNum ; i++ ){
            if( collor[i]<0 && possibleCollors[i] ){
                return true;
            }
        }
    }
    return false;
}

/**
retorna true se a cor collor está associada a aresta, false caso contrario
**/
bool Edge::hasCollor(int collor){
    if( isActive && activeCollors!=NULL && possibleCollors!=NULL && collor<maxCollorsNum && possibleCollors[collor] ){
        return true;
    }
    return false;
}

/**
retorna true se a aresta está pintada com a cor collor, false caso contrario
**/
bool Edge::isPaintedCollor(int collor){
    if( activeCollors!=NULL && possibleCollors!=NULL && collor<maxCollorsNum ){
        return activeCollors[collor];
    }
    return false;
}

/**
retorna true se  a aresta está pintada com alguma cor, false caso contrario
**/
bool Edge::isPainted(){
    if( isActive && activeCollors!=NULL && possibleCollors!=NULL ){
        for( int i(0) ; i<maxCollorsNum ; i++ ){
            if( activeCollors[i] ){
                return true;
            }
        }
    }
    return false;
}

/**
atribui todas as caracteristicas de uma aresta a outra aresta
**/
Edge& Edge::operator=( const Edge& rhs ){
    this->maxCollorsNum = rhs.maxCollorsNum;

    if(activeCollors==NULL){
        activeCollors = new bool[maxCollorsNum];
    }

    if(possibleCollors==NULL){
        possibleCollors = new bool[maxCollorsNum];
    }

    for( int i(0) ; i<maxCollorsNum ; i++ ){
        this->activeCollors[i] = rhs.activeCollors[i];
        this->possibleCollors[i] = rhs.possibleCollors[i];

    }

    this->isActive = rhs.isActive;
    return *this;
}
