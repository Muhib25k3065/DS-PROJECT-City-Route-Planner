#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
using namespace std;

class MinHeap {
private:
    vector<int> heap;
    vector<int> position;
    vector<double> key;
    int count;

    bool isSmaller(int vertexA, int vertexB) const;
    void swapPositions(int i, int j);
    void moveUp(int i);
    void moveDown(int i);

public:
    MinHeap(int numVertices);

    bool isEmpty() const;
    int size() const;
    bool contains(int vertex) const;
    bool insert(int vertex, double priority);
    bool decreaseKey(int vertex, double newPriority);
    bool extractMin(int& vertex, double& priority);
    bool peekMin(int& vertex, double& priority) const;
    double getKey(int vertex) const;
    void clear();
};

#endif
