#include "WarehouseBST.h"

// Constructor
WarehouseBST::WarehouseBST() : root(nullptr) {}

// Destructor
WarehouseBST::~WarehouseBST() {
    destroy();
}

// Public insert wrapper
void WarehouseBST::insert(int key) {
    root = insert(root, key);
}

// TODO: Implement BST insert
Node* WarehouseBST::insert(Node* node, int key) {
    // TODO:
    // 1. If node is nullptr, create new node.
    if(node == nullptr){
    	return new Node(key);
    }
    
    if(key < node->key)
    {
    	node->left = insert(node->left,key);
    }
    else if(key > node->key)
    {
    	node->right = insert(node->right,key);
    }
    return node;
    
    // 2. If key < node->key, go left.
    // 3. If key > node->key, go right.
    // 4. Ignore duplicates.

}

// Public wrapper
int WarehouseBST::countParentNodes() const {
    return countParentNodes(root);
}

// TODO: Count nodes that have at least one child
int WarehouseBST::countParentNodes(Node* node) const {
    // TODO:
 
 if(!node){
 return 0;}
 
    if(node->right || node->left)
    {
    	return countParentNodes(node->right)+countParentNodes(node->left)+1;

    }
    else {
    	return countParentNodes(node->right)+countParentNodes(node->right);
    }
    
  

    
}

// Public destroy wrapper
void WarehouseBST::destroy() {
    destroy(root);
    root = nullptr;
}

// TODO: Postorder deletion of all nodes
void WarehouseBST::destroy(Node* node) {
    // TODO:
    // 1. If node is nullptr, return.
    // 2. Recursively destroy left child.
    // 3. Recursively destroy right child.
    // 4. Delete current node.
	if(node == nullptr)
	{
	return;
	}
    if (node->left)
    {
    	destroy(node->left);
    }
    if(node->right){
    	destroy(node->right);
    }
 
    if(!root->right && !root->left)
    {
    	delete node;
    }
    
    
    
}
