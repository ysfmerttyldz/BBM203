#ifndef HASHTABLEQP_H
#define HASHTABLEQP_H

class HashTableQP {
private:
    static const int TABLE_SIZE = 11;
    int table[TABLE_SIZE];

    // Hash function: key mod TABLE_SIZE
    int hash(int key) const;

public:
    HashTableQP();

    // Public interface
    void insert(int key);           // TODO: implement in .cpp
    bool search(int key) const;     // TODO: implement in .cpp
    int getIndex(int key) const;    // TODO: implement in .cpp
    void printTable() const;        // already implemented in .cpp
};

#endif // HASHTABLEQP_H
