#ifndef SUITCASE_H
#define SUITCASE_H

#include <string> 

class Suitcase {
private:
    std::string* arr;
    int top;
    int size;

public:
    Suitcase(int size = 5);
    ~Suitcase();

    bool isEmpty() const;
    bool isFull() const;
    void push(std::string value); 
    std::string pop();            
    void print() const;

    void reverse();
    void resize();   
    int capacity() const;
};

#endif

