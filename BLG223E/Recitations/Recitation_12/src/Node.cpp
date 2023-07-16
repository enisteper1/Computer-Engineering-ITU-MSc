#include "Node.h"
#include <iostream>

using namespace std;

Node::Node(string from, string to, int amount){
    this->from = from;
    this->to = to;
    this->transfer = amount;
    this->next = NULL;
}

Node::~Node(){
    if(this->next != NULL)
        delete this->next; 
}

string Node::toString(){
    return this->from+this->to+to_string(this->transfer);
}

void Node::print(){
    cout<<this->from<<"-"<<this->to<<"-"<<this->transfer<<" ";
}

Node* Node::getnext(){
    return this->next;
}

void Node::setnext(Node* next){
    this->next = next;
}

string Node::getfrom(){
    return this->from;
}
string Node::getto(){
    return this->to;
}
int Node::getamount(){
    return this->transfer;
}