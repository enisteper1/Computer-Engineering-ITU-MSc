#include "BlockNode.h"
#include "Node.h"

BlockNode::BlockNode(){
    this->head = NULL;
    this->next = NULL;
    this->hash = "0";
}


BlockNode::~BlockNode(){
    if(this->head !=NULL){
        delete this->head;
        this->head=NULL;
    }
    
    if(this->next != NULL)
        delete this->next; 

}

void BlockNode::addNode(Node* anode){
    if(this->head==NULL){
        this->head=anode;
    }
    else{ //added at the end of the list
        Node *p = this->head;
        while(p->getnext() != NULL)
            p=p->getnext();
        p->setnext(anode);
    }
}

BlockNode* BlockNode::getnext(){
    return this->next;
}

void BlockNode::setnext(BlockNode* bn){
    this->next=bn;
}

void BlockNode::setHash(string hash){
    this->hash=hash;
}

string BlockNode::getHash(){
    return this->hash;
}

Node* BlockNode::gethead(){
    return this->head;
}

string BlockNode::toString(){
    string s=this->hash;

    Node* p =this->head;
    while(p!=NULL){
        s += p->toString();
        p=p->getnext();
    }
    return s;
}