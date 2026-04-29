#include "RedBlackTree.h"
using namespace std;
RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO
    if(!node)return;
    clear(node->left);
    clear(node->right);
    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name
    while(node){
        if(fullName>node->data.fullName)node=node->right;
        else if(fullName<node->data.fullName)node = node->left;
        else return node;
    }
    return nullptr;
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    ResearcherNode* t =new ResearcherNode(researcher);
    t->left = t->right = t->parent = nullptr;
    t->color = RED;
    bool flag = false;
    root= bstInsert(root,t,flag);
    if(!flag) {delete t; return false;}
    insertFixup(t);
    return true;

    //attempt to insert, erturn true if success, false otherwise
    return false;//temporary, you may delete this
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).
    if(!current){
        inserted = true;
        node->parent = nullptr;
        return node;
    }
    if(node->data.fullName<current->data.fullName)
    {
        ResearcherNode* t=bstInsert(current->left,node,inserted);
        current->left = t;
        if(t) t->parent = current;
    }
    else if(node->data.fullName>current->data.fullName)
    {
        ResearcherNode* t=bstInsert(current->right,node,inserted);
        current->right = t;
        if(t) t->parent = current;
    }
    else {
        inserted =false;
        return current;
    }
    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.
    while (node&&node != root&&node->parent->color==RED)
    {
        if (!(node->parent->parent))break;
        ResearcherNode *parent = node->parent,*secondpar = parent->parent;;
        if (parent==secondpar->left)
        {
            ResearcherNode *grandparother = secondpar->right;
            if (grandparother&&grandparother->color == RED)
            {
                parent->color = BLACK;
                grandparother->color = BLACK;
                secondpar->color = RED;
                node = secondpar;
            }
            else
            {
                if (node==parent->right)
                {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    secondpar = nullptr;
                    if(parent) secondpar = parent->parent;
                }
                if(parent)parent->color=BLACK;
                if(secondpar) {secondpar->color = RED; rotateRight(secondpar);}
            }
        }
        else
        {
            ResearcherNode *grandparother = secondpar->left;
            if (grandparother&&grandparother->color==RED)
            {
                parent->color = BLACK;
                grandparother->color = BLACK;
                secondpar->color = RED;
                node = secondpar;
            }
            else
            {
                if (node==parent->left)
                {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    secondpar = nullptr;
                    if(parent)secondpar = parent->parent;
                }
                if(parent)parent->color=BLACK;
                if(secondpar){secondpar->color=RED; rotateLeft(secondpar);}
            }
        }
    }

    if (root)root->color=BLACK;

}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.
    ResearcherNode *z = find(root, fullName),*x=nullptr,*xpar=nullptr;
    if (!z)return false;
    ResearcherNode *y=z;
    Color ycol=y->color;
    if (!z->left)
    {
        x = z->right;
        xpar = z->parent;
        if (!z->parent)root=z->right;
        else if (z==z->parent->left)z->parent->left=z->right;
        else z->parent->right=z->right;
        if (z->right)z->right->parent=z->parent;
    }
    else if (!z->right)
    {
        x = z->left;
        xpar = z->parent;
        if (!z->parent) root=z->left;
        else if (z==z->parent->left)z->parent->left=z->left;
        else z->parent->right=z->left;
        if (z->left) z->left->parent=z->parent;
    }
    else
    {
        y = minimum(z->right);
        ycol = y->color;
        x = y->right;
        if (y->parent==z)
        {
            xpar = y;
            if (x)x->parent = y;
        }
        else
        {
            xpar = y->parent;
            if (y==y->parent->left) y->parent->left=y->right;
            else y->parent->right=y->right;
            if (y->right) y->right->parent=y->parent;
            y->right=z->right;
            y->right->parent = y;
        }
        if (!z->parent)root=y;
        else if (z==z->parent->left) z->parent->left=y;
        else z->parent->right=y;
        y->parent = z->parent;
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (ycol==BLACK)deleteFixup(x,xpar);
    if (root)root->color=BLACK;
    return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *x, ResearcherNode *p)
{
    // TODO: Standard Red-Black deletion fixup.
    while(x!=root&&(!x||x->color==BLACK))
    {
        if(!p)break;

        if(x==p->left)
        {
            ResearcherNode *w=p->right;

            if(w&&w->color==RED)
            {
                w->color=BLACK;
                p->color=RED;
                rotateLeft(p);
                w=p->right;
            }

            if((!w||!w->left||w->left->color==BLACK)&&(!w||!w->right||w->right->color==BLACK))
            {
                if(w)w->color=RED;
                x=p;
                p=x->parent;
            }
            else
            {
                if(!w||!w->right||w->right->color==BLACK)
                {
                    if(w&&w->left)w->left->color=BLACK;
                    if(w)w->color=RED;
                    if(w)rotateRight(w);
                    w=p->right;
                }

                if(w)w->color=p->color;
                p->color=BLACK;
                if(w&&w->right)w->right->color=BLACK;
                rotateLeft(p);
                x=root;
                p=nullptr;
            }
        }
        else
        {
            ResearcherNode *w=p->left;

            if(w&&w->color==RED)
            {
                w->color=BLACK;
                p->color=RED;
                rotateRight(p);
                w=p->left;
            }

            if((!w||!w->right||w->right->color==BLACK)&&(!w||!w->left||w->left->color==BLACK))
            {
                if(w)w->color=RED;
                x=p;
                p=x->parent;
            }
            else
            {
                if(!w||!w->left||w->left->color==BLACK)
                {
                    if(w&&w->right)w->right->color=BLACK;
                    if(w)w->color=RED;
                    if(w)rotateLeft(w);
                    w=p->left;
                }

                if(w)w->color=p->color;
                p->color=BLACK;
                if(w&&w->left)w->left->color=BLACK;
                rotateRight(p);
                x=root;
                p=nullptr;
            }
        }
    }

    if(x)x->color=BLACK;
}



ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    if(!node) return nullptr;
    while(node->left) node = node->left;
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.
    if(!x ||!(x->right)) return;
    ResearcherNode* y=x->right;
    x->right = y->left;
    if(y->left) y->left->parent = x;
    y->parent = x->parent;
    if(!(x->parent))root = y;
    else if(x==x->parent->left)x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    if(!y || !(y->left)) return;
    ResearcherNode* x=y->left;
    y->left = x->right;
    if(x->right) x->right->parent = y;
    x->parent = y->parent;
    if(!(y->parent))root = x;
    else if(y==y->parent->left)y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if(!node) return 0;
    return 1+getResearcherCount(node->left)+getResearcherCount(node->right);
    return 0;
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if(!node) return 0;
    return node->data.numAssigned+getTotalLoad(node->right)+getTotalLoad(node->left);
    return 0;
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.
    if(!node)return;
    cout<<node->data.fullName<<" "<<node->data.capacity<<" "<<node->data.numAssigned<<endl;
    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}
