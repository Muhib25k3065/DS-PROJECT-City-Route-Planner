#include "MinHeap.h"
using namespace std;

MinHeap::MinHeap(int numVertices) {
    if (numVertices < 0) {
        numVertices = 0;
    }
    for (int i = 0; i < numVertices; i++) {
        heap.push_back(-1);
        position.push_back(-1);
        key.push_back(0.0);
    }
    count = 0;
}

bool MinHeap::isEmpty() const {
    return count == 0;
}

int MinHeap::size() const {
    return count;
}

bool MinHeap::contains(int vertex) const {
    int total = position.size();
    if (vertex < 0 || vertex >= total) {
        return false;
    }
    return position[vertex] != -1;
}

bool MinHeap::isSmaller(int a, int b) const {
    if (key[a] < key[b]) {
        return true;
    }
    if (key[a] > key[b]) {
        return false;
    }
    return a < b;
}

void MinHeap::swapPositions(int i, int j) {
    int temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
    position[heap[i]] = i;
    position[heap[j]] = j;
}

void MinHeap::moveUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (isSmaller(heap[i], heap[parent])) {
            swapPositions(i, parent);
            i = parent;
        } else {
            break;
        }
    }
}

void MinHeap::moveDown(int i) {
    while (true) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int smallest = i;

        if (leftChild < count && isSmaller(heap[leftChild], heap[smallest])) {
            smallest = leftChild;
        }
        if (rightChild < count && isSmaller(heap[rightChild], heap[smallest])) {
            smallest = rightChild;
        }
        if (smallest == i) {
            break;
        }
        swapPositions(i, smallest);
        i = smallest;
    }
}

bool MinHeap::insert(int vertex, double priority) {
    int total = position.size();
    if (vertex < 0 || vertex >= total) {
        return false;
    }
    if (position[vertex] != -1) {
        return false;
    }
    key[vertex] = priority;
    heap[count] = vertex;
    position[vertex] = count;
    count++;
    moveUp(count - 1);
    return true;
}

bool MinHeap::decreaseKey(int vertex, double newPriority) {
    if (!contains(vertex)) {
        return false;
    }
    if (newPriority > key[vertex]) {
        return false;
    }
    key[vertex] = newPriority;
    moveUp(position[vertex]);
    return true;
}

bool MinHeap::extractMin(int& vertex, double& priority) {
    if (count == 0) {
        return false;
    }
    vertex = heap[0];
    priority = key[vertex];
    swapPositions(0, count - 1);
    position[vertex] = -1;
    count--;
    if (count > 0) {
        moveDown(0);
    }
    return true;
}

bool MinHeap::peekMin(int& vertex, double& priority) const {
    if (count == 0) {
        return false;
    }
    vertex = heap[0];
    priority = key[vertex];
    return true;
}

double MinHeap::getKey(int vertex) const {
    int total = key.size();
    if (vertex < 0 || vertex >= total) {
        return 0.0;
    }
    return key[vertex];
}

void MinHeap::clear() {
    int total = position.size();
    for (int i = 0; i < total; i++) {
        position[i] = -1;
    }
    count = 0;
}
