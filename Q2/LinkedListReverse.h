// <DO NOT CHANGE>
#ifndef LINKED_LIST_REVERSE_H
#define LINKED_LIST_REVERSE_H

#include <iostream>

class SingleReverse {
public:
    class Node {
    public:
        int data;
        Node* next;
        explicit Node(int v) : data(v), next(nullptr) {}
    };

private:
    Node* head; // start of the singly linked list

public:
    SingleReverse();
    ~SingleReverse();

    // Insert node at the end of the list
    void insertEnd(int value);

    // Reverse the entire list in-place
    void reverse();

    // Print list values on one line, separated by spaces
    void printList() const;

    // Free all nodes; safe on empty list
    void clear();
};

#endif
// </DO NOT CHANGE>
