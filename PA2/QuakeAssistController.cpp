#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

using namespace std;

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    istringstream st(line);
    string type; st >> type;
    if (type == "INIT_TEAMS")
    {
        int numteams; st >> numteams;
        initializeTeams(numteams);
    }
    else if(type == "SET_TEAM_CAPACITY"){
        int id,c; st >> id >> c;
        handleSetTeamCapacity(id,c);
    }
    else if(type == "ADD_SUPPLY"){
        string a,b,c; int d,e; st >> a >> b >> c >> d >> e;
        handleAddSupply(a,b,c,d,e);
    }
    else if(type == "ADD_RESCUE"){
        string a,b,d; int c,e; st >> a >> b >> c >> d >> e;
        handleAddRescue(a,b,c,d,e);
    }
    else if(type == "REMOVE_REQUEST"){
        string a; st >> a;
        handleRemoveRequest(a);
    }
    else if(type == "PRINT_TEAM"){
        int a; st >> a;
        printTeam(a);
    }
    else if(type == "PRINT_QUEUES"){
        printQueues();
    }
    else if(type =="HANDLE_EMERGENCY"){
        int a,b; st >> a >> b;
        handleHandleEmergency(a,b);
    }
    else if(type =="DISPATCH_TEAM"){
        int a; st >> a;
        handleDispatchTeam(a);
    }
    else if(type =="SET_TEAM_CAPACITY"){
        int a,b; st >> a >> b;
        int index = findTeamIndexById(a);
        teams[index].setMaxLoadCapacity(b);
    }
    else if(type =="CLEAR"){
        clear();
        supplyQueue.clear();
        rescueQueue.clear();
        for (int i = 0; i < teamCount; i++)
        {
            teams[i].clearMission();
        }
        
    }
    else {
        cout <<"Error: Unknown command '"<<type<<"'."<<endl;
    }
    return true;
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    cout << "Initialized "<< numTeams<< " teams."<<endl;
    teams = new Team[numTeams];
    teamCount = numTeams;
    for (int i = 0; i < numTeams; i++)
    {
        teams[i].setId(i);
    }
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i].getId()==teamId)
        {
            return i;
        }
        
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    int index = findTeamIndexById(teamId);
    teams[index].setMaxLoadCapacity(capacity);
    cout << "Team "<<teamId<< " capacity set to " <<capacity<<"."<<endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    cout << "Request " << id << " added to SUPPLY queue." << endl;
    supplyQueue.enqueue(Request(id,cityStr,supplyTypeStr,amount,emergencyLevel));
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    cout << "Request " << id << " added to RESCUE queue." << endl;
    rescueQueue.enqueue(Request(id,cityStr,numPeople,riskStr,emergencyLevel));
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue. 
    if (supplyQueue.removeById(id))
    {
        cout <<"Request "<<id << " removed from queues."<<endl;
        return true;
    }
    else if(rescueQueue.removeById(id)){
        cout <<"Request "<<id << " removed from queues."<<endl;
        return true;
    }
    else {
        cout <<"Error: Request "<<id<< " not found."<<endl;
        return false;
    }
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.

    int index = findTeamIndexById(teamId);
    Team& team = teams[index];
    int cnt1 = 0,cnt2 = 0;
    for (int i = 0; i < k; i++)
    {
        Request supplyreq,rescuereq;
        bool flag1 = supplyQueue.peek(supplyreq);
        bool flag2 = rescueQueue.peek(rescuereq);
        if (!flag1 && !flag2)
        {
            break;
        }
        bool flag3 = false;

        if (!flag1)
        {
            flag3 = false;
        }
        else if(!flag2){
            flag3 = true;
        }
        else {
            int f = supplyreq.computeEmergencyScore();
            int s = rescuereq.computeEmergencyScore();
            if (f>s)
            {
                flag3 = true;
            }
            else if(s>f){
                flag3 = false;
            }
            else {
                flag3 = false;
            }
            
        }
        Request req;
        if (flag3)
        {
            req =supplyreq;
        }
        else {
            req = rescuereq;
        }

        bool flag4 = team.tryAssignRequest(req);
        if (!flag4)
        {
            cout << "Overload on Team " << teamId << ": rolling back mission." <<endl;
            team.rollbackMission(supplyQueue,rescueQueue);
            return false;
        }
        Request req2;
        if (flag3)
        {
            cnt1++;
            supplyQueue.dequeue(req2);
        }
        else {
            cnt2++;
            rescueQueue.dequeue(req2);
        }

        
        
        
    }

    int t = cnt1+cnt2;
    if (t)
    {
        cout << "Team " << teamId << " assigned "<< t << " requests ("<< cnt1 << " SUPPLY, "<< cnt2 << " RESCUE), total workload "<< team.getCurrentWorkload() << "." <<endl;   
    }
    

    (void)teamId;
    (void)k;
    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    cout << "SUPPLY QUEUE:"<<endl;
    for (int i = 0; i < supplyQueue.getCount(); i++)
    {
        int index = (supplyQueue.getFrontIndex()+i)%supplyQueue.getCapacity();
        cout << supplyQueue.getData()[index].getId() << " "<< supplyQueue.getData()[index].getCity() << " "<< supplyQueue.getData()[index].getSupplyType() << " " << supplyQueue.getData()[index].getAmount()<< " "<< supplyQueue.getData()[index].getEmergencyLevel()<<endl; 
    }
    cout << "RESCUE QUEUE:"<<endl;
    for (int i = 0; i < rescueQueue.getCount(); i++)
    {
        int index = (rescueQueue.getFrontIndex()+i)%rescueQueue.getCapacity();
        cout << rescueQueue.getData()[index].getId() << " "<< rescueQueue.getData()[index].getCity() << " "<< rescueQueue.getData()[index].getNumPeople() << " " << rescueQueue.getData()[index].getRisk()<< " "<< rescueQueue.getData()[index].getEmergencyLevel()<<endl; 
    
    }
    
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    cout <<"TEAM "<<teamId<<" STACK:"<<endl;
    int index = findTeamIndexById(teamId);
    const Team& team = teams[index];
    const MissionStack& s = team.getMissionStack();
    if (s.isEmpty())
    {
        return;
    }
    int top = s.getTopIndex();
    for (int i = top; i >=0; i--)
    {
        Request& req = s.getData()[i];
        if (req.getType()=="SUPPLY")
        {
            cout<<req.getId()<<" "<<req.getCity()<<" "<<req.getSupplyType()<<" "<<req.getAmount()<<" "<<req.getEmergencyLevel()<<endl;
        }
        else {
            cout <<req.getId()<<" "<<req.getCity()<<" " <<req.getNumPeople()<<" "<<req.getRisk()<<" "<<req.getEmergencyLevel()<<endl;
        }
        
    }
    
    
}

void QuakeAssistController::clear() {
    //Clear data.
    supplyQueue.clear();
    rescueQueue.clear();
    for (int i = 0; i < teamCount; i++)
    {
        teams[i].clearMission();
    }
    cout <<"System cleared."<<endl;
    
}
