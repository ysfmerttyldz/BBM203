#ifndef WAREHOUSE_BST_H
#define WAREHOUSE_BST_H

#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class WarehouseBST {
private:
    Node* root;

    Node* insert(Node* node, int key);
    int countParentNodes(Node* node) const;

    void destroy(Node* node);

public:
    WarehouseBST();
    ~WarehouseBST();

    void insert(int key);
    int countParentNodes() const;
    void destroy();
};

#endif
