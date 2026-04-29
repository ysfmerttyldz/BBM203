
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>
using namespace std;

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known
    std::string t[8];
    int c=tokenize(line,t,8);
    if(!c)return;
    if(t[0]=="ADD_ARTIFACT")handleAddArtifact(t,c);
    else if(t[0]=="REMOVE_ARTIFACT")handleRemoveArtifact(t,c);
    else if(t[0]=="HIRE_RESEARCHER")handleHireResearcher(t,c);
    else if(t[0]=="FIRE_RESEARCHER")handleFireResearcher(t,c);
    else if(t[0]=="REQUEST")handleRequest(t,c);
    else if(t[0]=="RETURN")handleReturn(t,c);
    else if(t[0]=="RETURN_ALL")handleReturnAll(t,c);
    else if(t[0]=="RESEARCHER_LOAD")handleResearcherLoad(t,c);
    else if(t[0]=="MATCH_RARITY")handleMatchRarity(t,c);
    else if(t[0]=="PRINT_UNASSIGNED")handlePrintUnassigned(t,c);
    else if(t[0]=="PRINT_STATS")handlePrintStats(t,c);
    else if(t[0]=="CLEAR")handleClear(t,c);
    else cout<<"Error: Unknown command '"<<t[0]<<"'"<<endl;
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).
    int id=stoi(tokens[1]),rar=stoi(tokens[3]),val=stoi(tokens[4]);;    
    Artifact a(id,tokens[2],rar,val);
    if(artifactTree.insertArtifact(a))cout<<"Artifact "<<id<<" added."<<endl;
    else cout<<"Error: Artifact "<<id<<" already exists."<<endl;
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.
    int id=stoi(tokens[1]);
    ArtifactNode* n=artifactTree.findArtifact(id);
    if(!n){cout<<"Error: Artifact "<<id<<" not found."<<endl; return;}
    if(n->data.assignedToName!="")
    {
        ResearcherNode* r=researcherTree.findResearcher(n->data.assignedToName);
        if(r)r->data.removeArtifact(id);
        artifactTree.clearAssignedTo(id);
    }
    if(artifactTree.removeArtifact(id))cout<<"Artifact "<<id<<" removed."<<endl;
    else cout<<"Error: Artifact "<<id<<" not found."<<endl;
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.
    int cap=stoi(tokens[2]);
    Researcher r(tokens[1],cap);
    if(researcherTree.insertResearcher(r))cout<<"Researcher "<<tokens[1]<<" hired."<<endl;
    else cout<<"Error: Researcher "<<tokens[1]<<" already exists."<<endl;
    
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.
    ResearcherNode* r=researcherTree.findResearcher(tokens[1]);
    if(!r){cout<<"Error: Researcher "<<tokens[1]<<" not found."<<endl;return;}
    for(int i=0;i<r->data.numAssigned;i++)artifactTree.clearAssignedTo(r->data.assignedArtifacts[i]);
    researcherTree.removeResearcher(tokens[1]);
    cout<<"Researcher "<<tokens[1]<<" fired."<<endl;
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."
    int id=stoi(tokens[2]);
    ResearcherNode* r=researcherTree.findResearcher(tokens[1]);
    if(!r){cout<<"Error: Researcher "<<tokens[1]<<" not found."<<endl;return;}
    ArtifactNode* a=artifactTree.findArtifact(id);
    if(!a){cout<<"Error: Artifact "<<id<<" not found."<<endl;return;}
    if(a->data.assignedToName!=""){cout<<"Error: Artifact "<<id<<" is already assigned."<<endl;return;}
    if(r->data.numAssigned>=r->data.capacity){cout<<"Error: Researcher "<<tokens[1]<<" is at full capacity."<<endl;return;}
    if(!r->data.addArtifact(id)){cout<<"Error: Researcher "<<tokens[1]<<" is at full capacity."<<endl;return;}
    artifactTree.setAssignedTo(id,tokens[1]);
    a=artifactTree.findArtifact(id);
    if(a)a->data.updateValueBasedOnUsage();
    cout<<"Artifact "<<id<<" assigned to "<<tokens[1]<<"."<<endl;
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."
    int id=stoi(tokens[2]);
    ResearcherNode* r=researcherTree.findResearcher(tokens[1]);
    if(!r){cout<<"Error: Researcher "<<tokens[1]<<" not found."<<endl;return;}
    ArtifactNode* a=artifactTree.findArtifact(id);
    if(!a){cout<<"Error: Artifact "<<id<<" not found."<<endl;return;}
    if(a->data.assignedToName!=tokens[1]){cout<<"Error: Artifact "<<id<<" not assigned to "<<tokens[1]<<"."<<endl;return;}
    r->data.removeArtifact(id);
    artifactTree.clearAssignedTo(id);
    cout<<"Artifact "<<id<<" returned by "<<tokens[1]<<"."<<endl;
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.
    ResearcherNode* r=researcherTree.findResearcher(tokens[1]);
    if(!r){cout<<"Error: Researcher "<<tokens[1]<<" not found."<<endl;return;}
    for(int i=0;i<r->data.numAssigned;i++)artifactTree.clearAssignedTo(r->data.assignedArtifacts[i]);
    r->data.removeAllArtifacts();
    cout<<"All artifacts returned by "<<tokens[1]<<"."<<endl;
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.
    ResearcherNode* r=researcherTree.findResearcher(tokens[1]);
    if(!r){cout<<"Error: Researcher "<<tokens[1]<<" not found."<<endl;return;}
   // cout<<"Researcher "<<tokens[1]<<" is supervising "<<r->data.numAssigned<<" artifacts."<<endl;
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.
    int mn=stoi(tokens[1]);
    cout<<"=== MATCH_RARITY "<<mn<<" ==="<<endl;
    artifactTree.printMatchRarity(mn);
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().
    cout<<"Unassigned artifacts:"<<endl;
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)
    int ta=artifactTree.getArtifactCount(),tr=researcherTree.getResearcherCount(),ar=ta?artifactTree.getTotalRarity()/ta:0,al=tr?researcherTree.getTotalLoad()/tr:0;
    cout<<"=== SYSTEM STATISTICS ==="<<endl;
    cout<<"Artifacts: "<<ta<<endl;
    cout<<"Researchers: "<<tr<<endl;
    cout<<"Average rarity: "<<ar<<endl;
    cout<<"Average load: "<<al<<endl;
    cout<<"Researchers:"<<endl;
    researcherTree.traversePreOrderForStats();
    cout<<"Artifacts:"<<endl;
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    artifactTree.clear();
    researcherTree.clear();
    // e.g. "All data cleared."
    cout <<"All data cleared."<<endl;
}
