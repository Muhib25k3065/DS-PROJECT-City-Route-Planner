#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <cstddef>

template <class T>
class LinkedQueue {
private:
    struct Node {
        T data;
        Node* next;

        Node(T value) {
            data = value;
            next = NULL;
        }
    };

    Node* frontNode;
    Node* rearNode;
    int count;

public:
    LinkedQueue() {
        frontNode = NULL;
        rearNode = NULL;
        count = 0;
    }

    ~LinkedQueue() {
        clear();
    }

    void enqueue(T value) {
        Node* node = new Node(value);
        if (rearNode == NULL) {
            frontNode = node;
            rearNode = node;
        } else {
            rearNode->next = node;
            rearNode = node;
        }
        count++;
    }

    bool dequeue(T& out) {
        if (frontNode == NULL) {
            return false;
        }
        Node* oldFront = frontNode;
        out = oldFront->data;
        frontNode = oldFront->next;
        if (frontNode == NULL) {
            rearNode = NULL;
        }
        delete oldFront;
        count--;
        return true;
    }

    bool peek(T& out) const {
        if (frontNode == NULL) {
            return false;
        }
        out = frontNode->data;
        return true;
    }

    bool isEmpty() const {
        return frontNode == NULL;
    }

    int size() const {
        return count;
    }

    void clear() {
        while (frontNode != NULL) {
            Node* nextNode = frontNode->next;
            delete frontNode;
            frontNode = nextNode;
        }
        rearNode = NULL;
        count = 0;
    }
};

#endif
