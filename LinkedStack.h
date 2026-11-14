#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include <cstddef>

template <class T>
class LinkedStack {
private:
    struct Node {
        T data;
        Node* next;

        Node(T value) {
            data = value;
            next = NULL;
        }
    };

    Node* topNode;
    int count;

public:
    LinkedStack() {
        topNode = NULL;
        count = 0;
    }

    ~LinkedStack() {
        clear();
    }

    void push(T value) {
        Node* node = new Node(value);
        node->next = topNode;
        topNode = node;
        count++;
    }

    bool pop(T& out) {
        if (topNode == NULL) {
            return false;
        }
        Node* oldTop = topNode;
        out = oldTop->data;
        topNode = oldTop->next;
        delete oldTop;
        count--;
        return true;
    }

    bool peek(T& out) const {
        if (topNode == NULL) {
            return false;
        }
        out = topNode->data;
        return true;
    }

    bool isEmpty() const {
        return topNode == NULL;
    }

    int size() const {
        return count;
    }

    void clear() {
        while (topNode != NULL) {
            Node* nextNode = topNode->next;
            delete topNode;
            topNode = nextNode;
        }
        count = 0;
    }
};

#endif
