#ifndef EDGE_H
#define EDGE_H


class Edge
{
private:
    int maxCollorsNum;
    bool * possibleCollors;
    bool * activeCollors;

public:
    bool isActive;
    Edge();
    ~Edge();
    void setNumCollors(int num);
    void addCollor(int collor);
    void paint(int collor);
    void clearCollor(int collor);
    bool hasCollors(int * collor);
    bool hasCollor(int collor);
    bool isPaintedCollor(int collor);
    bool isPainted();
    Edge& operator=( const Edge& rhs );
};

#endif // EDGE_H
