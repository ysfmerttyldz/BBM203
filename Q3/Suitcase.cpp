#include "Suitcase.h"
#include <iostream>
using namespace std;

Suitcase::Suitcase(int s) {
    size = s;
    top = -1;
    arr = new std::string[size];
}

Suitcase::~Suitcase() {
	//Implement destructor
	delete[] arr;
	top = -1;
	size = 0;
    
}

bool Suitcase::isEmpty() const {
    return top == -1;
}

bool Suitcase::isFull() const {
    return top == size - 1;
}

void Suitcase::push(std::string value) {
	//TO DO: Implement push function
    if (isFull()) {
        cout << "Error: Suitcase is too small! Find bigger suitcase! (Stack full. Call resize() first.)\n";
        return;
    }
    top++;
    arr[top] = value;
    
}

std::string Suitcase::pop() {  
	//TO DO: Implement pop function
    if (isEmpty()) {
        cout << "Stack underflow\n";
        return ""; 
    }
    string temp = arr[top];
    top--;
    return temp;
    
}

void Suitcase::resize() {
	//TO DO: Double the capacity of stack

    string* newarr = new string[2*size];
    for(int i = 0; i<size;i++)
    {
    	newarr[i] = arr[i];
    }
  	 delete[] arr;
    arr = newarr;
    
    size = 2*size;
    cout << "[INFO] Stack resized to " << size << endl;
}

void Suitcase::reverse() {
    int start = 0, end = top;
    while (start < end) {
        std::swap(arr[start], arr[end]);  
        start++;
        end--;
    }
}

void Suitcase::print() const {
    cout << "Stack (top to bottom): ";
    for (int i = top; i >= 0; i--)
        cout << arr[i] << " ";
    cout << endl;
}

int Suitcase::capacity() const {
    return size;
}

