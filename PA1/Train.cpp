#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

using namespace std;

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight= wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    //TODO: Do the cleaning as necesssary
    wagons.clear();
    totalWeight = 0;
    nextLocomotive = nullptr;
    name = "";
    destination = parseDestination("");
}


// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{

    // TODO: Verify whether any wagon’s coupler is overloaded.
    // You must traverse from the rear (backmost wagon) toward the front.
    //
    // Splitting rule:
    // Split the train AFTER the overloaded wagon (the overloaded one stays).
    // Use WagonList::splitAtById to detach the overloaded segment into a new WagonList.
    //
    // If no overload occurs, return nullptr (no split needed).
    //
    // If a valid split occurs:
    // new Train should be named "<oldName>_split_<splitCounter>".

    // print message
    // std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
    // std::cout << newTrain->wagons << std::endl;

    int s = 0;
    Wagon* cur = wagons.getRear();
    while (cur)
    {
        int curmax = cur->getMaxCouplerLoad();
        if (s > curmax)
        {
            Wagon* nextwagon = cur->getNext();
            if (nextwagon == nullptr)
            {
                return nullptr;
            }
            int splitlist = nextwagon->getID();
          //  cout << splitlist<< std::endl;



            WagonList t = wagons.splitAtById(splitlist);
           // cout << "tw"<<t.getTotalWeight();

            string n = name+"_split_"+to_string(splitCounter);
           // cout << "ŞU AN BURDASIN";
            //cout << n << std::endl;
            Train* nt = new Train(n,destination);
            //cout << nt->getName() << ' '<< nt->getTotalWeight() << ' '<< nt->getDestination();


        
            nt->appendWagonList(t);
            totalWeight = wagons.getTotalWeight();
            cout << "Train " << name << " split due to coupler overload before Wagon " << splitlist << std::endl;
            cout << nt->wagons << endl;
            return nt;
        }
        s+= cur->getWeight();
        cur = cur->getPrev();
    }

    return nullptr;
}