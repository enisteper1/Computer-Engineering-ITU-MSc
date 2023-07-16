#pragma once
#include "BlockNode.h"
#include <map>

class BlockList{
    private:
        BlockNode* head;
        BlockNode* tail;

    public:
        BlockList();
        ~BlockList();
        void addBlock(BlockNode*);
        void printBalance(map<string,int>);
        void print();
};