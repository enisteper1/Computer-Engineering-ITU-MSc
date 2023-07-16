#include <stdio.h>
#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(Interval i,int max)
{
    this->i = new Interval(i);
    this->max = max;
    this->left=NULL;
    this->right=NULL;
}

void Node::set_left(Node* left)
{
    this->left = left;
}

void Node::set_right(Node* right)
{
    this->right = right;
}

void Node::set_i(int low, int high)
{
    this->i->set_low(low);
    this->i->set_high(high);
}

void Node::set_max(int max)
{
    this->max = max;
}

Node* Node::get_left()
{
    return this->left;
}

Node* Node::get_right()
{
    return this->right;
}

Interval* Node::get_i()
{
    return this->i;
}

int Node::get_max()
{
    return this->max;
}