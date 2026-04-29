#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    if (count == capacity)
    {
        resize(2*capacity);
    }
    data[rear] = req;
    rear = (rear+1)%capacity;
    count++;

    (void)req; // suppress unused warning until implemented
    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    if (count == 0)
    {
        return false;
    }
    
    (void)outReq; // suppress unused warning until implemented
    outReq = data[front];
    front = (front+1)%capacity;
    count--;
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if (count == 0)
    {
        return false;
    }
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    if (count == 0)
    {
        return false;
    }
    bool flag = false;
    Request* newarr = new Request[capacity];
    int counter = 0;
    for (int i = 0; i < count; i++)
    {
        if (data[(front+i)%capacity].getId()==id)
        {
            flag = true;
        }
        else {
            newarr[counter] = data[(front+i)%capacity];
            counter++;
        }
    }
    if (!flag)
    {
        return false;
    }
    delete[] data;
    data = newarr;
    count = counter;
    front = 0;
    rear = count%capacity;
    
    
    
    (void)id; // suppress unused warning until implemented
    return true;
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    Request* newarr = new Request[newCapacity];
    for (int i = 0; i < count; i++)
    {
        newarr[i] = data[(front+i)%capacity];
    }
    capacity = newCapacity;
    delete[] data;
    data = newarr;
    rear = count;
    front = 0;

    return true;
}
