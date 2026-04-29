#include "HashTableQP.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    // 1. Determine input file
    string filename = "input_qp.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    ifstream in(filename);
    if (!in) {
        cerr << "Could not open input file: " << filename << endl;
        return 1;
    }

    cout << "Reading from file: " << filename << endl;

    HashTableQP table;

    // 2. Read N
    int n;
    if (!(in >> n)) {
        cerr << "Invalid input format (N)." << endl;
        return 1;
    }

    // 3. Read N keys and insert them
    cout << "Inserting keys with quadratic probing..." << endl;
    cout << "Keys: ";
    for (int i = 0; i < n; ++i) {
        int key;
        if (!(in >> key)) {
            cerr << "Not enough keys in input file." << endl;
            return 1;
        }
        cout << key << " ";
        table.insert(key);
    }
    cout << endl;

    // 4. Print table
    cout << "Final table (index: value):" << endl;
    table.printTable();

    // 5. Read query key Q
    int query;
    if (!(in >> query)) {
        cerr << "No query key provided in input file." << endl;
        return 0;
    }

    // 6. Search and print result
    bool found = table.search(query);
    int idx = table.getIndex(query);

    cout << endl;
    cout << "Search " << query << " -> " << (found ? "FOUND" : "NOT FOUND");
    if (found) {
        cout << " at index " << idx;
    }
    cout << endl;

    return 0;
}
