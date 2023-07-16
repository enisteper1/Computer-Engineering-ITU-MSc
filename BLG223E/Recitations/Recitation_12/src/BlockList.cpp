#include "BlockList.h"
#include "md5.h"
#include <map>
#include <iostream>

using namespace std;

BlockList::BlockList(){
    this->head = NULL;
    this->tail = NULL;
}

BlockList::~BlockList(){
    if(this->head!=NULL)
        delete this->head;
    this->head=NULL;
    this->tail=NULL;
}

void BlockList::addBlock(BlockNode* anode){
    if(this->head==NULL){
        this->head=anode;
        this->tail=this->head;
    }
    else{ //added at the end of the list
        MD5 md5;
        anode->setHash(md5(this->tail->toString()));
        this->tail->setnext(anode);
        this->tail = this->tail->getnext();
    }

}

void BlockList::printBalance(map<string,int> balances){
    BlockNode* p=this->head;

    while(p!=NULL){
        Node* n=p->gethead();
        while(n!=NULL){
            balances[n->getfrom()] -= n->getamount();
            balances[n->getto()] += n->getamount();
            n=n->getnext();
        }
        p=p->getnext();
    }

    for (auto const& x : balances){
        cout << x.first << ':' << x.second << " ";
    }
}

void BlockList::print(){
    if(this->tail!=NULL)
        cout<<this->tail->getHash()<<" ";
    else cout<<"NA ";
}