#include "HashTableQP.h"
#include <iostream>
using namespace std;

// Constructor: initialize all cells as empty (-1)
HashTableQP::HashTableQP() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = -1;
    }
}

int HashTableQP::hash(int key) const {
    return key % TABLE_SIZE;
}

// =========================
//        TODO PART
// =========================

// Insert key using quadratic probing.
// index(i) = (h + i*i) % TABLE_SIZE
void HashTableQP::insert(int key) {
    // 1) Compute h = hash(key).
    int h = hash(key);
    for(int i = 0; i<TABLE_SIZE-1;i++)
    {
    	int id = (h+i*i)%TABLE_SIZE;
    	if(table[id]==-1)
    	{
    		table[id]=key;
    		return;
    	}
    	if(table[id]==key)
    	{
    		return;
    	}
    }
    // 2) For i from 0 to TABLE_SIZE-1:
    //       index = (h + i*i) % TABLE_SIZE;
    //       - if table[index] == -1, put key there and stop
    //       - if table[index] == key, you may stop (ignore duplicates)
    // 3) If you cannot find an empty slot, the table is considered full.
}

// Search key using quadratic probing.
// Return true if key is found; false otherwise.
bool HashTableQP::search(int key) const {
	int h = hash(key);
	for(int i = 0; i<TABLE_SIZE-1;i++)
	{
		int id = (h+i*i)%TABLE_SIZE;
		if(table[id]==key){return true;}
		if(table[id]==-1){return false;}
	}
	return false;
    // 1) Compute h = hash(key).
    // 2) Same loop as insert: i = 0..TABLE_SIZE-1
    //       index = (h + i*i) % TABLE_SIZE;
    //       - if table[index] == key -> return true
    //       - if table[index] == -1  -> key is not in table, return false
    // 3) If you finish the loop without finding key, return false.
    return false; // TEMP: replace with your implementation
}

// Return the index of key if it is in the table, -1 otherwise.
int HashTableQP::getIndex(int key) const {
	int h = hash(key);
	for(int i = 0; i<TABLE_SIZE-1;i++)
	{
		int id = (h+i*i)%TABLE_SIZE;
		if(table[id]==-1)
		{
			return -1;
		}
		if(table[id]==key)
		{
			return id;
		}
		
	}
	return -1;
    // Use exactly the same probing sequence as in search().
    // If you find the key, return the index.
    // If you see an empty cell (-1) or finish probing without a match,
    // return -1.
    return -1; // TEMP: replace with your implementation
}

// Print the entire table: "index: value"
void HashTableQP::printTable() const {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cout << i << ": " << table[i] << endl;
    }
}
