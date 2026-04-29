#include "AVLTree.h"
using namespace std;
AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
//TODO
if(!node)return;
clear(node->left); clear(node->right); delete(node);
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root
    ArtifactNode *y = x->right;
    ArtifactNode *z = nullptr;
    if(y){
        z = y->left;
    }
    y->left = x;
    x->right = z;
    x->height = 1+max(height(x->left),height(x->right));
    y->height = 1+max(height(y->left),height(y->right));
    return y;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).
    ArtifactNode *x = y->left;
    ArtifactNode *z = nullptr;
    if(x){
        z = x->right;
    }
    x->right = y;
    y->left = z;
    y->height = 1+max(height(y->left),height(y->right));
    x->height = 1+max(height(x->left),height(x->right));
    return x;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.
    if(!node) return nullptr;
    ArtifactNode*  a=node;
    while(a->left)a= a->left;
    return a;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.
    if(!node){
        inserted = true;
        return new ArtifactNode(artifact);
    }
    if (artifact.artifactID < node->data.artifactID)
    {
        node->left = insert(node->left,artifact,inserted);
    }
    else if (artifact.artifactID > node->data.artifactID)
    {
        node->right = insert(node->right,artifact,inserted);
    }
    else {
        inserted = false;
        return node;
    }
    node->height =1+max(height(node->left),height(node->right));
    int b = getBalance(node);
    if(b>1 && artifact.artifactID<node->left->data.artifactID)return rotateRight(node);
    if(b<-1&& artifact.artifactID>node->right->data.artifactID)return rotateLeft(node);
    if(b>1&&artifact.artifactID>node->left->data.artifactID){ node->left = rotateLeft(node->left); return rotateRight(node);}
    if(b<-1 && artifact.artifactID<node->right->data.artifactID){node->right = rotateRight(node->right); return rotateLeft(node);}
    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.
    if(!node)return nullptr;
    if(artifactID<node->data.artifactID)node->left = remove(node->left,artifactID,removed);
    else if(artifactID>node->data.artifactID)node->right = remove(node->right,artifactID,removed);
    else{
        removed = true;
        if(node->left && node->right){
            ArtifactNode *t = findMinNode(node->right);
            node->data = t->data;
            bool flag = false;
            node->right = remove(node->right,t->data.artifactID,flag);
        }
        else if(!node->left&&!node->right){
            delete node;
            return nullptr;
        }
        else {
            //cout<<"burasıı: "<<endl;
        //"cout<<removed;"
        //cout <<"buranın üstüüü:"<<endl;
    
            if(node->right){
                ArtifactNode* t = node->right;
                delete node;
                return t;
            }
            else {
                ArtifactNode* t = node->left;
                delete node;
                return t;
            }
        }
    }
    if(!node)return nullptr;
    node->height = 1+max(height(node->left),height(node->right));
    int b = getBalance(node);
    if(b>1&&getBalance(node->left)>=0)return rotateRight(node);
    if(b>1&&getBalance(node->left)<0){node->left = rotateLeft(node->left); return rotateRight(node);}
    if(b<-1&&getBalance(node->right)<=0)return rotateLeft(node);
    if(b<-1&&getBalance(node->right)>0){node->right = rotateRight(node->right);return rotateLeft(node);}
    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO
    ArtifactNode* c =node;
    while(c){
        if(artifactID<c->data.artifactID)c = c->left;
        else if(artifactID>c->data.artifactID)c=c->right;
        else return c;
    }    

    return nullptr;
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{

    //assign artifact to researcher
    ArtifactNode* c=findArtifact(artifactID);
    if(!c)return;
    c->data.assignedToName=researcherName;

}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode* c=findArtifact(artifactID);
    if(!c)return;
    c->data.assignedToName="";
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>
    if(!node)return;
    printUnassigned(node->left);
    if(node->data.assignedToName==""){
       cout<<node->data.artifactID<<" "<<node->data.name<<" "<<node->data.rarityLevel<<" "<<node->data.researchValue<<endl;
    }
    printUnassigned(node->right);
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    if(!node)return 0;
    return 1+getArtifactCount(node->left)+getArtifactCount(node->right);
    // TODO: return size of subtree.
    return 0;
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    if(!node)return 0;
    return node->data.rarityLevel+getTotalRarity(node->left)+getTotalRarity(node->right);
    // TODO: sum of rarityLevel over subtree.
    return 0;
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.
    if(!node)return;
    traversePostOrderForStats(node->left);
    traversePostOrderForStats(node->right);
    cout<<node->data.artifactID<<" "<<node->data.name<<" "<<node->data.rarityLevel<<" "<<node->data.researchValue<<" ";
    if(node->data.assignedToName=="")cout<<"UNASSIGNED"<<endl;
    else cout<<node->data.assignedToName<<endl;
    
}

void AVLTree::printMatchRarity(int mn) const
{
    printMatchRarity(root,mn);
}

void AVLTree::printMatchRarity(ArtifactNode *node,int mn) const
{
    if(!node)return;
    printMatchRarity(node->left,mn);
    if(node->data.rarityLevel>=mn)
    {
        cout<<node->data.artifactID<<" "<<node->data.name<<" "<<node->data.rarityLevel<<" "<<node->data.researchValue<<" ";
        if(node->data.assignedToName=="")cout<<"UNASSIGNED"<<endl;
        else cout<<"ASSIGNED:"<<node->data.assignedToName<<endl;
    }

    printMatchRarity(node->right,mn);
}

