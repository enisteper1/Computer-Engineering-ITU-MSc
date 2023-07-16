//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname:
//---Student Number:
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-------------Initialize Priority Queue-------------//
PriorityQueue::PriorityQueue(){
    //TODO
    this->head = NULL;
};

//-------------Insert New Node To Priority Queue-------------//
void PriorityQueue::enque(Node* newnode){
    //TODO
    if(this->head == NULL)
        head = newnode;
    else
    {
        // If input node lower than head
        // Set new head as input node
        if(newnode->token.val < head->token.val)
        {
            Node* tmp = head;
            head = newnode;
            head->next = tmp;
        }
        // If node token val higher than head
        else
        {   // traverse list one by one
            Node* p = head;
            while(p != NULL)
            {
                // If node token val equal to traversed node
                if(newnode->token.val == p->token.val)
                {
                    // If also following node has same value
                    // continue on next one
                    if(p->next != NULL && newnode->token.val == p->next->token.val)
                    {
                        p=p->next;
                    }
                    // If 
                    else
                    {
                        Node* tmp = p->next;
                        p->next = newnode;
                        newnode->next = tmp;
                        break;
                    }
                }
                // If input node has higher value than traversed one
                else if(newnode->token.val > p->token.val)
                    {
                        // If also it has following node
                        // and the following node has equal or lesser value
                        if(p->next != NULL && newnode->token.val >= p->next->token.val)
                            p = p->next;
                        // If there is no following node
                        // or the following node has greater value
                        else
                        {
                            Node* tmp = p->next;
                            p->next = newnode;
                            newnode->next = tmp;
                            break;
                        }
                        
                    }
            }
        }
    }
};

//-------------Remove Node From Priority Queue-------------//
Node* PriorityQueue::dequeue(){
    // Delete node with least frequency
    if(head != NULL)
    {
        Node* tmp = head;
        head = head->next;
        return tmp;
    }
    return NULL;
};

//-------------Initialize Tree-------------//
Tree::Tree(){
    this->root = NULL;
};

//-------------Delete Tree Nodes From Memory-------------//
Tree::~Tree(){
    //Start deleting from root
    deleteTree(root);
};

//-------------Delete Tree From Memory-------------//
void Tree::deleteTree(Node* node){
    //-------------This Function Is Not Necessary-------------//
    //-----------------Use It At Your Own Will----------------//
    // Delete all nodes
    if(node != NULL)
    {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

//-------------Merges Two Node Into One-------------//
Node* Tree::mergeNodes(Node* temp1, Node* temp2){
   //Generate new node
   Node* p = new Node();
   // combine strings
   p->token.symbol = temp1->token.symbol + temp2->token.symbol;
   // sum values
   p->token.val = temp1->token.val + temp2->token.val;
   return p;
};

void Tree::printTree(Node* traverse, int level){
    cout << level << "\t";
    for (int i = 0; i < level; i++)
        cout << "\t"; 

    cout << traverse->token.symbol << "(" << traverse->token.val << ")" << endl;

    if(traverse->left)
        printTree(traverse->left, level + 1);  
    if(traverse->right)
        printTree(traverse->right, level + 1);
};