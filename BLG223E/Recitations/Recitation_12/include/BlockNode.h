#pragma once
#include "Node.h"
#include <string>

using namespace std;

class BlockNode{
    private:
        Node* head;
        string hash;
        BlockNode* next;

    public:
        BlockNode();
        ~BlockNode();
        void addNode(Node*);
        BlockNode* getnext();
        void setnext(BlockNode*);
        void setHash(string);
        string getHash();
        void calculateHash();
        Node* gethead();
        string toString();
};