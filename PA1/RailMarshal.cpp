#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

RailMarshal::RailMarshal()
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        departureYard[i] = TrainTrack(static_cast<Destination>(i));
    }
    TrainTrack::autoDispatch = false;
    
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        auto& t = departureYard[i];
        while (!t.isEmpty())
        {
            Train* tt = t.departTrain();
            delete tt;
        }
        
    }
    
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.
    
    istringstream st(line);
    string type;
    st>> type;

    if (type=="ADD_WAGON")
    {
        int id,weight,maxCouplerLoad;
        string cargoType,destination;
        if(!(st >> id>> cargoType >>destination>>weight >>maxCouplerLoad)){
            cout << "Error: Invalid ADD_WAGON parameters.\n";
        }
        else {
            Wagon* w = new Wagon(id,parseCargo(cargoType),parseDestination(destination),weight,maxCouplerLoad);
            classificationYard.insertWagon(w);
            cout << "Wagon " << *w << " added to yard." << std::endl;
        }
    }
    else if(type == "REMOVE_WAGON"){
        int id;
        if(!(st >> id)){
            cout << "Error: Invalid REMOVE_WAGON parameters.\n";
        }
        else {
            bool flag = false;
            for (int i = 0; i < NUM_DESTINATIONS_INT && !flag; i++)
            {
                for (int j = 0; j < NUM_CARGOTYPES_INT; j++)
                {
                    WagonList& wagonl = classificationYard.getBlockTrain(i,j);
                    if (Wagon* wagon = wagonl.detachById(id))
                    {
                        cout << "Wagon " << id << " removed." << endl;
                        flag = true;
                        delete wagon;    
                        break;
                        
                    }
                    
                }
                
            }
            if (!flag)
            {
                cout << "Error: Wagon " << id << " not found." << endl;
            }
            
        }
    }
    else if(type == "PRINT_YARD"){
        cout << "--- classification Yard ---\n";
        classificationYard.print();
    }
    else if (type == "ASSEMBLE_TRAIN") {
        std::string dest;
        if (!(st >> dest)) {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }

        Destination d = parseDestination(dest);
        TrainTrack& track = getDepartureYard(d);
        std::string trainName = track.generateTrainName();
        //cout << d << ' '<< track.getFirst << ' '<<  trainname;

        Train* train = classificationYard.assembleTrain(d, trainName);
        if (!train) {
            std::cout << "No wagons to assemble for " << dest << std::endl;
            return;
        }

        int cnt = 1;
        while (Train* newTrain = train->verifyCouplersAndSplit(cnt)) {
            cnt++;
            track.addTrain(newTrain);
            std::cout << "Train " << newTrain->getName()<< " assembled after split with " << newTrain->getWagons()<< " wagons." << std::endl;
        }

        track.addTrain(train);
        std::cout << "Train " << train->getName()<< " assembled with " << train->getWagons()<< " wagons." << std::endl;
    }
    else if (type =="DISPATCH_TRAIN")
    {
        string dest;
        if (!(st >> dest))
        {
            cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
        Destination d = parseDestination(dest);
        dispatchFromTrack(d);
        
    }
    else if(type=="PRINT_TRACK"){
        string dest;
        if (!(st >> dest))
        {
            cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        Destination d = parseDestination(dest);
        getDepartureYard(d).printTrack();
    }
    else if(type == "AUTO_DISPATCH"){
        string val;
        if (!(st>>val))
        {
            cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }
        
        if (val=="ON")
        {
            TrainTrack::autoDispatch = true;
            cout <<"Auto dispatch enabled"<<endl;
        }
        else {
            TrainTrack::autoDispatch = false;
            cout <<"Auto dispatch disabled"<<endl;
        }
        
        
    }
    else if(type == "CLEAR"){
        classificationYard.clear();
        for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
        {
            TrainTrack& t =departureYard[i];
            while (!t.isEmpty())
            {
                Train* tt = t.departTrain();
                delete tt;
            }
            
        }
        cout << "System cleared." << endl;
    }
    else {
        cout << "Error: Unknown command '" << type << "'" << endl;
    }
    
    
    

    // if ADD_WAGON
    // Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;

    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;

    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;

    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";

    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    TrainTrack& tt = getDepartureYard(track);

    if (tt.isEmpty()) {
        std::cout << "Error: No trains to dispatch from track "
                  << destinationToString(track) << ".\n";
        return;
    }

    Train* tr = tt.departTrain();  // "Train ... departed ..." yazar
    std::cout << "Dispatching " << tr->getName()
              << " (" << tr->getTotalWeight() << " tons)." << std::endl;
    delete tr;
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
