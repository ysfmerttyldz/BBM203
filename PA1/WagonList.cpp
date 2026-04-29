#include "WagonList.h"

#include <iostream>

using namespace std;

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.
    Wagon* w = front;
    while (w!= nullptr)
    {
        Wagon* ww = w->getNext();
        delete w;
        w = ww;
    }
    front = nullptr,rear = nullptr;
    totalWeight = 0;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.
    front = other.front;
    rear = other.rear;
    totalWeight = other.totalWeight;
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.
    clear();
    front = other.front;
    rear = other.rear;
    totalWeight = other.totalWeight;
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
    return *this;
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.
    Wagon* w = front;
    while (w)
    {
        if(w->getID()==id){
            return w;
        }
        w = w->getNext();
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight
    w->setNext(nullptr); w->setPrev(rear);
    if (front==nullptr)
    {
        front = w;
        rear = w;
    }
    else {       
        rear->setNext(w);
        rear = w;
    } 
    totalWeight+= w->getWeight();
}

bool WagonList::isEmpty() const
{
    // TODO: Return true if the list has no wagons.
    if (front == nullptr)
    {
        return true;
    }
    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.
    if (front == nullptr)
    {
        front = wagon,rear = wagon;
        totalWeight+= wagon->getWeight();
        return;
    }
    Wagon* w = front;
    while (w && w->getWeight()>wagon->getWeight())
    {
        w = w->getNext();
    }
    if (w == nullptr)
    {
        wagon->setPrev(rear);
        wagon->setNext(nullptr);
        rear->setNext(wagon);
        rear = wagon;
    }
    else if(w==front){
        front->setPrev(wagon);
        wagon->setNext(front);
        wagon->setPrev(nullptr);
        front = wagon;
    }
    else {
        Wagon* ww = w->getPrev();
        wagon->setPrev(ww);
        wagon->setNext(w);
        ww->setNext(wagon);
        w->setPrev(wagon);   
    }
    totalWeight += wagon->getWeight();  
}

void WagonList::clearWagon(WagonList &other){
    other.totalWeight = 0;
    other.front = nullptr;
    other.rear = nullptr;
}

void WagonList::appendList(WagonList &&other)
{
    if (other.isEmpty())
    {
        return;
    } 

    if (isEmpty()) 
    {
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
        other.front = nullptr; other.rear  = nullptr;
        other.totalWeight = 0;
        return;
    }
    if (front->getWeight()>other.front->getWeight())
    {
        rear->setNext(other.front);
        other.front->setPrev(rear);
        rear = other.rear;
    }
    else {
        other.rear->setNext(front);
        front->setPrev(other.rear);
        front = other.front;

    }
    totalWeight += other.totalWeight;
    other.front = nullptr; other.rear  = nullptr;
    other.totalWeight = 0;
}

int WagonList::getTotalWeight() const
{
    return totalWeight;
}

Wagon* WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.
    if (isEmpty())
    {
        return nullptr;
    }
    Wagon* w = front;
    while (w && w->getID()!= id)
    {
        w = w->getNext();
    }
    if (w == nullptr)
    {
        return nullptr;
    }
    if (w == rear)
    {
        rear = rear->getPrev();
        if (rear == nullptr)
        {
            front = nullptr;
        }
        else {
            rear->setNext(nullptr);
        }
    }
    else if (w == front)
    {
        front = front->getNext();
        if (front==nullptr)
        {
            rear = nullptr;
        }
        else {
            front->setPrev(nullptr);
        }
    }
    else {
        w->getPrev()->setNext(w->getNext());
        w->getNext()->setPrev(w->getPrev());
    }
    cout << "Wagon " << w->id << " detached from Wagon List. " << endl;
    w->setNext(nullptr); w->setPrev(nullptr);
    totalWeight-= w->getWeight();
    return w;
}

WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.
    Wagon* w =front;
    while (w && w->getID()!= id)
    {
        w = w->getNext();
    }

    if (w == nullptr)
    {
        return newList;
    }
    newList.front = w;
    newList.rear = rear;
    Wagon* prev = w->getPrev(),*next = w->getNext();
    if (prev != nullptr)
    {
        rear = prev;
        prev->setNext(nullptr);
    }
    else {
        front = nullptr,rear = nullptr;
    }
    w->setPrev(nullptr);
    int cnt = 0;
    Wagon* cur = w;
    while (cur)
    {
        cnt += cur->getWeight();
        cur = cur->getNext();
    }
    newList.totalWeight = cnt;
    totalWeight-= cnt; 
    return newList; // moved, not copied
}

// Print is already implemented
void WagonList::print() const
{
    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
