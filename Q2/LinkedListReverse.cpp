#include <iostream>
#include "LinkedListReverse.h"
using namespace std;

SingleReverse::SingleReverse() : head(nullptr) {}

SingleReverse::~SingleReverse() {
    clear();
}

// Insert node at the end of the list
void SingleReverse::insertEnd(int value)
{
    Node* newNode = new Node(value);	
	
    // TODO: Implement tail insertion for singly linked list
    
    if(!newNode){
    	return;
    }
    newNode->next = nullptr;
    if(!head){
    	head = newNode;
    	return;
    }
    
    Node* cur = head;
    while(cur->next)
    {
    	cur = cur->next;
    }
    cur->next = newNode;

    
    
	//Tekledik vesselam 31 ekim 2025
}

// Reverse the entire list in-place
void SingleReverse::reverse()
{
    // TODO: Implement in-place reversal with three pointers
    
    if(!head)
    {
    	return;
    }
    Node* c = head;
    Node* prev;

	while(c)
	{
		Node* cur = c->next;
		c->next =prev;
		prev = c;
		c = cur;
	}
	head = prev;
	
}

void SingleReverse::printList() const
{
    for (Node* cur = head; cur; cur = cur->next) {
        cout << cur->data;
        if (cur->next) cout << " ";
    }
    cout << endl;
}

void SingleReverse::clear()
{
    while (head) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}
 
