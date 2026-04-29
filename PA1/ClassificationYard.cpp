#include "ClassificationYard.h"
#include <iostream>

using namespace std;

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

Train* ClassificationYard::assembleTrain(Destination dest, const std::string& trainName)
{
    WagonList* wagonlist = getBlocksFor(dest);
    WagonList ans;

    bool visited[NUM_CARGOTYPES_INT];
    for (int i = 0; i < NUM_CARGOTYPES_INT; ++i) 
    {
        visited[i] = false;
    }

   
    while (true)
    {
        pair<int,int> b = {-1,-1};
        for (int i = 0; i < NUM_CARGOTYPES_INT; ++i)
        {
            if (i == 2 || visited[i] || wagonlist[i].isEmpty()){
                continue;
            }
            //cout <<"BURASI": <<i;
            //cout <<"visited: "<< visited[i];

            Wagon* f = wagonlist[i].getFront();
            int w;
            if (f==nullptr)
            {
                w = -1;
            }
            else {
                w = f->getWeight();
            }

            if (w > b.first) {
                b = {w,i};
            }
        }

        if (b.second == -1) 
        {
            break;
        }         

        ans.appendList(std::move(wagonlist[b.second]));
        visited[b.second] = true;
    }

    if (!wagonlist[2].isEmpty())
    {
        Wagon* x = wagonlist[2].getFront();
        if (x != nullptr)
        {
            Wagon* www = wagonlist[2].detachById(x->getID());
            if (www!= nullptr)
            {
                ans.addWagonToRear(www);
            }
            
        }
    }

    if (ans.isEmpty()) 
    {
        return nullptr;
    }

    Train* t = new Train(trainName, dest);
    t->appendWagonList(ans);
    return t;
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                return false;
            }
            
        }
        
    }
    
    return true;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++)
        {
            blockTrains[i][j].clear();
        }
        
    }
    
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}