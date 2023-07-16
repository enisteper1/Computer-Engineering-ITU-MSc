#include "Node.h"

class IntervalTree
{
    private:
        Node* root;
        
    public:
        IntervalTree();
        Node* createNode(Interval);
        Node* add(Node*,Interval);
        void add(Interval);
        bool doOverlap(Interval, Interval);
        Interval* overlapSearch(Node*, Interval);
        Interval* overlapSearch(Interval);
        void inorder_traversal(Node*);
        void inorder_traversal();
        Node* minlowinterval(Node*);
        int findmax(int,int,int);
        Node* remove(Node*,Interval);
        void remove(Interval);
};